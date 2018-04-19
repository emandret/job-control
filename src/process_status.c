/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:02:33 by emandret          #+#    #+#             */
/*   Updated: 2018/04/19 22:46:44 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Set the state of each process corresponding to the status.
*/

static int	mark_process_by_pid(t_process *p, int status)
{
	if (WIFSTOPPED(status))
	{
		format_job_info(0, p->pid, "stopped", p->argv[0]);
		mark_process_state(p, ST_STOPPED);
	}
	else
	{
		if (WIFSIGNALED(status))
			format_job_info(0, p->pid, "killed", p->argv[0]);
		mark_process_state(p, ST_COMPLETED);
	}
	if (!is_forkable(p->next))
		return (0);
	return (1);
}

/*
** Mark the state of each process, according to the status info returned by
** waitpid(2).
**
** @return 1 if the status has been marked, 0 if not marked, -1 if an error has
** occurred.
*/

static int	mark_process_status(pid_t pid, int status)
{
	t_job		*j;
	t_process	*p;

	if (pid > 0)
	{
		j = g_first_job;
		while (j)
		{
			p = j->first_process;
			while (p)
			{
				if (p->pid == pid)
					return (mark_process_by_pid(p, status));
				p = p->next;
			}
			j = j->next;
		}
		return (0);
	}
	else if (pid == 0 || (pid == -1 && errno == ECHILD))
		return (0);
	return (-1);
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
		if (mark_process_status(pid, status) <= 0)
			break ;
	}
}

/*
** 1. Check if child processes wish to report status. Execution of the program
**    IS SUSPENDED if a child process is running and does not report status.
**    Program will resume only when all the processes associated with job J are
**    terminated or signaled.
**
** 2. Wait (blocking mode) for a child process to report status. If the
**    WUNTRACED option is set, children of the current process that are stopped
**    due to a SIGTTIN, SIGTTOU, SIGTSTP, or SIGSTOP signal also have their
**    status reported.
**
** 3. If status has been marked, wait for another child process by calling
**    waitpid(2) again. If status has not been marked, exit the loop.
*/

void		wait_for_job(t_job *j)
{
	int		status;
	pid_t	pid;

	if (!is_forkable(j->first_process))
		return ;
	while (1)
	{
		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
		if (mark_process_status(pid, status) <= 0 ||
			check_job_state(j, ST_COMPLETED) ||
			check_job_state(j, ST_STOPPED))
			break ;
	}
}
