/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 02:10:00 by emandret          #+#    #+#             */
/*   Updated: 2018/03/26 23:28:42 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_STRUCT_H
# define DATA_STRUCT_H

# include <sys/types.h>
# include <stdbool.h>
# include <termios.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <errno.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define READ_END 0
# define WRITE_END 1

/*
** A process is a single process.
*/
typedef struct			s_process
{
	struct s_process	*next;
	char				**argv;
	pid_t				pid;
	bool				completed;
	bool				stopped;
	int					status;
}						t_process;

/*
** A job is a pipeline of processes.
*/
typedef struct			s_job
{
	struct s_job		*next;
	char				*command;
	t_process			*first_process;
	pid_t				pgid;
	bool				notified;
	struct termios		tmodes;
	int					stdin;
	int					stdout;
	int					stderr;
}						t_job;

/*
** Keep track of attributes of the shell.
*/
typedef struct			s_shell
{
	pid_t				pgid;
	struct termios		tmodes;
	int					terminal_fd;
	bool				is_interactive;
}						t_shell;

/*
** The declaration for the environment global variable
*/
extern char				**g_envp;

/*
** The active jobs are linked into a list. This is its head.
*/
extern t_job			*g_first_job;

/*
** The shell attributes are required everywhere.
*/
extern t_shell			g_shell;

#endif
