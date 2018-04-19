/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_notify.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 18:37:29 by emandret          #+#    #+#             */
/*   Updated: 2018/04/19 21:42:12 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Print formatted info about job status.
*/

void	format_job_info(int id, pid_t pid, const char *status, const char *name)
{
	fprintf(stderr, "\n[%d] %05jd %-12s %s\n", id, (intmax_t)pid, status, name);
}

/*
** 1. Check if some processes want to report status. Non-blocking mode so the
**    program execution will not be suspended.
**
** 2. Loop through every job and notify if completed or stopped.
*/

void	do_job_notification(void)
{
	t_job	*j;

	check_for_status();
	j = g_first_job;
	while (j)
	{
		if (!j->notified && !check_job_state(j, ST_RUNNING))
		{
			if (check_job_state(j, ST_COMPLETED))
				format_job_info(j->id, j->pgid, "completed", j->name);
			else if (check_job_state(j, ST_STOPPED))
				format_job_info(j->id, j->pgid, "suspended", j->name);
			j->notified = true;
		}
		j = j->next;
	}
}
