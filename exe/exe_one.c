
#include "shell.h"

void trm_prthsp(char *s)
{
    int		i;
    int     j;

	i = len(s) - 1;
	while (i >= 0 && s[i] == RS)
		i--;
	j = 0;
	while (s[j] && s[j] == RS)
		j++;
    while (s[i] == ')' && s[j] == '(' && i > j)
    {
        s[i--] = RS;
        s[j++] = RS;
        while (i >= 0 && s[i] == RS)
		    i--;
        while (s[j] && s[j] == RS)
		    j++;
    }
}

int exe_one(t_sh *sh, char *cmd) {

    (void)sh;

    char **cmds = ft_split(cmd, "|");
    int i = -1;
    if (cmds[0] == 0)
        printf("[]\n");
    if (cmds[1] == 0)
    {
        char **cmdss = ft_split(cmds[0], RSS);
        while (cmdss[++i])
        {
            printf("[%s]", cmdss[i]);
        }
        printf("\n");
        free2(cmdss);
    }
    else
    {
        while (cmds[++i])
        {
            exe_all(sh, cmds[i]);
        }
    }
    free2(cmds);
    free(cmd);
    return 0;
}

int exe_and(t_sh *sh, char *cmd1, char *cmd2) {
    (void)sh;
    sh->exit_c = exe_all(sh, cmd1);
    free(cmd1);
    if (sh->exit_c)
    {
        free(cmd2);
        return (sh->exit_c);
    }
    sh->exit_c = exe_all(sh, cmd2);
    free(cmd2);
    return (sh->exit_c);
}

int exe_or(t_sh *sh, char *cmd1, char *cmd2) {
    (void)sh;
    sh->exit_c = exe_all(sh, cmd1);
    // if (!sh->exit_c)
    // {
    //     return (0);
    // }
    sh->exit_c = exe_all(sh, cmd2);
    free(cmd1);
    free(cmd2);
    return (sh->exit_c);
}

int split_token(char *cmd) {
    int i = 0;
    while (cmd[i] && cmd[++i]) {
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


    (void)sh;

    int find;
    // printf("after trm: %s\n",cmd);
    find = split_token(cmd);

    if (find % 10 == 1)
        sh->exit_c = exe_and(sh, sdup(cmd), sdup(cmd + find / 10 + 1));
    else if (find % 10 == 2)
        sh->exit_c = exe_or(sh, sdup(cmd), sdup(cmd + find / 10 + 1));
    else
    {
        trm_prthsp(cmd);
        sh->exit_c = exe_one(sh, sdup(cmd));
    }
    return (sh->exit_c);
}
