/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_notify.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 18:37:29 by emandret          #+#    #+#             */
/*   Updated: 2018/03/27 22:49:47 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Print formatted info about job status.
*/

static void	format_job_info(t_job *j, const char *status)
{
	char			name[FORMAT_BUF];
	t_process		*p;
	unsigned int	i;

	bzero(name, FORMAT_BUF);
	p = j->first_process;
	while (p)
	{
		i = 0;
		while (p->argv[i])
		{
			strlcat(name, p->argv[i], FORMAT_BUF);
			if (p->argv[++i])
				strlcat(name, " ", FORMAT_BUF);
		}
		if ((p = p->next))
			strlcat(name, " | ", FORMAT_BUF);
	}
	fprintf(stderr, "[%d] %jd (%s): %s\n", j->id, (intmax_t)j->pgid,
			status, name);
}

/*
** Notify user if job J is completed and remove it from the job list.
**
** @return  Boolean.
*/

static bool	notify_if_completed(t_job *j)
{
	if (job_is_completed(j))
	{
		format_job_info(j, "completed");
		free_job(j);
		return (true);
	}
	return (false);
}

/*
** Notify user if job J is stopped and set notified attribute to true.
**
** @return  Boolean.
*/

static bool	notify_if_stopped(t_job *j)
{
	if (job_is_stopped(j) && !j->notified)
	{
		format_job_info(j, "stopped");
		j->notified = true;
		return (true);
	}
	return (false);
}

/*
** 1. Check if some processes want to report status. Non-blocking mode
**    so the program execution will not be suspended.
**
** 2. Loop through every job and notify if completed or stopped.
*/

void		do_job_notification(void)
{
	t_job	*j;
	t_job	*jlast;
	t_job	*jnext;

	check_for_status();
	jlast = NULL;
	j = g_first_job;
	while (j)
	{
		jnext = j->next;
		if (notify_if_completed(j))
		{
			if (jlast)
				jlast->next = jnext;
			else
				g_first_job = jnext;
		}
		notify_if_stopped(j);
		jlast = j;
		j = jnext;
	}
}
