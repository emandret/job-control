/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:02:33 by emandret          #+#    #+#             */
/*   Updated: 2018/03/27 22:51:16 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Set the status.
**
** @return  Boolean.
*/

static bool	set_status(t_process *p, pid_t pid, int status)
{
	if (p->pid == pid)
	{
		p->status = status;
		if (WIFSTOPPED(status))
			p->stopped = true;
		else if (WIFEXITED(status))
			p->completed = true;
		else if (WIFSIGNALED(status))
		{
			p->completed = true;
			fprintf(stderr, "%jd: Terminated by signal %d\n", (intmax_t)pid,
					WTERMSIG(p->status));
		}
		return (true);
	}
	return (false);
}

/*
** Loop through all the processes of all jobs to mark the status of
** the process running under the process ID pid.
**
** @return  Boolean.
*/

static bool	mark_process_status(pid_t pid, int status)
{
	t_process	*p;
	t_job		*j;

	if (pid > 0)
	{
		j = g_first_job;
		while (j)
		{
			p = j->first_process;
			while (p)
			{
				if (set_status(p, pid, status))
					return (true);
				p = p->next;
			}
			j = j->next;
		}
	}
	else if (pid == 0 || (pid == -1 && errno == ECHILD))
		return (false);
	return (false);
}

/*
** 1. Check if child processes wish to report status. Execution of the program
**    IS NOT SUSPENDED if a child process is running and does not report status.
**
** 2. Wait (non-blocking mode) for a child process to report status. Otherwise,
**    if WNOHANG is specified and there are no stopped or exited children,
**    0 is returned.
**
** 3. If status has been marked, wait for another child process by calling
**    waitpid(2) again. If status has not been marked, exit the loop.
*/

void		check_for_status(void)
{
	int		status;
	pid_t	pid;

	while (1)
	{
		pid = waitpid(WAIT_ANY, &status, WUNTRACED | WNOHANG);
		if (!mark_process_status(pid, status))
			break ;
	}
}

/*
** 1. Check if child processes wish to report status. Execution of the program
**    IS SUSPENDED if a child process is running and does not report status.
**    Program will resume only when all the processes associated with job J
**    are terminated or signaled.
**
** 2. Wait (blocking mode) for a child process to report status. If the
**    WUNTRACED option is set, children of the current process that are
**    stopped due to a SIGTTIN, SIGTTOU, SIGTSTP, or SIGSTOP signal also
**    have their status reported.
**
** 3. If status has been marked, wait for another child process by calling
**    waitpid(2) again. If status has not been marked, exit the loop.
*/

void		wait_for_job(t_job *j)
{
	int		status;
	pid_t	pid;

	while (1)
	{
		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
		if (!mark_process_status(pid, status) ||
			job_is_completed(j) ||
			job_is_stopped(j))
			break ;
	}
}
