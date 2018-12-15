/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_schedule.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 18:28:57 by emandret          #+#    #+#             */
/*   Updated: 2018/04/18 03:39:30 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** 1. Put a process in foreground using tcsetpgrp(3) by giving terminal access
**    to the process group for receiving signals and stdin.
**
** 2. Wait and block program for the processes to finish or report status.
**
** 3. Reset terminal attributes.
*/

void	put_job_in_foreground(t_job *j, bool cont)
{
	tcsetpgrp(g_shell.terminal_fd, j->pgid);
	if (cont)
	{
		tcsetattr(g_shell.terminal_fd, TCSADRAIN, &j->tmodes);
		if (kill(-j->pgid, SIGCONT) == -1)
			perror("Couldn't kill (SIGCONT)");
	}
	wait_for_job(j);
	tcsetpgrp(g_shell.terminal_fd, g_shell.pgid);
	tcgetattr(g_shell.terminal_fd, &j->tmodes);
	tcsetattr(g_shell.terminal_fd, TCSADRAIN, &g_shell.tmodes);
}

/*
** A job not in foreground is already in background.
*/

void	put_job_in_background(t_job *j, bool cont)
{
	if (cont)
		if (kill(-j->pgid, SIGCONT) == -1)
			perror("Couldn't kill (SIGCONT)");
}
