/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_util2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 00:43:33 by emandret          #+#    #+#             */
/*   Updated: 2018/04/20 00:44:41 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** True if forkable. Specially made for built-in.
**
** @return Boolean.
*/

bool		is_forkable(t_process *p)
{
	return (!(p && p->builtin && !p->next));
}

/*
** Get the last process.
**
** @return The last process.
*/

t_process	*get_last_process(t_job *j)
{
	t_process	*p;

	p = j->first_process;
	while (p->next)
		p = p->next;
	return (p);
}
