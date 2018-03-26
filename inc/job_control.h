/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 02:21:15 by emandret          #+#    #+#             */
/*   Updated: 2018/03/26 22:55:04 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_CONTROL_H
# define JOB_CONTROL_H

# include "data_struct.h"

/*
** Assert that mallocs are protected.
*/
# define ASSERT_MALLOC(x) if (!(x)) return (NULL);

/*
** shell.c
*/
void		init_shell(void);

/*
** job_init.c
*/
t_job		*add_job_to_list(char *command, struct termios tmodes, int fd[3]);
void		free_job(t_job *j);

/*
** job_util.c
*/
t_job		*find_job(pid_t pgid);
bool		job_is_stopped(t_job *j);
bool		job_is_completed(t_job *j);

/*
** job_schedule.c
*/
void		put_job_in_foreground(t_job *j, bool cont);
void		put_job_in_background(t_job *j, bool cont);
void		mark_job_as_running(t_job *j);
void		mark_job_as_stopped(t_job *j);

/*
** job_notify.c
*/
void		do_job_notification(void);

/*
** job_launch.c
*/
void		launch_job(t_job *j, bool foreground);
void		continue_job(t_job *j, bool foreground);

/*
** process_init.c
*/
t_process	*add_process_to_job(t_job *j, char **argv);

/*
** process_status.c
*/
bool		mark_process_status(pid_t pid, int status);
void		check_for_status(void);
void		wait_for_job(t_job *j);

/*
** process_exec.c
*/
void		launch_job_processes(t_job *j, int fd[3], bool foreground);

#endif
