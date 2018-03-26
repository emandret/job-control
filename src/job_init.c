/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 17:49:22 by emandret          #+#    #+#             */
/*   Updated: 2018/03/26 23:30:24 by emandret         ###   ########.fr       */
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
** @return  The created job.
*/

static t_job	*init_job(char *command, struct termios tmodes, int fd[3])
{
	t_job	*j;

	ASSERT_MALLOC(j = malloc(sizeof(t_job)));
	j->next = NULL;
	j->command = command;
	j->first_process = NULL;
	j->pgid = 0;
	j->notified = false;
	j->tmodes = tmodes;
	j->stdin = fd[STDIN];
	j->stdout = fd[STDOUT];
	j->stderr = fd[STDERR];
	return (j);
}

/*
** The jobs - which are pipelines of processes - are stored in
** a linked list, this function add a new job to this list.
**
** @return  The added job.
*/

t_job			*add_job_to_list(char *command, struct termios tmodes,
								int fd[3])
{
	t_job	*j;

	if (!g_first_job)
		return (g_first_job = init_job(command, tmodes, fd));
	j = g_first_job;
	while (j->next)
		j = j->next;
	return (j->next = init_job(command, tmodes, fd));
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
