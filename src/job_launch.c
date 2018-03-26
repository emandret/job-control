/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_launch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 04:39:04 by emandret          #+#    #+#             */
/*   Updated: 2018/03/27 00:12:21 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** 1. Setup file descriptors corresponding to the job.
**
** 2. Launch the job J and its associated processes.
**
** 3. Put job J in foreground or background (by giving access to terminal
**    by process group ID pgid).
*/

void	launch_job(t_job *j, bool foreground)
{
	int	fd[3];

	fd[STDIN] = j->stdin;
	fd[STDOUT] = j->stdout;
	fd[STDERR] = j->stderr;
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
