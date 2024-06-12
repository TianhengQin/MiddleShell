
#include "shell.h"

int trm_prth(char *s)
{
    int		i;
    int     j;
    int     re;

    re = 0;
	i = len(s) - 1;
	while (i >= 0 && s[i] == RS)
		i--;
	j = 0;
	while (s[j] && s[j] == RS)
		j++;
    while (s[i] == ')' && s[j] == '(' && i > j)
    {
        re = 1;
        s[i--] = RS;
        s[j++] = RS;
        while (i >= 0 && s[i] == RS)
		    i--;
        while (s[j] && s[j] == RS)
		    j++;
    }
    return (re);
}

int run_one(t_sh *sh, char **cs)
{
    sde_trans2(cs);
    // printf("runone\n");
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
        sh->exit_c = 0;
		printf("run fork %s\n", cs[0]);
    }
	return(sh->exit_c);
}

int exe_one(t_sh *sh, char *cmd) {

    // printf("--- one: %s\n",cmd);
    char **cmds = split(cmd, "|");
    int i = -1;
    if (cmds[1] == 0)
    {
        char **cmdss = split(cmds[0], RSS);
        sh->exit_c = run_one(sh, cmdss);
        // if (!cmdss[0])
        // {
        //     printf("[]");
        // }
        // while (cmdss[++i])
        // {
        //     printf("[%s]", cmdss[i]);
        // }
        // printf("\n");
        free2(cmdss);
    }
    else
    {
        while (cmds[++i])
        {
            exe_all(sh, sdup(cmds[i]));
        }
    }
    free2(cmds);
    free(cmd);
    return (sh->exit_c);
}

int exe_and(t_sh *sh, char *cmd1, char *cmd2)
{
    // printf("--- and: %s --- %s\n",cmd1, cmd2);
    sh->exit_c = exe_all(sh, sdup(cmd1));
    free(cmd1);
    if (sh->exit_c)
    {
        free(cmd2);
        return (sh->exit_c);
    }
    sh->exit_c = exe_all(sh, sdup(cmd2));
    free(cmd2);
    return (sh->exit_c);
}

int exe_or(t_sh *sh, char *cmd1, char *cmd2)
{
    sh->exit_c = exe_all(sh, sdup(cmd1));
    if (!sh->exit_c)
    {
        return (0);
    }
    sh->exit_c = exe_all(sh, sdup(cmd2));
    free(cmd1);
    free(cmd2);
    return (sh->exit_c);
}

int split_token(char *cmd)
{
    int i = 0;
    while (cmd[i] && cmd[++i])
    {
        if (cmd[i - 1] == '\'')
        {
            ++i;
            while (cmd[i - 1] != '\'')
                ++i;
        }
        else if (cmd[i - 1] == '"')
        {
            ++i;
            while (cmd[i - 1] != '"')
                ++i;
        }
        else if (cmd[i - 1] == '(')
        {
            while (cmd[i - 1] != ')')
                ++i;
        }
        if (cmd[i] == '&' && cmd[i - 1] == '&')
        {
            cmd[i] = 0;
            cmd[i-1] = 0;
            return (10 * i + 1);
        }
        if (cmd[i] == '|' && cmd[i - 1] == '|')
        {
            cmd[i] = 0;
            cmd[i-1] = 0;
            return (10 * i + 2);
        }
    }
    return (0);
}

int exe_all(t_sh *sh, char *cmd) {

    int find;
    // printf("after trm: %s\n",cmd);
    // printf("--- all: %s\n",cmd);
    find = split_token(cmd);

    if (find % 10 == 1)
        sh->exit_c = exe_and(sh, sdup(cmd), sdup(cmd + find / 10 + 1));
    else if (find % 10 == 2)
        sh->exit_c = exe_or(sh, sdup(cmd), sdup(cmd + find / 10 + 1));
    else
    {
        if (trm_prth(cmd))
            sh->exit_c = exe_all(sh, sdup(cmd));
        else
            sh->exit_c = exe_one(sh, sdup(cmd));
    }
    free(cmd);
    return (sh->exit_c);
}
