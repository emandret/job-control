/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 22:48:43 by emandret          #+#    #+#             */
/*   Updated: 2018/05/01 21:25:15 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

char *const		*g_envp = NULL;

static void		print_job(t_job *j)
{
	t_process	*p;
	int			i;

	printf("next job      : %p\n", j->next);
	printf("first process : %p\n", j->first_process);
	printf("job id        : %d\n", j->id);
	printf("job pgid      : %d\n", j->pgid);
	printf("notified      : %d\n", j->notified);
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
	t_job			*i;
	// t_job			*j;

	init_shell();
	i = create_job("builtin_ex | ls -Rl /");
	// j = create_job("builtin example");
	add_process(i, "/bin/ls", NULL, (char *const[]){"ls", "-R", "/", NULL});
	add_process(i, NULL, &builtin_ex, (char *const[]){"builtin", NULL});
	// add_process(i, "/bin/cat", NULL, (char *const[]){"cat", "-e", NULL});
	launch_job(i, true);
	do_job_notification();
	// launch_job(j, true);
	// do_job_notification();
	print_job(i);
	// print_job(j);
	// sleep(10);
	return (0);
}
