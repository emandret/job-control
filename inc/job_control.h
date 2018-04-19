/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emandret <emandret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 02:21:15 by emandret          #+#    #+#             */
/*   Updated: 2018/04/20 00:45:15 by emandret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_CONTROL_H
# define JOB_CONTROL_H

# include "data_struct.h"

/*
** shell.c
*/
void			init_shell(void);
void			switch_signal_handlers(void);

/*
** job_init.c
*/
unsigned int	get_job_list_size(void);
t_job			*create_job(const char *name);
void			free_job(t_job *j);

/*
** job_util.c
*/
t_job			*find_job_by_pgid(pid_t pgid);
t_job			*find_job_by_id(unsigned int id);
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
void			format_job_info(int id, pid_t pid, const char *status,
				const char *name);
void			do_job_notification(void);

/*
** job_launch.c
*/
void			launch_job(t_job *j, bool foreground);
void			continue_job(t_job *j, bool foreground);

/*
** process_util.c
*/
pid_t			set_process_pgid(pid_t pid, pid_t pgid);
void			set_channel(int fildes, int fildes2);
void			close_channel(int fildes, int fildes2);
bool			check_process_state(t_process *p, t_state st);
void			mark_process_state(t_process *p, t_state st);

/*
** process_util2.c
*/
bool			is_forkable(t_process *p);
t_process		*get_last_process(t_job *j);

/*
** process_init.c
*/
t_process		*add_process(t_job *j, const char *xpath, t_builtin builtin,
				char *const *argv);

/*
** process_status.c
*/
void			check_for_status(void);
void			wait_for_job(t_job *j);

/*
** process_exec.c
*/
void			launch_job_processes(t_job *j, bool foreground);

/*
** builtin_ex.c
*/
void			builtin_ex(char *const *argv);

#endif
