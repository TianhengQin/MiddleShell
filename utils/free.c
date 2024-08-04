/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiqin <tiqin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 00:22:25 by tiqin             #+#    #+#             */
/*   Updated: 2023/11/17 03:35:30 by tiqin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	free2re(char **cs, int re)
{
	int	i;

	if (!cs)
		return (re);
	i = 0;
	while (cs[i])
	{
		free(cs[i]);
		i++;
	}
	free(cs);
	return (re);
}

void	free2(char **cs)
{
	int	i;

	if (!cs)
		return ;
	i = 0;
	while (cs[i])
	{
		free(cs[i]);
		i++;
	}
	free(cs);
}

void	free3(char ***cs)
{
	int	i;

	if (!cs)
		return ;
	i = 0;
	while (cs[i])
	{
		free2(cs[i]);
		i++;
	}
	free(cs);
}

char	*free_sh(t_sh *sh, int if_exit)
{
	free2(sh->env);
	free2(sh->evpth);
	free1(sh->odpwd);
	free1(sh->pwd);
	free1(sh->user);
	free1(sh->home);
	free1(sh->bf);
	free1(sh->hbf);
	free1(sh->hirdoc);
	if (if_exit == 2)
		perror("midsh");
	if (if_exit)
		exit(sh->exit_c);
	return (0);
}

void	free_cmd(char *cmd, char ***css, t_sh *sh)
{
	unlink(HERE_DOC);
	free(cmd);
	free3(css);
	free3(sh->iof);
	if (sh->pip)
	{
		free(sh->pip);
		sh->pip = 0;
	}
}

void	free_fail(t_sh *sh, char **cs)
{
	free_sh(sh, 0);
	free3(sh->iof);
	free(sh->pip);
	sh->pip = 0;
	free2(cs);
}

void	free1(char *p)
{
	if (p)
		free(p);
}

void	free0(char **p)
{
	free(*p);
	*p = 0;
}
