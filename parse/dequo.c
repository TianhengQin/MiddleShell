#include "shell.h"

// \22 *
// \23 ?

char *dequo(t_sh *sh, char *cmd)
{
    init_bf(sh);
	sh->i = -1;
	sh->j = 0;
	sh->quo = 0;
    apend_bf(sh, '^');
	while (cmd[++(sh->i)])
	{
		sh->quo = check_quo(sh->quo, cmd[sh->i]);
		if (sh->quo >= 10)
			sh->quo = sh->quo - 10;
        else if (sh->quo)
            apend_bf(sh, cmd[sh->i]);
		else if (sh->quo == 0)
		{
            if (cmd[sh->i] == '\22')
                apend_bf(sh, '\'');
            else if (cmd[sh->i] == '\23')
                apend_bf(sh, '"');
            else if (cmd[sh->i] == '*')
                apend_bf(sh, '\22');
            else if (cmd[sh->i] == '?')
                apend_bf(sh, '\23');
            else
                apend_bf(sh, cmd[sh->i]);
		}
	}
    apend_bf(sh, '^');
    free(cmd);
    return (sdup(sh->bf));
}
