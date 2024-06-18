#include "shell.h"

int	error_code(int ext)
{
	if (WIFSIGNALED(ext))
		return (128 + WTERMSIG(ext));
	else if (WIFEXITED(ext))
		return (WEXITSTATUS(ext));
	else
		return (ext);
}

char	*rltv_pth(char *pwd, char *cs)
{
	char	*rpth;

	rpth = sjoin(pwd, cs);
    if (!rpth)
        return (0);
	if (access(rpth, F_OK | X_OK) == 0)
		return (rpth);
	free(rpth);
	return (0);
}

char	*get_pth(char *pwd, char **evpth, char *cs)
{
	char	*right_path;
	char	*cmd;
	int		i;

	right_path = 0;
	if (access(cs, F_OK | X_OK) == 0)
		return (sdup(cs));
	if (cs[0] == '.')
		return (rltv_pth(pwd, cs));
	cmd = sjoin("/", cs);
    if (!cmd)
        return (0);
	i = -1;
	while (evpth && evpth[++i])
	{
		right_path = sjoin(evpth[i], cmd);
        if (!right_path)
            return (0);
		if (access(right_path, F_OK | X_OK) == 0)
			break ;
		free(right_path);
		right_path = 0;
	}
	free(cmd);
	return (right_path);
}

int fork_exe(t_sh *sh, char **cs)
{
    char	*pth;
	pid_t	m_pid;
	int		ext;

	m_pid = fork();
	if (m_pid == 0)
	{
		pth = get_pth(sh->pwd, sh->evpth, cs[0]);
		if (!pth)
		{
			if (cs[0][0] == '.' || cs[0][0] == '/')
				fprint(2, "midsh: %s: No such file or directory\n", cs[0]);
			else
				fprint(2, "midsh: %s: command not found\n", cs[0]);
			exit(127);
		}
		execve(pth, cs, sh->env);
        free(pth);
		free_sh(sh, 0);
		perror("midsh");
		free2(cs);
		exit(126);
	}
	waitpid(m_pid, &ext, 0);
	return(error_code(ext));
}

int run_exe(t_sh *sh, char **cs)
{
    // (void)sh;
    // (void)cs;
	char *pth = get_pth(sh->pwd, sh->evpth, cs[0]);
	if (!pth)
	{
		if (cs[0][0] == '.' || cs[0][0] == '/')
			fprint(2, "midsh: %s: No such file or directory\n", cs[0]);
		else
			fprint(2, "midsh: %s: command not found\n", cs[0]);
		exit(127);
	}
	execve(pth, cs, sh->env);
	free(pth);
	free_sh(sh, 0);
	perror("midsh");
	free2(cs);
	exit(126);
    return 0;
}

int run_one(t_sh *sh, char **cs, int fork)
{
    sde_trans2(cs);
    if (!cs[0])
        return (sh->exit_c);
	if (sncmp(cs[0], "cd", 3) == 0)
		run_cd(sh, cs);
	else if (sncmp(cs[0], "echo", 5) == 0)
		run_echo(sh, cs);
	else if (sncmp(cs[0], "pwd", 4) == 0)
		run_pwd(sh, cs);
	else if (sncmp(cs[0], "export", 7) == 0)
		run_export(sh, cs);
	else if (sncmp(cs[0], "unset", 6) == 0)
		run_unset(sh, cs);
	else if (sncmp(cs[0], "env", 4) == 0)
		run_env(sh, cs);
	else if (sncmp(cs[0], "exit", 5) == 0)
		run_exit(sh, cs);
	else
    {
        if (fork)
		    run_exe(sh, cs);
        else
            sh->exit_c = fork_exe(sh, cs);
    }
    if (fork)
        exit(sh->exit_c);
	return(sh->exit_c);
}

int exe_one(t_sh *sh, char *cmd, int fork)
{

	(void)fork;
    // printf("--- one: %s\n",cmd);
    char **cmds = split(cmd, "|");
    // int i = -1;
    if (cmds[1] == 0)
    {
		if (trm_prth(cmds[0]))
		{
			// printf("()\n");
            sh->exit_c = exe_all(sh, sdup(cmds[0]), 0);
		}
        else
		{
			cmds[0] = load_var(sh, cmds[0]);
			cmds[0] = repls_wikd(sh, cmds[0]);
			cmds[0] = load_wikd(sh, cmds[0]);
			// printf("dub\n");
			char **cmdss = split(cmds[0], RSS);
			sh->exit_c = run_one(sh, cmdss, 0);
			free2(cmdss);
		}
    }
    else
    {
    	// printf("--- pip: %s\n",cmd);
        sh->exit_c = exe_pip(sh, cmds);
    }
    free2(cmds);
    free(cmd);
    return (sh->exit_c);
}
