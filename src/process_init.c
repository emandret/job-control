/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:40:19 by emandret          #+#    #+#             */
/*   Updated: 2018/03/31 02:53:19 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Duplicate and allocate the argv array.
**
** @return The duplicated array.
*/

static char *const	*init_argv(char *const *argv)
{
	unsigned int	i;
	char			**t;

	i = 1;
	while (argv[i++])
		;
	ASSERT_MALLOC(t = malloc(sizeof(char*) * i));
	i = 0;
	while (argv[i])
	{
		t[i] = strdup(argv[i]);
		i++;
	}
	t[i] = NULL;
	return (t);
}

/*
** Create a new process. Not intended to be used outside this scope.
**
** @return The created process.
*/

static t_process	*init_process(const char *xpath, char *const *argv)
{
	t_process	*p;

	ASSERT_MALLOC(p = malloc(sizeof(t_process)));
	p->next = NULL;
	p->xpath = strdup(xpath);
	p->argv = init_argv(argv);
	p->pid = 0;
	p->completed = false;
	p->stopped = false;
	p->status = 0;
	return (p);
}

/*
** This function add a new process to the specified job.
**
** @return The added process.
*/

t_process			*add_process(t_job *j, const char *xpath, char *const *argv)
{
	t_process	*p;

	if (!j->first_process)
		return (j->first_process = init_process(xpath, argv));
	p = j->first_process;
	while (p->next)
		p = p->next;
	return (p->next = init_process(xpath, argv));
}
