/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_launch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 04:39:04 by emandret          #+#    #+#             */
/*   Updated: 2018/03/31 05:58:59 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** 1. Launch the job J (if not launched) and its associated processes.
**
** 2. Put job J in foreground or background (by giving access to terminal by
**    process group ID pgid).
*/

void	launch_job(t_job *j, bool foreground)
{
	if (!j->launched)
	{
		launch_job_processes(j, foreground);
		j->launched = true;
		if (!g_shell.is_interactive)
			wait_for_job(j);
		else if (foreground)
			put_job_in_foreground(j, false);
		else
			put_job_in_background(j, false);
	}
}

/*
** Continue the job J and mark it as running.
*/

void	continue_job(t_job *j, bool foreground)
{
	mark_job_state(j, ST_RUNNING);
	if (foreground)
		put_job_in_foreground(j, true);
	else
		put_job_in_background(j, true);
}
