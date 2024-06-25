#include "shell.h"

void skip_prth(char *cmd, int *i)
{
    int ps;

    ps = 1;
    while (cmd[*i] && ps)
    {
        if (cmd[*i] == '\'')
        {
            ++(*i);
            while (cmd[*i] != '\'')
                ++(*i);
        }
        else if (cmd[*i] == '"')
        {
            ++(*i);
            while (cmd[*i] != '"')
                ++(*i);
        }
        if (cmd[*i] == '(')
            ps++;
        else if (cmd[*i] == ')')
            ps--;
        (*i)++;
    }
}

int split_token(char *cmd)
{
    int i;

    i = 0;
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
            skip_prth(cmd, &i);
        if (cmd[i] == '&' && cmd[i - 1] == '&')
        {
            cmd[i] = 0;
            cmd[i - 1] = 0;
            return (10 * i + 1);
        }
        if (cmd[i] == '|' && cmd[i - 1] == '|')
        {
            cmd[i] = 0;
            cmd[i - 1] = 0;
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

int check_malloc(t_sh *sh, char *s1, char *s2, int i)
{
    (void)sh;
    if (i)
    {
        if (!s1 || !s2)
        {
            free1(s1);
            free1(s2);
            return (-1);
        }
    }
    else
    {
        if (!s1)
        {
            free(s1);
            return (-1);
        }
    }
    return (0);
}

int exe_all(t_sh *sh, char *cmd, int fork)
{
    int find;

    if (check_malloc(sh, cmd, 0, 0) == -1)
        return (12);
    find = split_token(cmd);
    if (find % 10 == 1)
        sh->exit_c = exe_and(sh, sdup(cmd), sdup(cmd + find / 10 + 1));
    else if (find % 10 == 2)
        sh->exit_c = exe_or(sh, sdup(cmd), sdup(cmd + find / 10 + 1));
    else
        sh->exit_c = exe_one(sh, sdup(cmd), fork);
    free(cmd);
    if (fork)
        free_sh(sh, 1);
    return (sh->exit_c);
}
