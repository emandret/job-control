/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_notify.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 18:37:29 by emandret          #+#    #+#             */
/*   Updated: 2018/03/25 19:42:15 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Print formatted info about job status.
*/

static void	format_job_info(t_job *j, const char *status)
{
	fprintf(stderr, "%jd (%s): %s\n", (intmax_t)j->pgid, status, j->command);
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
		format_job_info(j, "completed");
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
