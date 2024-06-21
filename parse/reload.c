#include "shell.h"

//status 0 normal 1 in '' 2 in ""

void apend_bf(t_sh *sh, char c)
{
    int i;
    char *nbf;

    if (sh->bf_inx == sh->bf_sz)
    {
        sh->bf_sz *= 2;
        nbf = malloc(sh->bf_sz + 1);
        if (!nbf)
            free_sh(sh, 2);
        i = -1;
        while (++i < sh->bf_inx)
            nbf[i] = sh->bf[i];
        nbf[i] = 0;
        free(sh->bf);
        sh->bf = nbf;
    }
    sh->bf[sh->bf_inx] = c;
    ++(sh->bf_inx);
    sh->bf[sh->bf_inx] = 0;
}

void apends_bf(t_sh *sh, char *s)
{
    int i;

    i = 0;
    while (s[i] && s[i] != RS)
    {
        if (s[i] == '\24')
            apend_bf(sh, '*');
        else if (s[i] == '\21')
            apend_bf(sh, '?');
        else
            apend_bf(sh, trans(s[i], 0));
        i++;
    }
}

char sp2dc(char c)
{
    if (is_in(" \t\v\f\r\n", c))
        return ('\36');
    return c;
}

/*
1 (
2 )
3 >
4 <
5 |
6 &&
7 ||
8 >>
9 <<
10 2>
11 2>>
*/

int is_tokn(char *s)
{
    if (s[0] == '(')
        return 1;
    else if (s[0] == ')')
        return 2;
    else if (s[0] == '&' && s[1] == '&')
        return 6;
    else if (s[0] == '|' && s[1] == '|')
        return 7;
    else if (s[0] == '|')
        return 5;
    else if (s[0] >= '0' && s[0] <= '9' && s[1] == '>' && s[2] == '>')
        return 11;
    else if (s[0] == '>' && s[1] == '>')
        return 8;
    else if (s[0] >= '0' && s[0] <= '9' && s[1] == '>')
        return 10;
    else if (s[0] == '>')
        return 3;
    else if (s[0] == '<' && s[1] == '<')
        return 9;
    else if (s[0] == '<')
        return 4;
    return 0;
}

void init_bf(t_sh *sh)
{
    sh->bf_inx = 0;
    sh->bf[0] = 0;
}

char *reload(t_sh *sh, char *cmd)
{
    init_bf(sh);
	sh->i = -1;
	sh->j = 0;
	sh->quo = 0;
    apend_bf(sh, RS);
	while (cmd[++(sh->i)])
	{
		sh->quo = check_quo(sh->quo, cmd[sh->i], 0);
		if (sh->quo)
            apend_bf(sh, cmd[sh->i]);
		else if (sh->quo == 0)
		{
            sh->tokn = is_tokn(&cmd[sh->i]);
            if (sh->tokn)
            {
                apend_bf(sh, '\36');
                if (sh->tokn > 5)
                    apend_bf(sh, cmd[(sh->i)++]);
                if (sh->tokn > 10)
                    apend_bf(sh, cmd[(sh->i)++]);
                apend_bf(sh, cmd[sh->i]);
                apend_bf(sh, '\36');
            }
            else
                apend_bf(sh, sp2dc(cmd[sh->i]));
		}
	}
    apend_bf(sh, RS);
    return 0;
}
