/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 22:48:43 by emandret          #+#    #+#             */
/*   Updated: 2018/03/28 21:02:04 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

char			**g_envp = NULL;

static void		print_job(t_job *j)
{
	t_process	*p;
	int			i;

	printf("next job      : %p\n", j->next);
	printf("first process : %p\n", j->first_process);
	printf("job id        : %d\n", j->id);
	printf("job pgid      : %d\n", j->pgid);
	printf("notified      : %d\n", j->notified);
	printf("stdin         : %d\n", j->std.in);
	printf("stdout        : %d\n", j->std.out);
	printf("stderr        : %d\n", j->std.err);
	p = j->first_process;
	i = 0;
	while (p)
	{
		printf(" + [%d] (process %jd) %s\n", i++, (intmax_t)p->pid, p->argv[0]);
		printf("  - completed : %d\n", p->completed);
		printf("  - stopped   : %d\n", p->stopped);
		printf("  - status    : %d\n", p->status);
		p = p->next;
	}
}

int				main(void)
{
	t_job			*j;

	init_shell();
	j = add_job_to_list(STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
	add_process_to_job(j, "/bin/ls", (char *[]){"ls", "-l", NULL});
	add_process_to_job(j, "/bin/cat", (char *[]){"cat", "-e", NULL});
	launch_job(j, true);
	do_job_notification();
	print_job(j);
	return (0);
}
