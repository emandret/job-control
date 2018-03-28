/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 18:19:02 by emandret          #+#    #+#             */
/*   Updated: 2018/03/28 22:41:06 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Find a job by its process group ID pgid.
**
** @return The found job.
*/

t_job	*find_job(pid_t pgid)
{
	t_job	*j;

	j = g_first_job;
	while (j)
	{
		if (j->pgid == pgid)
			return (j);
		j = j->next;
	}
	return (NULL);
}

/*
** Check the state of process P, between ST_RUNNING, ST_STOPPED
** and ST_COMPLETED.
**
** @return Boolean.
*/

bool	check_process_state(t_process *p, t_state st)
{
	if (st == ST_RUNNING)
		return (!p->stopped && !p->completed);
	if (st == ST_STOPPED)
		return (p->stopped || p->completed);
	if (st == ST_COMPLETED)
		return (p->completed);
	return (false);
}

/*
** Set the state of process P.
*/

void	set_process_state(t_process *p, t_state st)
{
	if (st == ST_RUNNING)
	{
		p->stopped = false;
		p->completed = false;
	}
	else if (st == ST_STOPPED)
	{
		p->stopped = true;
		p->completed = false;
	}
	else if (st == ST_COMPLETED)
	{
		p->stopped = false;
		p->completed = true;
	}
	else
	{
		p->stopped = true;
		p->completed = true;
	}
}

/*
** Check the state of job J.
**
** @return Boolean.
*/

bool	check_job_state(t_job *j, t_state st)
{
	t_process	*p;

	p = j->first_process;
	while (p)
	{
		if (!check_process_state(p, st))
			return (false);
		p = p->next;
	}
	return (true);
}
