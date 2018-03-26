/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 18:19:02 by emandret          #+#    #+#             */
/*   Updated: 2018/03/25 19:49:39 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Find a job by its process group ID pid.
**
** @return  The found job.
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
** Check if a job has stopped.
**
** @return  Boolean.
*/

bool	job_is_stopped(t_job *j)
{
	t_process	*p;

	p = j->first_process;
	while (p)
	{
		if (!p->completed && !p->stopped)
			return (false);
		p = p->next;
	}
	return (true);
}

/*
** Check if a job has completed.
**
** @return  Boolean.
*/

bool	job_is_completed(t_job *j)
{
	t_process	*p;

	p = j->first_process;
	while (p)
	{
		if (!p->completed)
			return (false);
		p = p->next;
	}
	return (true);
}
