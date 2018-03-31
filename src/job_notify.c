/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_notify.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 18:37:29 by emandret          #+#    #+#             */
/*   Updated: 2018/03/31 06:25:06 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Print formatted info about job status.
*/

void	format_job_info(t_job *j, const char *status)
{
	fprintf(stderr, "[%d] PGID %jd (%s): %s\n", j->id, (intmax_t)j->pgid,
			status, j->name);
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
				format_job_info(j, "completed");
			else if (check_job_state(j, ST_STOPPED))
				format_job_info(j, "stopped");
			j->notified = true;
		}
		j = j->next;
	}
}
