#include "shell.h"

void close_pip(int *pip, int len)
{
    int i;
    int n;

    i = 0;
    n = 2 * len - 2;
    while (i < n)
    {
        close(pip[i++]);
    }
}

void one_fork(t_sh *sh, char **cs)
{
    sh->pid[sh->i] = fork();
    if (sh->pid[sh->i] == -1)
        free_sh(sh, 2);
    else if (sh->pid[sh->i] == 0)
    {
        set_signal_exe();
        if (sh->i > 0)
        {
            if (dup2(sh->pip[2 * (sh->i - 1)], 0) == -1)
                free_sh(sh, 2);
        }
        if (sh->i < sh->len - 1)
        {
            if (dup2(sh->pip[2 * sh->i + 1], 1) == -1)
                free_sh(sh, 2);
        }
        close_pip(sh->pip, sh->len);
        exe_all(sh, sdup(cs[sh->i]), 1);
        free_sh(sh, 0);
        free2(cs);
        exit(126);
    }
}

void crat_pip(t_sh *sh)
{
    sh->pid = malloc(sizeof(int) * sh->len);
    if (!sh->pid)
        free_sh(sh, 2);
    sh->pip = malloc(sizeof(int) * sh->len * 2);
    if (!sh->pip)
        free_sh(sh, 2);
    sh->i = 0;
    while (sh->i < sh->len - 1)
    {
        if (pipe(&sh->pip[2 * sh->i])== -1)
            free_sh(sh, 2);
        (sh->i)++;
    }
}

void fre_pip(t_sh *sh)
{
    free(sh->pid);
    free(sh->pip);
    sh->pid = 0;
    sh->pip = 0;
}

int exe_pip(t_sh *sh, char **cs)
{
    int ext;

    sh->len = len2(cs);
    crat_pip(sh);
    sh->i = -1;
    while (cs[++(sh->i)])
    {
        // ext = 0;
        // while (ext < 1000)
        //     ext++;
        usleep(100);
	    one_fork(sh, cs);
    }
    close_pip(sh->pip, sh->len);
    sh->i = -1;
	while (++(sh->i) < sh->len)
		waitpid(sh->pid[sh->i], &ext, 0);
	sh->exit_c = error_code(ext);
    fre_pip(sh);
    return (sh->exit_c);
}
