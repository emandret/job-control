/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/29 04:07:01 by emandret          #+#    #+#             */
/*   Updated: 2018/04/19 23:57:28 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Set the process group ID pgid to the process corresponding to the process ID
** pid. If the process group ID pgid is zero, then the process group ID pgid is
** set to the process ID pid value.
**
** @return The process group ID pgid or -1 in case of error.
*/

pid_t	set_process_pgid(pid_t pid, pid_t pgid)
{
	if (pgid == 0)
		pgid = pid;
	if (setpgid(pid, pgid) == -1)
		return (-1);
	return (pgid);
}

/*
** Map the fildes file descriptor to fildes2 value if they are different.
*/

void	set_channel(int fildes, int fildes2)
{
	if (fildes > 0 && fildes != fildes2)
	{
		dup2(fildes, fildes2);
		close(fildes);
	}
}

/*
** Close the fildes file descriptor if different from fildes2.
*/

void	close_channel(int fildes, int fildes2)
{
	if (fildes != fildes2)
		close(fildes);
}

/*
** Check the state of process P, between ST_RUNNING, ST_STOPPED and
** ST_COMPLETED.
**
** @return Boolean.
*/

bool	check_process_state(t_process *p, t_state st)
{
	if (st == ST_RUNNING)
		return (!p->stopped && !p->completed);
	if (st == ST_STOPPED)
		return (p->stopped || p->completed);
	if (st == ST_COMPLETED)
		return (p->stopped && p->completed);
	return (false);
}

/*
** Mark the state of process P.
*/

void	mark_process_state(t_process *p, t_state st)
{
	if (st == ST_RUNNING)
	{
		p->stopped = false;
		p->completed = false;
	}
	else if (st == ST_STOPPED)
	{
		p->stopped = true;
		p->completed = false;
	}
	else if (st == ST_COMPLETED)
	{
		p->stopped = true;
		p->completed = true;
	}
}
