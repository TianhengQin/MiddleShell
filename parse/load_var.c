#include "shell.h"

//status 0 normal 1 in '' 2 in ""

int check_quo(int status, char c)
{
	if (status == 0 && c == '"')
	{
		status = 12;
	}
	else if (status == 0 && c == '\'')
	{
		status = 11;
	}
	else if (status == 1 && c == '\'')
	{
		status = 10;
	}
	else if (status == 2 && c == '"')
	{
		status = 10;
	}
	return (status);
}

void	apend_exit(t_sh *sh)
{
	int	exit;

	exit = sh->exit_c;
	if (exit / 100)
	{
		apend_bf(sh, exit / 100 + 48);
	}
	if (exit / 100 || exit % 100 / 10)
	{
		apend_bf(sh, exit % 100 / 10 + 48);
	}
	exit = exit % 10;
	apend_bf(sh, exit + 48);
}

void	apend_var(t_sh *sh, char *ev, char splt)
{
	int		i;

	i = -1;
	while (ev[++i])
	{
		if (is_in(" \t\v\f\r\n", ev[i]))
		{
			apend_bf(sh, splt);
		}
		else if (ev[i] == '"')
		{
			apend_bf(sh, '\23');
		}
		else if (ev[i] == '\'')
		{
			apend_bf(sh, '\22');
		}
		else
		{
			apend_bf(sh, ev[i]);
		}
	}
}

int	apend_dolr(char *c, t_sh *sh, char splt)
{
	int		len;
	char	tmp;
	int		f;

	if (c[0] == '?')
	{
		apend_exit(sh);
		return (1);
	}
	len = 0;
	while (is_aphnum(c[len]))
		len++;
	tmp = c[len];
	c[len] = '=';
	f = find_var(sh->env, c);
	c[len] = tmp;
	if (f >= 0)
		apend_var(sh, &sh->env[f][len + 1], splt);
	return (len);
}

char *load_var(t_sh *sh, char *cmd)
{
	sh->i = -1;
	sh->j = 0;
	init_bf(sh);
	sh->quo = 0;
	while (cmd[++(sh->i)])
	{
		sh->quo = check_quo(sh->quo, cmd[sh->i]);
		if (sh->quo >= 10)
			sh->quo = sh->quo - 10;
		if (sh->quo == 1)
		{
			apend_bf(sh, cmd[sh->i]);
		}
		else if (sh->quo == 2)
		{
			if (cmd[sh->i] == '$' && (is_apha(cmd[sh->i + 1]) || cmd[sh->i + 1] == '?'))
				sh->i += apend_dolr(cmd + sh->i + 1, sh, ' ');
			else
			{
				apend_bf(sh, cmd[sh->i]);
			}
		}
		else if (sh->quo == 0)
		{
			if (cmd[sh->i] == '$' && (is_apha(cmd[sh->i + 1]) || cmd[sh->i + 1] == '?'))
				sh->i += apend_dolr(cmd + sh->i + 1, sh, '^');
			else
			{
				if (cmd[sh->i] == '\21')
				{
					apend_bf(sh, '^');
				}
				else
				{
					apend_bf(sh, cmd[sh->i]);
				}
			}
		}
	}
	free(cmd);
	return (sdup(sh->bf));
}
