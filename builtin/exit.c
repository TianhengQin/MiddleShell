/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiqin <tiqin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 01:59:06 by tiqin             #+#    #+#             */
/*   Updated: 2023/11/17 07:22:33 by tiqin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	intmin(char *s)
{
	int		i;
	char	*imin;

	imin = "-9223372036854775808";
	i = 1;
	while (s[i] >= '0' && s[i] <= '9')
		i++;
	if (i < 20)
		return (0);
	if (i > 20)
		return (1);
	if (sncmp(s, imin, 20) > 0)
		return (1);
	return (0);
}

int	intmaxp(char *s)
{
	int		i;
	char	*imin;

	imin = "+9223372036854775807";
	i = 1;
	while (s[i] >= '0' && s[i] <= '9')
		i++;
	if (i < 20)
		return (0);
	if (i > 20)
		return (1);
	if (sncmp(s, imin, 20) > 0)
		return (1);
	return (0);
}

int	intmax(char *s)
{
	int		i;
	char	*imin;

	imin = "9223372036854775807";
	i = 0;
	while (s[i] >= '0' && s[i] <= '9')
		i++;
	if (i < 19)
		return (0);
	if (i > 19)
		return (1);
	if (sncmp(s, imin, 19) > 0)
		return (1);
	return (0);
}

int	check_long(char *s)
{
	int	i;

	if (s[0] == 0)
		return (1);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (s[i] == 0)
		return (1);
	while (s[i])
	{
		if (s[i] > '9' || s[i] < '0')
			return (1);
		i++;
	}
	if (s[0] == '+')
		return (intmaxp(s));
	else if (s[0] == '-')
		return (intmin(s));
	else
		return (intmax(s));
}

void	run_exit(t_sh *sh, char **cs)
{
	if (cs[1])
		cs[1] = trm_sp(cs[1]);
	if (cs[1] && check_long(cs[1]))
	{
		fprint(2, "exit: %s: numeric argument required\n", cs[1]);
		sh->exit_c = 255;
		sh->runing = 0;
		return ;
	}
	if (cs[1] && !check_long(cs[1]) && cs[2])
	{
		fprint(2, "exit: too many arguments\n");
		sh->exit_c = 1;
		return ;
	}
	if (cs[1])
	{
		sh->exit_c = ati(cs[1]);
	}
	sh->runing = -1;
}
