/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:02:33 by emandret          #+#    #+#             */
/*   Updated: 2018/04/19 21:09:00 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Set the state of each process corresponding to the status.
*/

static void	mark_process_state_by_status(t_job *j, t_process *p, int status)
{
	if (WIFSTOPPED(status))
		mark_process_state(p, ST_STOPPED);
	else if (WIFEXITED(status))
		mark_process_state(p, ST_COMPLETED);
	else if (WIFSIGNALED(status))
	{
		mark_process_state(p, ST_COMPLETED);
		format_job_info(j->id, p->pid, "killed", p->argv[0]);
	}
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
				{
					mark_process_state_by_status(j, p, status);
					return (1);
				}
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

	if (find_job_by_id(get_job_list_size()) == j)
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
