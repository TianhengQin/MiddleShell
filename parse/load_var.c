#include "shell.h"

//status 0 normal 1 in '' 2 in ""

int check_quo(int status, char c, int self)
{
	if (status == 0 && c == '"')
	{
		status = 2 + self;
	}
	else if (status == 0 && c == '\'')
	{
		status = 1 + self;
	}
	else if (status == 1 && c == '\'')
	{
		status = 0 + self;
	}
	else if (status == 2 && c == '"')
	{
		status = 0 + self;
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

char	trans(char c, int splt)
{
	if (is_in(" \t\v\f\r\n", c) && splt)
		return (RS);
	if (c == '"')
		return ('\23');
	if (c == '\'')
		return ('\22');
	if (c == '(')
		return ('\2');
	if (c == ')')
		return ('\3');
	if (c == '>')
		return ('\16');
	if (c == '<')
		return ('\17');
	if (c == '|')
		return ('\34');
	if (c == '&')
		return ('\35');
	return (c);
}

char de_trans(char c)
{
	if (c == '\23')
		return ('"');
	if (c == '\22')
		return ('\'');
	if (c == '\2')
		return ('(');
	if (c == '\3')
		return (')');
	if (c == '\16')
		return ('>');
	if (c == '\17')
		return ('<');
	if (c == '\34')
		return ('|');
	if (c == '\35')
		return ('&');
	return (c);
}

void sde_trans(char *c)
{
	int i;

	dequo(c);
	i = 0;
	while (c[i])
	{
		c[i] = de_trans(c[i]);
		i++;
	}
}

void sde_trans2(char **cs)
{
	int i;

	i = 0;
	while (cs[i])
	{
		sde_trans(cs[i]);
		i++;
	}
}

void	apend_var(t_sh *sh, char *ev, char splt)
{
	int		i;

	i = -1;
	while (ev[++i])
	{
		if (splt == ' ')
			apend_bf(sh, ev[i]);
		else
			apend_bf(sh, trans(ev[i], 1));
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
		sh->quo = check_quo(sh->quo, cmd[sh->i], 0);
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
				sh->i += apend_dolr(cmd + sh->i + 1, sh, RS);
			else
				apend_bf(sh, cmd[sh->i]);
		}
	}
	free(cmd);
	return (sdup(sh->bf));
}
