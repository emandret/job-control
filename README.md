# job-control
##

# Struct Job
- t_job structure :
```C
typedef struct            s_job
{
    struct s_job        *next;
    const char            *name;
    t_process            *first_process;
    unsigned int        id;
    pid_t                pgid;
    bool                launched;
    bool                notified;
    struct termios        tmodes;
    t_std                std;
}                        t_job;
```
- t_process structure :
```C
typedef struct            s_process
{
    struct s_process    *next;
    const char            *xpath;
    char *const            *argv;
    pid_t                pid;
    bool                completed;
    bool                stopped;
    int                    status;
    t_std                std;
}                        t_process;
```
- t_std structure :
```C
typedef struct            s_std
{
    int                    in;
    int                    out;
    int                    err;
}                        t_std;
```
# Usage

```C
t_job            *create_job(const char *name, int stdin, int stdout, int stderr);
```
> the function `create_job` create an instance for the `job-control`
> - `name` is the name of the created instance (the name of the command liked to it for exemple)
> - `stdin` the `file_des` of the `input` for the instance
> - `stdout` the `file_des` of the `ouput` for the instance
> - `stderr` the `file_des` of the `error ouput` for the instance

```C
t_process        *add_process(t_job *j, const char *xpath, char *const *argv);
```
> add a process to the instance `j`, if there is already a process in the job `j` there will be piped
> - `j` is the instance of the process to be linked
> - `xpath` is the path to the binary
> - `argv` is the argument given to the process added
```C
void			launch_job(t_job *j, bool foreground);
```
> lauch the job `j`, in `background` if bool `foreground` is `false`
> - `j` the job to be launch
> - `forebround` if the job have to be lauch in `foreground`
```C
void			do_job_notification(void);
```
> notify the state of all previous lauched `job`
