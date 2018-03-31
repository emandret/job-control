/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 18:19:02 by emandret          #+#    #+#             */
/*   Updated: 2018/03/31 02:53:54 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Find a job by its process group ID pgid.
**
** @return The found job.
*/

t_job	*find_job_by_pgid(pid_t pgid)
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
** Find a job by its ID.
**
** @return The found job.
*/

t_job	*find_job_by_id(unsigned int id)
{
	t_job	*j;

	j = g_first_job;
	while (j)
	{
		if (j->id == id)
			return (j);
		j = j->next;
	}
	return (NULL);
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

/*
** Mark the state of job J.
*/

void	mark_job_state(t_job *j, t_state st)
{
	t_process	*p;

	p = j->first_process;
	while (p)
	{
		mark_process_state(p, st);
		p = p->next;
	}
	j->notified = false;
}
