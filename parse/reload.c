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
    while (s[i] && s[i] != '^')
    {
        if (s[i] == '\22')
            apend_bf(sh, '*');
        else if (s[i] == '\23')
            apend_bf(sh, '?');
        else
            apend_bf(sh, s[i]);
        i++;
    }
}

char sp2dc(char c)
{
    if (is_in(" \t\v\f\r\n", c))
        return ('\21');
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
    else if (s[0] == '>' && s[1] == '>')
        return 8;
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
	while (cmd[++(sh->i)])
	{
		sh->quo = check_quo(sh->quo, cmd[sh->i]);
		if (sh->quo >= 10)
			sh->quo = sh->quo - 10;
		if (sh->quo)
            apend_bf(sh, cmd[sh->i]);
		else if (sh->quo == 0)
		{
            sh->tokn = is_tokn(&cmd[sh->i]);
            if (sh->tokn > 5)
            {
                apend_bf(sh, '\21');
                apend_bf(sh, cmd[(sh->i)++]);
                apend_bf(sh, cmd[sh->i]);
                apend_bf(sh, '\21');
            }
            else if (sh->tokn)
            {
                apend_bf(sh, '\21');
                apend_bf(sh, cmd[sh->i]);
                apend_bf(sh, '\21');
            }
            else
                apend_bf(sh, sp2dc(cmd[sh->i]));
		}
	}
    return 0;
}
