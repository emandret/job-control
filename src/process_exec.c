/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:43:43 by emandret          #+#    #+#             */
/*   Updated: 2018/04/20 00:53:59 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Call the process if it is a built-in one.
**
** @return Boolean.
*/

static bool		call_if_builtin(t_process *p)
{
	if (p->builtin)
	{
		p->builtin(p->argv);
		close_channel(p->std.in, STDIN_FILENO);
		close_channel(p->std.out, STDOUT_FILENO);
		close_channel(p->std.err, STDERR_FILENO);
		return (true);
	}
	return (false);
}

/*
** Set all the process channels.
*/

static void		set_all_channels(t_process *p)
{
	set_channel(p->std.in, STDIN_FILENO);
	set_channel(p->std.out, STDOUT_FILENO);
	set_channel(p->std.err, STDERR_FILENO);
	set_channel(p->std.fd3, 3);
	set_channel(p->std.fd4, 4);
	set_channel(p->std.fd5, 5);
	set_channel(p->std.fd6, 6);
	set_channel(p->std.fd7, 7);
	set_channel(p->std.fd8, 8);
	set_channel(p->std.fd9, 9);
}

/*
** 1. Execute a built-in command or duplicate our running program using fork(2).
**    Exit in case of failure. Execute the process otherwise.
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
	if (!p->next && call_if_builtin(p))
	{
		mark_process_state(p, ST_COMPLETED);
		return ;
	}
	else if (!(p->pid = fork()))
	{
		if (g_shell.is_interactive)
		{
			set_process_pgid(getpid(), j->pgid);
			if (foreground)
				tcsetpgrp(g_shell.terminal_fd, j->pgid);
			switch_signal_handlers();
		}
		set_all_channels(p);
		if (call_if_builtin(p))
			exit(EXIT_SUCCESS);
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
	while (p)
	{
		if (p->next)
		{
			if (pipe(pipefd) == -1)
				exit(EXIT_FAILURE);
			p->std.out = pipefd[WRITE_END];
		}
		launch_process(p, j, foreground);
		close_channel(p->std.in, j->first_process->std.in);
		close_channel(p->std.out, get_last_process(j)->std.out);
		if ((p = p->next))
			p->std.in = pipefd[READ_END];
	}
}
