/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 02:10:00 by emandret          #+#    #+#             */
/*   Updated: 2018/04/18 02:18:23 by emandret         ###   ########.fr       */
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

# define READ_END 0
# define WRITE_END 1

/*
** Represent a built-in type
*/
typedef void			(*t_builtin)(char *const *argv);

/*
** The three main state for a process.
**
** @ST_RUNNING          The process is running.
** @ST_STOPPED          The process is stopped.
** @ST_COMPLETED        The process is completed.
*/
typedef enum			e_state
{
	ST_RUNNING,
	ST_STOPPED,
	ST_COMPLETED
}						t_state;

/*
** The three standard file descriptors.
**
** @in                  The stdin.
** @out                 The stdout.
** @err                 The stderr.
** @fd3                 The additional file descriptors according to POSIX.
*/
typedef struct			s_std
{
	int					in;
	int					out;
	int					err;
	int					fd3;
	int					fd4;
	int					fd5;
	int					fd6;
	int					fd7;
	int					fd8;
	int					fd9;
}						t_std;

/*
** A process is a single process:
**
** @next                The next process in the list.
** @xpath               The executable path.
** @builtin             The built-in function pointer.
** @argv                The arguments.
** @pid                 The process ID pid of the process.
** @completed           True if the process has completed.
** @stopped             True if the process is stopped.
** @status              The status code returned by waitpid(2).
** @std                 The three standard file descriptors.
*/
typedef struct			s_process
{
	struct s_process	*next;
	const char			*xpath;
	t_builtin			builtin;
	char *const			*argv;
	pid_t				pid;
	bool				completed;
	bool				stopped;
	int					status;
	t_std				std;
}						t_process;

/*
** A job is a pipeline of processes:
**
** @next                The next job in the list.
** @name                The name.
** @first_process       The head of the process list.
** @id                  The identifier.
** @pgid                The process group ID pgid of the job.
** @notified            True if the user has been notified for the job.
** @tmodes              The terminal attributes for a job.
** @std                 The three standard file descriptors.
*/
typedef struct			s_job
{
	struct s_job		*next;
	const char			*name;
	t_process			*first_process;
	unsigned int		id;
	pid_t				pgid;
	bool				launched;
	bool				notified;
	struct termios		tmodes;
	t_std				std;
}						t_job;

/*
** Keep track of attributes of the shell.
**
** @pgid				The process group ID pgid for the shell.
** @tmodes				Store the shell terminal attributes.
** @terminal_fd			The file descriptors of the tty input.
** @is_interactive		True if the shell reads from user input from a tty.
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
extern char *const		*g_envp;

/*
** The active jobs are linked into a list. This is its head.
*/
extern t_job			*g_first_job;

/*
** The shell attributes are required everywhere.
*/
extern t_shell			g_shell;

#endif
