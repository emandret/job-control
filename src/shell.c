/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 07:41:24 by emandret          #+#    #+#             */
/*   Updated: 2018/04/18 03:13:16 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "job_control.h"

/*
** The definition of the global attributes is here.
*/

t_shell	g_shell;

/*
** 1. Keep track of attributes of the shell.
**
** 2. Check if the shell is running interactively as the foreground job and loop
**    until we are in the foreground.
**
** 3. Ignore interactive and job-control signals and put ourselves in our own
**    process group then grab control of the terminal.
**
** 4. Save the default attributes for the shell.
*/

void	init_shell(void)
{
	g_shell.terminal_fd = STDIN_FILENO;
	if ((g_shell.is_interactive = isatty(g_shell.terminal_fd)))
	{
		while (tcgetpgrp(g_shell.terminal_fd) != (g_shell.pgid = getpgrp()))
			kill(-g_shell.pgid, SIGTTIN);
		switch_signal_handlers();
		g_shell.pgid = getpid();
		if (setpgid(g_shell.pgid, g_shell.pgid) == -1)
		{
			perror("Couldn't put the shell in its own process group");
			exit(EXIT_FAILURE);
		}
		tcsetpgrp(g_shell.terminal_fd, g_shell.pgid);
		tcgetattr(g_shell.terminal_fd, &g_shell.tmodes);
	}
}

void	switch_signal_handlers(void)
{
	static bool	on;

	if (!on)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		on = true;
	}
	else
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		on = false;
	}
}
