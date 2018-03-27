/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 17:49:22 by emandret          #+#    #+#             */
/*   Updated: 2018/03/27 15:07:26 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** Set the job list head to NULL at the beginning.
*/

t_job			*g_first_job = NULL;

/*
** Create a new job. Not intended to be used outside this scope.
**
** @return      The created job.
*/

static t_job	*init_job(int fd[3])
{
	t_job	*j;

	ASSERT_MALLOC(j = malloc(sizeof(t_job)));
	tcgetattr(g_shell.terminal_fd, &j->tmodes);
	j->next = NULL;
	j->first_process = NULL;
	j->id = job_list_size();
	j->pgid = 0;
	j->notified = false;
	j->std.in = fd[0];
	j->std.out = fd[1];
	j->std.err = fd[2];
	return (j);
}

/*
** The number of jobs in the list
**
** @return      The size.
*/

unsigned int	job_list_size(void)
{
	unsigned int	i;
	t_job			*j;

	i = 0;
	j = g_first_job;
	while (j)
	{
		i++;
		j = j->next;
	}
	return (i);
}

/*
** The jobs - which are pipelines of processes - are stored in
** a linked list, this function add a new job to this list.
**
** @return      The added job.
*/

t_job			*add_job_to_list(int stdin, int stdout, int stderr)
{
	t_job	*j;

	if (!g_first_job)
		return (g_first_job = init_job((int []){stdin, stdout, stderr}));
	j = g_first_job;
	while (j->next)
		j = j->next;
	return (j->next = init_job((int []){stdin, stdout, stderr}));
}

/*
** Free the job and its associated processes.
*/

void			free_job(t_job *j)
{
	t_process	*p;
	t_process	*pnext;

	if ((p = j->first_process))
		while (p)
		{
			pnext = p->next;
			free(p);
			p = pnext;
		}
	free(j);
}
