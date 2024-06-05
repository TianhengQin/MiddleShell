#include "shell.h"

//status 0 normal 1 in '' 2 in ""

int check_quo(int status, int *j, char *re, char c)
{
	(void)(re);
	(void)(j);
	if (status == 0 && c == '"')
	{
		status = 12;
		// re[(*j)++] = c;
	}
	else if (status == 0 && c == '\'')
	{
		status = 11;
		// re[(*j)++] = c;
	}
	else if (status == 1 && c == '\'')
	{
		status = 10;
		// re[(*j)++] = c;
	}
	else if (status == 2 && c == '"')
	{
		status = 10;
		// re[(*j)++] = c;
	}
	return (status);
}

void	apend_exit(t_sh *sh, char *re)
{
	int	exit;

	exit = sh->exit_c;
	if (exit / 100)
		re[(sh->j)++] = exit / 100 + 48;
	if (exit / 100 || exit % 100 / 10)
		re[(sh->j)++] = exit % 100 / 10 + 48;
	exit = exit % 10;
	re[(sh->j)++] = exit + 48;
}

void	apend_var(char *re, int *j, char *ev, char splt)
{
	int		i;

	i = -1;
	while (ev[++i])
	{
		if (ev[i] == ' ')
			re[(*j)++] = splt;
		else
			re[(*j)++] = ev[i];
	}
}

int	apend_dolr(char *c, t_sh *sh, char *re, char splt)
{
	int		len;
	char	tmp;
	int		f;

	if (c[0] == '?')
	{
		apend_exit(sh, re);
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
		apend_var(re, &(sh->j), &sh->env[f][len + 1], splt);
	return (len);
}

char *load_var(t_sh *sh, char *cmd)
{
	char *re;

	sh->i = -1;
	sh->j = 0;
	re = malloc(8192);
	if (!re)
		return 0;
	sh->quo = 0;
	while (cmd[++(sh->i)])
	{
		sh->quo = check_quo(sh->quo, &(sh->j), re, cmd[sh->i]);
		if (sh->quo >= 10)
		{
			sh->quo = sh->quo - 10;
			continue ;
		}
		if (sh->quo == 1)
			re[(sh->j)++] = cmd[sh->i];
		if (sh->quo == 2)
		{
			if (cmd[sh->i] == '$' && is_apha(cmd[sh->i + 1]))
				sh->i += apend_dolr(cmd + sh->i + 1, sh, re, ' ');
			else
				re[(sh->j)++] = cmd[sh->i];
		}
		if (sh->quo == 0)
		{
			if (cmd[sh->i] == '$' && is_apha(cmd[sh->i + 1]))
				sh->i += apend_dolr(cmd + sh->i + 1, sh, re, DC1);
			else
				re[(sh->j)++] = cmd[sh->i];
		}
	}
	re[sh->j] = 0;
	return sdupf(re);
}
