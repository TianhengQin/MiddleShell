/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sutils1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiqin <tiqin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 01:10:36 by tiqin             #+#    #+#             */
/*   Updated: 2023/11/18 10:45:56 by tiqin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	len2(char **str)
{
	int	i;

	i = 0;
	while (str[i] != 0)
	{
		i++;
	}
	return (i);
}

int	len3(char ***css)
{
	int	i;

	i = 0;
	while (css[i] != 0)
	{
		i++;
	}
	return (i);
}

char	*trm_sp(char *s)
{
	int	i;
	char	*re;

	i = 0;
	while (s[i] && is_in(" \t\v\f\r\n", s[i]))
		i++;
	re = sdup(&s[i]);
	if (!re)
		return (0);
	i = len(re) - 1;
	while (i >= 0 && is_in(" \t\v\f\r\n", re[i]))
		re[i--] = 0;
	free(s);
	return (re);
}

char	*sdupr(const char *src)
{
	char	*dup;
	int	i;

	i = 0;
	while (src[i] && src[i] != RS)
	{
		i++;
	}
	dup = (char *)malloc(i + 1);
	if (!dup)
		return (0);
	i = 0;
	while (src[i] && src[i] != RS)
	{
		dup[i] = src[i];
		i++;
	}
	dup[i] = 0;
	return (dup);
}

int	lenr(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != RS)
	{
		i++;
	}
	return (i);
}
