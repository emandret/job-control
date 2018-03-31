/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:02:33 by emandret          #+#    #+#             */
/*   Updated: 2018/03/31 06:14:06 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Print formatted info about processes terminated by a signal.
*/

static void	format_termsig_info(t_process *p, int status)
{
	fprintf(stderr, "PID %jd (terminated by signal %d): %s\n", (intmax_t)p->pid,
			WTERMSIG(status), p->xpath);
}

/*
** Set the state of each process by process ID pid. Handle notifying for
** processes terminated by signals.
**
** @return Boolean.
*/

static bool	mark_process_state_by_pid(pid_t pid, int status)
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
				{
					format_termsig_info(p, status);
					mark_process_state(p, ST_COMPLETED);
				}
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
** 1. Mark the status of each process, setting the state after.
**
** 2. Two conditions are defined to set the state:
**
** .. 2.1. If pid > 0, then a child process has had its status reported, and its
** ..      process ID pid is returned. The process is then marked using it.
**
** .. 2.2. If pid = 0, or pid = -1 with the errno equals to ECHILD, then it that
** ..      there are no child processes to wait for and waitpid(2) exited. But,
** ..      if the job has been lauched, and is currently in running state, then
** ..      it means that the job has exited, so it is marked as completed after.
**
** @return Boolean.
*/

static bool	mark_process_status(pid_t pid, int status)
{
	t_job	*j;

	if (pid > 0)
		return (mark_process_state_by_pid(pid, status));
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

	while (1)
	{
		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
		if (!mark_process_status(pid, status) ||
			!check_job_state(j, ST_RUNNING))
			break ;
	}
}
