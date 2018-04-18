/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 19:40:19 by emandret          #+#    #+#             */
/*   Updated: 2018/04/18 05:53:41 by emandret         ###   ########.fr       */
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
	if (!(t = malloc(sizeof(char*) * i)))
		return (NULL);
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

static t_process	*init_process(const char *xpath, t_builtin builtin,
					char *const *argv)
{
	t_process	*p;

	if (!(p = malloc(sizeof(t_process))))
		return (NULL);
	p->next = NULL;
	if (xpath)
		p->xpath = strdup(xpath);
	p->builtin = builtin;
	p->argv = init_argv(argv);
	p->pid = 0;
	p->completed = false;
	p->stopped = false;
	p->status = 0;
	p->std.in = STDIN_FILENO;
	p->std.out = STDOUT_FILENO;
	p->std.err = STDERR_FILENO;
	p->std.fd3 = -1;
	p->std.fd4 = -1;
	p->std.fd5 = -1;
	p->std.fd6 = -1;
	p->std.fd7 = -1;
	p->std.fd8 = -1;
	p->std.fd9 = -1;
	return (p);
}

/*
** This function add a new process to the specified job.
**
** @return The added process.
*/

t_process			*add_process(t_job *j, const char *xpath,
					t_builtin builtin, char *const *argv)
{
	t_process	*p;

	if (!j->first_process)
		return (j->first_process = init_process(xpath, builtin, argv));
	p = j->first_process;
	while (p->next)
		p = p->next;
	return (p->next = init_process(xpath, builtin, argv));
}
