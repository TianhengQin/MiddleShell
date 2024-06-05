#include "shell.h"

//status 0 normal 1 in '' 2 in ""

void apend_bf(t_sh *sh, char c)
{
    sh->bf[sh->bf_inx] = c;
    ++(sh->bf_inx);
    sh->bf[sh->bf_inx] = 0;
}

char sp2dc(char c)
{
    if (is_in(" \t\v\f\r\n", c))
        return ('\20');
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
	char *re;

    init_bf(sh);
	sh->i = -1;
	sh->j = 0;
	re = malloc(8192);
	sh->quo = 0;
	while (cmd[++(sh->i)])
	{
		sh->quo = check_quo(sh->quo, &(sh->j), re, cmd[sh->i]);
		if (sh->quo >= 10)
		{
			sh->quo = sh->quo - 10;
			// continue ;
		}
		if (sh->quo) {
            apend_bf(sh, cmd[sh->i]);
			re[(sh->j)++] = cmd[sh->i];
            continue;
        }
		if (sh->quo == 0)
		{
            sh->tokn = is_tokn(&cmd[sh->i]);
            if (sh->tokn > 5)
            {
                apend_bf(sh, '\20');
                apend_bf(sh, cmd[(sh->i)]);
                apend_bf(sh, cmd[sh->i+1]);
                apend_bf(sh, '\20');
                re[(sh->j)++] = '\20';
                re[(sh->j)++] = cmd[(sh->i)++];
                re[(sh->j)++] = cmd[sh->i];
                re[(sh->j)++] = '\20';
            }
            else if (sh->tokn)
            {
                apend_bf(sh, '\20');
                apend_bf(sh, cmd[sh->i]);
                apend_bf(sh, '\20');
                re[(sh->j)++] = '\20';
                re[(sh->j)++] = cmd[sh->i];
                re[(sh->j)++] = '\20';
            }
            else {
                apend_bf(sh, sp2dc(cmd[sh->i]));
			    re[(sh->j)++] = sp2dc(cmd[sh->i]);
            }
		}
	}
	re[sh->j] = 0;
    printf("bf vs re: %d\n", sncmp(re, sh->bf, 8192));
	return sdupf(re);
}
