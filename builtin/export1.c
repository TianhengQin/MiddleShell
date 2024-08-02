/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiqin <tiqin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:07:36 by tiqin             #+#    #+#             */
/*   Updated: 2023/11/16 02:43:59 by tiqin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

// int	valid_exp(char *s)
// {
// 	int	i;

// 	if (s[0] == '=')
// 		return (1);
// 	if (is_apha(s[0]) == 0)
// 		return (1);
// 	i = 0;
// 	while (s[++i] && s[i] != '=')
// 	{
// 		if (is_aphnum(s[i]) == 0)
// 			return (1);
// 	}
// 	return (0);
// }

int	valid_exp(char *s)
{
	int	i;

	i = 0;
	if (!is_apha(s[i]))
		return (-1);
	while (is_aphnum(s[i]))
		i++;
	if (i == 0)
		return (-1);
	if (s[i] == 0)
		return (0);
	if (s[i] == '=')
		return (1);
	if (s[i] == '+' && s[i + 1] == '=')
		return (2);
	return (-1);
}

char **bdenv(char **env)
{
	char	**exe;
	int		len;
	int		i;
	int		j;

	len = len2(env);
	exe = malloc(sizeof(char *) * (len + 1));
	if (!exe)
		return (0);
	j = -1;
	i = 0;
	while (env[++j])
	{
		if (is_in(env[j], '='))
		{
			exe[i] = sdup(env[j]);
			if (!exe[i++])
			{
				free2(exe);
				return (0);
			}
		}
	}
	exe[i] = 0;
	return (exe);
}
