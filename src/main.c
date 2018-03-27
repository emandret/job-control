/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 22:48:43 by emandret          #+#    #+#             */
/*   Updated: 2018/03/27 18:10:33 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

char			**g_envp = NULL;

static int		size(char **tab)
{
	unsigned int	i;

	i = 0;
	while (tab[i++])
		;
	return (++i);
}

static char		**tabdup(char **tab)
{
	unsigned int	i;
	char			**new;

	if (!(new = (char**)malloc(sizeof(char*) * size(tab))))
		return (NULL);
	i = 0;
	while (tab[i])
	{
		new[i] = strdup(tab[i]);
		i++;
	}
	new[i] = NULL;
	return (new);
}

static t_job	*new_job(void)
{
	struct termios	tmodes;
	t_job			*j;
	// char			*argv1[5] = {"/bin/ls", "-l", NULL};
	char			*argv2[5] = {"/bin/cat", "-e", NULL};

	tcgetattr(g_shell.terminal_fd, &tmodes);
	j = add_job_to_list(STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
	// add_process_to_job(j, tabdup(argv1));
	add_process_to_job(j, tabdup(argv2));
	return (j);
}

static void		print_job(t_job *j)
{
	t_process	*p;
	int			i;

	printf("************ JOB ************\n");
	printf("next job      = %p\n", j->next);
	printf("first process = %p\n", j->first_process);
	printf("job id        = %d\n", j->id);
	printf("job pgid      = %d\n", j->pgid);
	printf("notified      = %d\n", j->notified);
	printf("stdin         = %d\n", j->std.in);
	printf("stdout        = %d\n", j->std.out);
	printf("stderr        = %d\n", j->std.err);
	p = j->first_process;
	i = 0;
	while (p)
	{
		printf("  PROCESS %d: %s\n", i++, p->argv[0]);
		printf("   - completed = %d\n", p->completed);
		printf("   - stopped   = %d\n", p->stopped);
		printf("   - status    = %d\n", p->status);
		p = p->next;
	}
	printf("********** END JOB **********\n");
}

int				main(void)
{
	t_job	*j;

	init_shell();
	j = new_job();
	print_job(j);
	launch_job(j, true);
	do_job_notification();
	print_job(j);
	return (0);
}
