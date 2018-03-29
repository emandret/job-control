/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:02:33 by emandret          #+#    #+#             */
/*   Updated: 2018/03/29 04:56:08 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Set the status by process ID pid.
**
** @return  Int 0 or 1.
*/

static bool	mark_state_by_pid(pid_t pid, int status)
{
	t_job		*j;
	t_process	*p;

	j = g_first_job;
	while (j)
	{
		p = j->first_process;
		while (p)
		{
			if (p->pid == pid)
			{
				if (WIFSIGNALED(status))
					mark_process_state(p, ST_COMPLETED);
				else if (WIFSTOPPED(status))
					mark_process_state(p, ST_STOPPED);
				return (true);
			}
			p = p->next;
		}
		j = j->next;
	}
	return (false);
}

/*
**
*/

static bool	mark_process_status(pid_t pid, int status)
{
	t_job	*j;

	if (pid > 0)
		return (mark_state_by_pid(pid, status));
	else if (pid == 0 || (pid == -1 && errno == ECHILD))
	{
		j = g_first_job;
		while (j)
		{
			if (j->launched && check_job_state(j, ST_RUNNING))
				mark_job_state(j, ST_COMPLETED);
			j = j->next;
		}
	}
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
			!check_job_state(j, ST_RUNNING))
			break ;
	}
}
