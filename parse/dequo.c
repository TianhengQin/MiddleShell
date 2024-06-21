#include "shell.h"

// \22 '
// \23 "
// \24 *
// \21 ?

// char *dequo(t_sh *sh, char *cmd)
// {
//     init_bf(sh);
// 	sh->i = -1;
// 	sh->j = 0;
// 	sh->quo = 0;
//     apend_bf(sh, RS);
// 	while (cmd[++(sh->i)])
// 	{
// 		sh->quo = check_quo(sh->quo, cmd[sh->i], 10);
//         if (sh->quo > 10)
//             sh->j = sh->i;
// 		if (sh->quo >= 10)
//         {
//             if (sh->i == sh->j + 1)
//                 apend_bf(sh, 'E');
// 			sh->quo = sh->quo - 10;
//         }
//         else if (sh->quo)
//             apend_bf(sh, cmd[sh->i]);
// 		else if (sh->quo == 0)
// 		{
//             if (cmd[sh->i] == '\22')
//                 apend_bf(sh, '\'');
//             else if (cmd[sh->i] == '\23')
//                 apend_bf(sh, '"');
//             else if (cmd[sh->i] == '*')
//                 apend_bf(sh, '\22');
//             else if (cmd[sh->i] == '?')
//                 apend_bf(sh, '\23');
//             else
//                 apend_bf(sh, cmd[sh->i]);
// 		}
// 	}
//     apend_bf(sh, RS);
//     free(cmd);
//     return (sdup(sh->bf));
// }

char *repls_wikd(t_sh *sh, char *cmd)
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
            if (cmd[sh->i] == '*')
                apend_bf(sh, '\24');
            else if (cmd[sh->i] == '?')
                apend_bf(sh, '\21');
            else
                apend_bf(sh, cmd[sh->i]);
		}
	}
    apend_bf(sh, RS);
    free(cmd);
    return (sdup(sh->bf));
}

// char *dequo(t_sh *sh, char *cmd)
// {
//     init_bf(sh);
// 	sh->i = -1;
// 	sh->j = 0;
// 	sh->quo = 0;
//     apend_bf(sh, RS);
// 	while (cmd[++(sh->i)])
// 	{
// 		sh->quo = check_quo(sh->quo, cmd[sh->i], 10);
//         if (sh->quo > 10)
//             sh->j = sh->i;
// 		if (sh->quo >= 10)
//         {
//             if (sh->i == sh->j + 1)
//                 apend_bf(sh, 'E');
// 			sh->quo = sh->quo - 10;
//         }
//         else if (sh->quo)
//             apend_bf(sh, cmd[sh->i]);
// 		else if (sh->quo == 0)
// 		{
//             if (cmd[sh->i] == '\22')
//                 apend_bf(sh, '\'');
//             else if (cmd[sh->i] == '\23')
//                 apend_bf(sh, '"');
//             else if (cmd[sh->i] == '*')
//                 apend_bf(sh, '\22');
//             else if (cmd[sh->i] == '?')
//                 apend_bf(sh, '\23');
//             else
//                 apend_bf(sh, cmd[sh->i]);
// 		}
// 	}
//     apend_bf(sh, RS);
//     free(cmd);
//     return (sdup(sh->bf));
// }

void dequo(char *c)
{
    int i;
    int j;
    int quo;

    i = -1;
    j = 0;
    quo = 0;
    while (c[++i])
    {
        quo = check_quo(quo, c[i], 10);
        if (quo >= 10)
            quo = quo - 10;
        else
            c[j++] = c[i];
    }
    c[j] = 0;
}

void dequor(char *c)
{
    int i;
    int j;
    int quo;

    i = 0;
    j = 0;
    quo = 0;
    while (c[i])
    {
        quo = check_quo(quo, c[i], 10);
        if (quo >= 10)
            quo = quo - 10;
        else
            c[j++] = c[i];
        i++;
    }
    c[j] = 0;
}
