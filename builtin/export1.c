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
	while (is_apha(s[i]))
		i++;
	if (s[i] == 0)
		return (0);
	if (s[i] == '=')
		return (1);
	if (s[i] == '+' && s[i + 1] == '=')
		return (2);
	return (-1);
}
