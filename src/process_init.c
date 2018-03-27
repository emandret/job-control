/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:40:19 by emandret          #+#    #+#             */
/*   Updated: 2018/03/27 22:13:44 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Duplicate the argv array.
**
** @return          The duplicated array.
*/

static char			**tabdup(char **t)
{
	unsigned int	i;
	char			**nt;

	i = 1;
	while (t[i++])
		;
	ASSERT_MALLOC(nt = malloc(sizeof(char*) * i));
	i = 0;
	while (t[i])
	{
		nt[i] = strdup(t[i]);
		i++;
	}
	nt[i] = NULL;
	return (nt);
}

/*
** Create a new process. Not intended to be used outside this scope.
**
** @return          The created process.
*/

static t_process	*init_process(char *xpath, char **argv)
{
	t_process	*p;

	ASSERT_MALLOC(p = malloc(sizeof(t_process)));
	p->next = NULL;
	p->xpath = strdup(xpath);
	p->argv = tabdup(argv);
	p->pid = 0;
	p->completed = false;
	p->stopped = false;
	p->status = 0;
	return (p);
}

/*
** This function add a new process to the specified job.
**
** @return          The added process.
*/

t_process			*add_process_to_job(t_job *j, char *xpath, char **argv)
{
	t_process	*p;

	if (!j->first_process)
		return (j->first_process = init_process(xpath, argv));
	p = j->first_process;
	while (p->next)
		p = p->next;
	return (p->next = init_process(xpath, argv));
}
