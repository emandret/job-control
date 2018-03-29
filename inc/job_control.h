/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 02:21:15 by emandret          #+#    #+#             */
/*   Updated: 2018/03/29 04:11:14 by emandret         ###   ########.fr       */
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
** The format buffer size.
*/
# define FORMAT_BUF 1024

/*
** shell.c
*/
void			init_shell(void);

/*
** job_init.c
*/
unsigned int	job_list_size(void);
t_job			*add_job_to_list(int stdin, int stdout, int stderr);
void			free_job(t_job *j);

/*
** job_util.c
*/
t_job			*find_job(pid_t pgid);
bool			check_job_state(t_job *j, t_state st);
void			mark_job_state(t_job *j, t_state st);

/*
** job_schedule.c
*/
void			put_job_in_foreground(t_job *j, bool cont);
void			put_job_in_background(t_job *j, bool cont);

/*
** job_notify.c
*/
void			do_job_notification(void);

/*
** job_launch.c
*/
void			launch_job(t_job *j, bool foreground);
void			continue_job(t_job *j, bool foreground);

/*
** process_util.c
*/
bool			check_process_state(t_process *p, t_state st);
void			mark_process_state(t_process *p, t_state st);

/*
** process_init.c
*/
t_process		*add_process_to_job(t_job *j, char *xpath, char **argv);

/*
** process_status.c
*/
void			check_for_status(void);
void			wait_for_job(t_job *j);

/*
** process_exec.c
*/
void			launch_job_processes(t_job *j, bool foreground);

#endif
