/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 22:48:43 by emandret          #+#    #+#             */
/*   Updated: 2018/03/27 00:12:48 by emandret         ###   ########.fr       */
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

static t_job	*new_job(int fd[3])
{
	struct termios	tmodes;
	t_job			*j;
	char			*argv1[5] = {"/bin/ls", "-l", NULL};
	char			*argv2[5] = {"/bin/cat", "-e", NULL};

	tcgetattr(g_shell.terminal_fd, &tmodes);
	j = add_job_to_list("ls -l | cat -e", tmodes, fd);
	add_process_to_job(j, tabdup(argv1));
	add_process_to_job(j, tabdup(argv2));
	return (j);
}

static void		print_job(t_job *j)
{
	t_process	*p;
	int			i;

	printf("************ JOB ************\n");
	printf("next job      = %p\n", j->next);
	printf("command       = %s\n", j->command);
	printf("first process = %p\n", j->first_process);
	printf("job pgid      = %d\n", j->pgid);
	printf("notified      = %d\n", j->notified);
	printf("stdin         = %d\n", j->stdin);
	printf("stdout        = %d\n", j->stdout);
	printf("stderr        = %d\n", j->stderr);
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
	j = new_job((int [3]){STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO});
	print_job(j);
	launch_job(j, true);
	print_job(j);
	return (0);
}
