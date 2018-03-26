/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_launch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 04:39:04 by emandret          #+#    #+#             */
/*   Updated: 2018/03/26 05:27:19 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Launch the job J and its associated processes.
*/

void	launch_job(t_job *j, bool foreground)
{
	int	fd[3];

	launch_job_processes(j, fd, foreground);
	if (!g_shell.is_interactive)
		wait_for_job(j);
	else if (foreground)
		put_job_in_foreground(j, false);
	else
		put_job_in_background(j, false);
}

/*
** Continue the job J
*/

void	continue_job(t_job *j, bool foreground)
{
	mark_job_as_running(j);
	if (foreground)
		put_job_in_foreground(j, true);
	else
		put_job_in_background(j, true);
}
