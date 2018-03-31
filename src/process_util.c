/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/29 04:07:01 by emandret          #+#    #+#             */
/*   Updated: 2018/03/31 06:14:35 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

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
		return (p->stopped && !p->completed);
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
