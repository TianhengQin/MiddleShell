/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiqin <tiqin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 00:50:28 by tiqin             #+#    #+#             */
/*   Updated: 2023/11/17 02:17:37 by tiqin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	skip_prfs(char *dup, size_t *i, int *ps)
{
	if (dup[*i] == 39 || dup[*i] == 34)
		*i = skip_quo(dup, *i);
	if (dup[*i] == '(')
		(*ps)++;
	else if (dup[*i] == ')')
		(*ps)--;
	(*i)++;
}

size_t	skip_quo(char *dup, size_t i)
{
	int ps;

	if (dup[i] == (39))
	{
		i++;
		while (dup[i] && dup[i] != 39)
			i++;
	}
	else if (dup[i] == 34)
	{
		i++;
		while (dup[i] && dup[i] != 34)
			i++;
	}
	else if (dup[i] == '(')
	{
		i++;
		ps = 1;
		while (dup[i] && ps)
			skip_prfs(dup, &i, &ps);
	}
	if (dup[i])
		return (i);
	else
		return (i - 1);
}

static size_t	cut(char *dup, char *set)
{
	size_t	i;
	size_t	count;

	i = 0UL;
	count = 0UL;
	if (dup[0] == 0)
		return (0);
	if (is_in(set, dup[0]) == 0)
		count++;
	while (dup[i])
	{
		if (dup[i] == 39 || dup[i] == 34 || dup[i] == '(')
			i = skip_quo(dup, i);
		if (is_in(set, dup[i]))
		{
			dup[i] = 0;
			if (dup[i + 1] && is_in(set, dup[i + 1]) == 0)
				count++;
		}
		i++;
	}
	return (count);
}

static int	w_split(char **re, char *dup, size_t len)
{
	size_t	i;
	int		tot;

	i = 0UL;
	tot = 0;
	if (dup[0])
	{
		re[tot] = sdup((char *)dup);
		if (!re[tot])
			return (tot);
		tot++;
	}
	while (i + 1 < len)
	{
		if ((dup[i] == 0) && dup[i + 1])
		{
			re[tot] = sdup((char *)dup + i + 1);
			if (!re[tot])
				return (tot);
			tot++;
		}
		i++;
	}
	re[tot] = 0;
	return (-1);
}

char	**split(char const *s, char *c)
{
	char		*dup;
	char		**re;
	size_t		tot;
	int			i;

	dup = sdup((char *)s);
	if (!dup)
		return (0);
	tot = cut(dup, c);
	re = (char **)malloc((tot + 1) * sizeof(char *));
	if (!re)
	{
		free(dup);
		return (0);
	}
	i = w_split(re, dup, len(s));
	if (i >= 0)
	{
		free_re(re, i);
		re = 0;
	}
	free(dup);
	return (re);
}
