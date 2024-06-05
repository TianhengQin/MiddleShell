#include "shell.h"


#include "shell.h"

void trm_prth(char *s)
{
    int		i;
    int j;

    i = 0;
    while (is_in(" \t\v\f\r\n", s[i]))
        ++i;
	j = len(s) - 1;
	while (j >= 0 && is_in(" \t\v\f\r\n", s[i]))
        --j;
    if (s[i] == '(' && s[j] == ')')
    {
        s[i] = ' ';
        s[j] = ' ';
        trm_prthsp(s);
    }
}

int exe_one(t_sh *sh, char *cmd) {

    (void)sh;
    trm_prths(cmd);
    printf("%s\n", cmd);
    return 0;
}

int exe_and(t_sh *sh, char *cmd1, char *cmd2) {
    (void)sh;
    int re;
    re = exe_all(sh, cmd1);
    if (re == 1)
        return 3;
    else if (re)
        return re;
    re = exe_all(sh, cmd2);
    if (re == 1)
        return 3;
    else if (re)
        return (re);
    return 0;
}

int exe_or(t_sh *sh, char *cmd1, char *cmd2) {

    (void)sh;
    int re;
    re = exe_all(sh, cmd1);
    if (re == 1)
        return 4;
    else if (re)
        return re;
    re = exe_all(sh, cmd2);
    if (re == 1)
        return 4;
    else if (re)
        return (re);
    return 0;
}

int split_token(char *cmd) {
    int i = 0;
    while (cmd[++i]) {
        if (cmd[i - 1] == '(')
        {
            while (cmd[i] && cmd[i] != ')')
                ++i;
        }
        else if (cmd[i - 1] == '"')
        {
            while (cmd[i] && cmd[i] != '"')
                ++i;
        }
        else if (cmd[i] && cmd[i - 1] == '\'')
        {
            while (cmd[i] && cmd[i] != '\'')
                ++i;
        }
        if (cmd[i] == '&' && cmd[i - 1] == '&') {
            cmd[i] = 0;
            cmd[i-1] = 0;
            return (10 * i + 1);
        }
        if (cmd[i] == '|' && cmd[i - 1] == '|') {
            cmd[i] = 0;
            cmd[i-1] = 0;
            return (10 * i + 2);
        }
    }
    return (0);
}

int check_prths(char *cmd)
{
    int i;
    int prth;

    i = -1;
    prth = 0;
    while (cmd[++i])
    {
        if (cmd[i] == '(')
            ++prth;
        else if (cmd[i] == ')')
            --prth;
    }
    if (prth == 0)
        return 0;
    else if (prth > 0)
        return 1;
    else
        return 2;
}

int check_all(t_sh *sh, char *cmd) {

    (void)sh;

    int find;
    if (!cmd)
        return (6);

    if (is_mpty(cmd))
        return 1;

    find = check_prths(cmd);
    if (find)
        return find;

    trm_prth(cmd);

    find = split_token(cmd);

    if (find % 10 == 1)
        return check_and(sh, sdup(cmd), sdup(cmd + find / 10 + 1));
    else if (find % 10 == 2)
        return check_or(sh, sdup(cmd), sdup(cmd + find / 10 + 1));
    else
        return check_one(sh, sdup(cmd));
    fre(cmd);
}
