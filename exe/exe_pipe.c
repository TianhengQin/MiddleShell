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

int exe_pip(t_sh *sh, char **cs)
{

    // fprint(1, "run pip %s, %s\n", cs[0], cs[1]);

    int i;
    int len = len2(cs);
    // fprint(1, "run pip len %d\n", len);
    int pip[FD_LIMIT];
    int pid[PS_LIMIT];
    int ext;

    // char *a[3];
    i = 0;
    while (i < len - 1)
    {
        if (pipe(&pip[2 * i])== -1)
        {
            perror("pipe");
        }
        i++;
    }
    i = -1;
    while (cs[++i])
    {
	    pid[i] = fork();  
        if (pid[i] == -1)
            fprint(2, "Error creating process\n");
        else if (pid[i] == 0)
        {
            // write(2, "fork\n", 5);
            if (i > 0)
            {
                dup2(pip[2 * (i - 1)], 0);
            }
            if (i < len - 1)
            {
                dup2(pip[2 * i + 1], 1);
            }
            close_pip(pip, len);
            exe_all(sh, sdup(cs[i]), 1);
            exit(126);
        }
    }
    close_pip(pip, len);
    i = -1;
	while (++i < len)
	{
		waitpid(pid[i], &ext, 0);
        // perror("waitpid");
        // fprint(2, "wait %d\n", pidf);
	}
	sh->exit_c = error_code(ext);
    // dup2(sh->stdi, 0);
	// dup2(sh->stdo, 1);
    // fprint(2, "wait all\n");
    return (sh->exit_c);
}

// void	one_fork(t_sh *sh, char **cs, int i, int len)
// {
// 	int	pid;

// 	pid = fork();
// 	if (pid == -1)
// 		fprint(2, "Error creating process\n");
// 	else if (pid == 0)
// 	{
// 		set_signal_exe();
// 		close_pip(sh, i);
// 		if (i > 0)
// 		{
// 			close(sh->pip[2 * (i - 1) + 1]);
// 			dup2(sh->pip[2 * (i - 1)], 0);
// 			close(sh->pip[2 * (i - 1)]);
// 		}
// 		if (i < len - 1)
// 		{
// 			close(sh->pip[2 * i]);
// 			dup2(sh->pip[2 * i + 1], 1);
// 			close(sh->pip[2 * i + 1]);
// 		}
// 		exe_one(sh, cs, i);
// 	}
// 	else
// 		sh->pidf = pid;
// }


// void	exe_pip(t_sh *sh, char ***css)
// {
// 	int	i;
// 	int	ext;
// 	int	pidf;

// 	sh->exit_c = 0;
// 	i = 0;
// 	crt_hirdoc(sh);
// 	sh->stdi = dup(0);
// 	sh->stdo = dup(1);
// 	set_pip(sh);
// 	while (css[i])
// 	{
// 		one_fork(sh, css[i], i, len3(css));
// 		i++;
// 	}
// 	close_pip(sh, -1);
// 	pidf = 0;
// 	while (pidf != -1)
// 	{
// 		pidf = waitpid(-1, &ext, 0);
// 		ext = error_code(ext);
// 		if (pidf == sh->pidf)
// 			sh->exit_c = ext;
// 	}
// 	reset_io(sh);
// }
