#include "shell.h"


int split_token(char *cmd)
{
    int i = 0;
    int ps = 1;
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
            while (cmd[i] && ps)
            {
                if (cmd[i] == '\'')
                {
                    ++i;
                    while (cmd[i] != '\'')
                        ++i;
                }
                else if (cmd[i] == '"')
                {
                    ++i;
                    while (cmd[i] != '"')
                        ++i;
                }
                if (cmd[i] == '(')
                    ps++;
                else if (cmd[i] == ')')
                    ps--;
                i++;
            }
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
    if (s[i] == ')' && s[j] == '(' && i > j)
    {
        re = 1;
        s[i--] = RS;
        s[j++] = RS;
    }
    // while (s[i] == ')' && s[j] == '(' && i > j)
    // {
    //     re = 1;
    //     s[i--] = RS;
    //     s[j++] = RS;
    //     while (i >= 0 && s[i] == RS)
	// 	    i--;
    //     while (s[j] && s[j] == RS)
	// 	    j++;
    // }
    return (re);
}

int exe_all(t_sh *sh, char *cmd, int fork)
{
    int find;

    sh->stdi = dup(0);
	sh->stdo = dup(1);
    // fprint(2, "--- all: %s\n", cmd);
    find = split_token(cmd);
    if (find % 10 == 1)
        sh->exit_c = exe_and(sh, sdup(cmd), sdup(cmd + find / 10 + 1));
    else if (find % 10 == 2)
        sh->exit_c = exe_or(sh, sdup(cmd), sdup(cmd + find / 10 + 1));
    else
        sh->exit_c = exe_one(sh, sdup(cmd), fork);
    free(cmd);
    dup2(sh->stdi, 0);
	dup2(sh->stdo, 1);
    close(sh->stdi);
    close(sh->stdo);
    if (fork)
        exit(sh->exit_c);
    return (sh->exit_c);
}
