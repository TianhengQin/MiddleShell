/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiqin <tiqin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 23:59:33 by tiqin             #+#    #+#             */
/*   Updated: 2023/11/18 10:33:07 by tiqin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	rm_signal(void)
{
	struct termios	ter;

	tcgetattr(0, &ter);
	ter.c_lflag &= ~ECHOCTL;
	tcsetattr(0, 0, &ter);
}

void	to_lower(char *cs)
{
	int i;

	i = -1;
	while(cs[++i])
	{
		if (cs[i] >= 'A' && cs[i] <= 'Z')
			cs[i] = cs[i] + 32;
	}
}

void	set_signal_exe(char *cs)
{
	int	i;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	i = len(cs);
	if (i < 3)
		return ;
	cs = sdup(&cs[i - 3]);
	to_lower(cs);
	if (sncmp(cs, "man", 4) == 0)
		signal(SIGQUIT, SIG_IGN);
	free(cs);
}
