/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:43:43 by emandret          #+#    #+#             */
/*   Updated: 2018/04/12 15:07:02 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Set the process group ID pgid to the process corresponding to the process ID
** pid. If the process group ID pgid is zero, then the process group ID pgid is
** set to the process ID pid value.
**
** @return The process group ID pgid or -1 in case of error.
*/

static pid_t	set_process_pgid(pid_t pid, pid_t pgid)
{
	if (pgid == 0)
		pgid = pid;
	if (setpgid(pid, pgid) == -1)
		return (-1);
	return (pgid);
}

/*
** Map the fildes file descriptor to fildes2 value if they are different.
*/

static void		set_channel(int fildes, int fildes2)
{
	if (fildes != fildes2)
	{
		dup2(fildes, fildes2);
		close(fildes);
	}
}

/*
** Close the fildes file descriptor if different from fildes2.
*/

static void		close_channel(int fildes, int fildes2)
{
	if (fildes != fildes2)
		close(fildes);
}

/*
** 1. Duplicate our running program using fork(2). Exit in case of failure.
**    Execute the process otherwise.
**
** 2. In the child process (pid = 0):
**
** .. 2.1. Verify if the shell is an interactive shell and set the process group
**         ID pgid.
**
** .. 2.2. If foreground is set to true, set the foreground process ID of the
**         terminal associated with the file descriptor to the process group
**         ID pgid.
**
** .. 2.3. Reset signal handlers.
**
** .. 2.4. Map the channels to stdin, stdout and stderr if they are different
**         (pipes), otherwise, do nothing. Then execute the process and exit.
**
** 3. In the parent process (pid > 0):
**
** .. 3.1. Verify if the shell is an interactive shell and set the process group
**         ID pgid.
*/

static void		launch_process(t_process *p, t_job *j, bool foreground)
{
	if (!(p->pid = fork()))
	{
		if (g_shell.is_interactive)
		{
			set_process_pgid(getpid(), j->pgid);
			if (foreground)
				tcsetpgrp(g_shell.terminal_fd, j->pgid);
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			signal(SIGTSTP, SIG_DFL);
			signal(SIGTTIN, SIG_DFL);
			signal(SIGTTOU, SIG_DFL);
			signal(SIGCHLD, SIG_DFL);
		}
		set_channel(p->std.in, STDIN_FILENO);
		set_channel(p->std.out, STDOUT_FILENO);
		set_channel(p->std.err, STDERR_FILENO);
		execve(p->xpath, p->argv, g_envp);
		exit(EXIT_FAILURE);
	}
	else if (p->pid > 0 && g_shell.is_interactive)
		j->pgid = set_process_pgid(p->pid, j->pgid);
	if (p->pid == -1 || j->pgid == -1)
		exit(EXIT_FAILURE);
}

/*
** 1. Launch all the pipelined processes associated with a job. Handle the pipes
**    to map file descriptors from the first to the last process.
**
** 2. Use pipe(2) to allocate two fildes for inter-process communication. If a
**    next process exists, stdout of process P is equal to write-end fildes of
**    the created pipe. Otherwise, process stdout is equal to the job stdout.
**
** 3. Call launch_process(), duplicating current running program and executing
**    the requested process.
**
** 4. Clean the pipe ends by closing the unused ones since they are duplicated
**    in the launched process. If the fildes value is equal to the job fildes
**    value, it is not closed.
*/

void			launch_job_processes(t_job *j, bool foreground)
{
	t_process	*p;
	int			pipefd[2];

	p = j->first_process;
	p->std.in = j->std.in;
	while (p)
	{
		if (p->next)
		{
			if (pipe(pipefd) == -1)
				exit(EXIT_FAILURE);
			p->std.out = pipefd[WRITE_END];
		}
		else
			p->std.out = j->std.out;
		p->std.err = j->std.err;
		launch_process(p, j, foreground);
		close_channel(p->std.in, j->std.in);
		close_channel(p->std.out, j->std.out);
		if ((p = p->next))
			p->std.in = pipefd[READ_END];
	}
}
