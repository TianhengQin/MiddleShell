/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiqin <tiqin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 01:30:12 by tiqin             #+#    #+#             */
/*   Updated: 2023/11/17 03:34:42 by tiqin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	find_var(char **str, char *s)
{
	int		i;
	int	len;
	int	diff;

	i = 0;
	len = 0;
	while (s[len] != '=')
		len++;
	len++;
	while (str[i])
	{
		diff = sncmp(str[i], s, len);
		if (diff == 0 || diff == -61)
			return (i);
		i++;
	}
	return (-1);
}

void	init_env(t_sh *sh)
{
	int		i;
	char	**re;

	i = 0;
	while (sh->env[i])
		i++;
	re = malloc((i + 1) * sizeof(char *));
	if (!re)
		return ;
	i = -1;
	while (sh->env[++i])
		re[i] = sdup(sh->env[i]);
	sh->env = re;
	set_pwd(sh);
}

void	set_odpwd(t_sh *shell)
{
	int	i;

	i = find_var(shell->env, "OLDPWD=");
	if (i >= 0)
		shell->odpwd = sdup(shell->env[i] + 7);
	else
		shell->odpwd = sdup(shell->pwd);
}

void	set_home(t_sh *shell)
{
	int	i;

	i = find_var(shell->env, "HOME=");
	if (i >= 0)
		shell->home = sdup(shell->env[i] + 5);
	else
		shell->home = sdup("");
}

void	set_user(t_sh *shell)
{
	int	i;

	i = find_var(shell->env, "USER=");
	if (i >= 0)
		shell->user = sdup(shell->env[i] + 5);
	else
		shell->user = sdup("(x л x)");

}

void	set_env(t_sh *shell)
{
	int		i;

	init_env(shell);
	set_odpwd(shell);
	set_user(shell);
	set_home(shell);
	i = find_var(shell->env, "PATH=");
	if (i >= 0)
		shell->evpth = ft_split(shell->env[find_var(shell->env, "PATH=")] + 5, ":");
	else
		shell->evpth = 0;
	shell->runing = 1;
	if (find_var(shell->env, "SHLVL=") >= 0)
		shell->env[find_var(shell->env, "SHLVL=")][6]++;
	else
		env_append(shell, "SHLVL=1");
	shell->exit_c = 0;
	shell->stdi = -1;
	shell->stdo = -1;
	shell->pip = 0;
}

/*
set env reverse
*/
void	set_env_r(t_sh *shell)
{
	int		i;
	char	*pwd;

	pwd = sjoin("PWD=", shell->pwd);
	i = find_var(shell->env, "PWD=");
	if (i >= 0)
	{
		free(shell->env[i]);
		shell->env[i] = sjoin("PWD=", shell->pwd);
	}
	else
		env_append(shell, pwd);
	free(pwd);
	pwd = sjoin("OLDPWD=", shell->odpwd);
	i = find_var(shell->env, "OLDPWD=");
	if (i >= 0)
	{
		free(shell->env[i]);
		shell->env[i] = sjoin("OLDPWD=", shell->odpwd);
	}
	else
		env_append(shell, pwd);
	free(pwd);
}

void	set_envpth(t_sh *sh)
{
	if (sh->evpth)
		free2(sh->evpth);
	if (find_var(sh->env, "PATH=") >= 0)
	{
		sh->evpth = ft_split(sh->env[find_var(sh->env, "PATH=")] + 5, ":");
		if (!sh->evpth)
			free_sh(sh, 2);
	}
	else
		sh->evpth = 0;
}
