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

int	is_pth(char *cs)
{
	int i = 0;

	while (cs[i])
	{
		if (cs[i] == '/')
			return 1;
		i++;
	}
	return 0;
}

char	*get_pth(char *pwd, char **evpth, char *cs)
{
	char	*right_path;
	char	*cmd;
	int		i;

	right_path = 0;
	if (access(cs, F_OK) == 0 && is_pth(cs))
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
		if (access(right_path, F_OK) == 0)
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

int openfile(t_sh *sh, int tokn, char *file)
{
	(void)sh;
	char *fnam = sdupr(file);
	// fprint(2, "file name %s\n", fnam);
	fnam = load_var(sh, fnam);
	fnam = repls_wikd(sh, fnam);
	fnam = load_wikd(sh, fnam);
	sde_trans(fnam);
	char **fnams = ft_split(fnam, RSS);
	if (fnams[1] || !fnams[0])
	{
		fprint(2, "midsh: ambiguous redirect\n");
		free2(fnams);
		free(fnam);
		return (1);
	}
	free(fnam);
	fnam = sdup(fnams[0]);
	free2(fnams);
	// fprint(2, "file name %s\n", fnam);
	int fd = 0;
	if (tokn == 3)
		fd = open(fnam, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (tokn == 4)
		fd = open(fnam, O_RDONLY);
	else if (tokn == 8)
		fd = open(fnam, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
	{
		free(fnam);
		perror("midsh");
		return (1);
	}
	if (tokn == 3 || tokn == 8)
	{
		dup2(fd, 1);
		close(fd);
	}
	else
	{
		dup2(fd, 0);
		close(fd);
	}
	free(fnam);
	return (0);
}

int redir(t_sh *sh, char *cmd)
{
	int i = 0;
	int tokn = 0;
	int j = 0;
	while (cmd[i])
	{
		// fprint(2, "idx %d\n", i);
		if (cmd[i] == '(')
		{
			while (cmd[i] != ')')
				i++;
			continue;
		}
		if ((i == 0 || cmd[i - 1] == RS) && cmd[i] != RS)
		{
			tokn = is_tokn(&cmd[i]);
			// fprint(2, "is %d\n", tokn);
			if (tokn == 3 || tokn == 4 || tokn > 7)
			{
				j = 0;
				while (cmd[i + j] != RS)
					j++;
				while (cmd[i + j] == RS)
					j++;
				if (openfile(sh, tokn, &cmd[i + j]))
					return 1;
				while (j)
				{
					cmd[i] = RS;
					i++;
					j--;
				}
				while (cmd[i] != RS)
					cmd[i++] = RS;
			}
			else
			{
				// fprint(2, "not tokn %d\n", i);

				i++;
				// fprint(2, "not tokn %d\n", i);

			}
		}
		else
			i++;
	}
	return (0);
}

void dup_io(t_sh *sh, char *cmd)
{
    sh->stdi = dup(0);
    if (sh->stdi == -1)
    {
        free(cmd);
        free_sh(sh, 2);
    }
    sh->stdo = dup(1);
    if (sh->stdo == -1)
    {
        close(sh->stdi);
        free(cmd);
        free_sh(sh, 2);
    }
}

void re_io(t_sh *sh)
{
    if (dup2(sh->stdi, 0) == -1)
    {
        // fprint(2, "fd %d\n", sh->stdi);
        free_sh(sh, 2);
    }
	if (dup2(sh->stdo, 1) == -1)
        free_sh(sh, 2);
    close(sh->stdi);
    close(sh->stdo);
}

int exe_one(t_sh *sh, char *cmd, int fork)
{
	(void)fork;
	if (check_malloc(sh, cmd, 0, 0) == -1)
		return (12);
    char **cmds = split(cmd, "|");
	if (!cmds)
	{
		free(cmd);
		return (12);
	}
    if (cmds[1] == 0)
    {
		if (trm_prth(cmds[0]))
            sh->exit_c = exe_all(sh, sdup(cmds[0]), 0);
        else
		{
			dup_io(sh, cmd);
			if (redir(sh, cmds[0]))
			{
				free(cmd);
				free2(cmds);
				re_io(sh);
				return (1);
			}
			cmds[0] = load_var(sh, cmds[0]);
			cmds[0] = repls_wikd(sh, cmds[0]);
			cmds[0] = load_wikd(sh, cmds[0]);
			char **cmdss = split(cmds[0], RSS);
			if (!cmdss)
				return (12);
			sh->exit_c = run_one(sh, cmdss, 0);
			free2(cmdss);
			re_io(sh);
		}
    }
    else
        sh->exit_c = exe_pip(sh, cmds);
    free2(cmds);
    free(cmd);
    return (sh->exit_c);
}
