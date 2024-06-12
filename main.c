/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiqin <tiqin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 03:31:37 by tiqin             #+#    #+#             */
/*   Updated: 2023/11/18 10:44:05 by tiqin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*get_prompt(t_sh *shell)
{
	char	*a;
	int		i;

	i = 0;
	a = sjoin((shell->user), "@midsh ");
	if (!a)
		return (free_sh(shell, 2));
	i = len(shell->pwd);
	while (shell->pwd[i] != '/')
		i--;
	if (!sncmp(shell->pwd, shell->home, 2147483647))
		a = sjoinf1(a, "~");
	else if (len(shell->pwd) < len(shell->home))
		a = sjoinf1(a, &(shell->pwd[i]));
	else
		a = sjoinf1(a, &(shell->pwd[i + 1]));
	if (!a)
		return (free_sh(shell, 2));
	a = sjoinf1(a, " % ");
	if (!a)
		return (free_sh(shell, 2));
	return (a);
}

void	rcv_sig(t_sh *shell)
{
	if (g_s)
		shell->exit_c = 128 + g_s;
	g_s = 0;
}

void	run_shell(t_sh *sh)
{
	char	*p;

    sh->runing = 1;
	while (sh->runing)
	{
		// printf("exit: %lld\n", sh->exit_c);
		p = get_prompt(sh);
		fprint(1, "%s\n",p);
		sh->cmd = readline(NULL);
		// sh->cmd = read_line(sh);
		free(p);
		if (!sh->cmd)
			break ;
		if (!sh->cmd[0] || all_sp(sh->cmd))
		{
			free(sh->cmd);
			sh->cmd = 0;
            continue;
		}
		add_history(sh->cmd);
		if (!check(sh, sh->cmd))
		{
			free(sh->cmd);
			sh->cmd = 0;
			continue;
		}
		free(sh->cmd);
		sh->cmd = sdup(sh->bf);
		if (!(sh->cmd))
			free_sh(sh, 2);
		// printf("exit: %lld\n", sh->exit_c);
		sh->cmd = load_var(sh, sh->cmd);
		if (!sh->cmd)
			free_sh(sh, 2);
		// printf("%s\n", sh->cmd);
		sh->cmd  = repls_wikd(sh, sh->cmd);
		if (!sh->cmd)
			free_sh(sh, 2);
		// printf("%s\n", sh->cmd);
		sh->cmd  = load_wikd(sh, sh->cmd);
		if (!sh->cmd)
			free_sh(sh, 2);
		// printf("[%s]\n\n", sh->cmd);
        exe_all(sh, sh->cmd);
		// printf("exit: %lld\n", sh->exit_c);
		// fprint(1, "a\n");
	}
	// printf("\n[Process completed]\n\n");
}

int	all(char **env)
{
	t_sh	sh;

	sh.exit_c = 0;
	sh.env = env;
	sh.bf = malloc(BF_SZ + 1);
	sh.bf[0] = 0;
	sh.bf_sz = BF_SZ;
	sh.bf_inx = 0;
	sh.bff = malloc(sizeof(char *) * (BFF_SZ + 1));
	sh.bff[0] = 0;
	sh.bff_sz = BFF_SZ;
	sh.bff_inx = 0;
	if (!sh.bf || !sh.bff)
		free_sh(&sh, 2);
	if (env[0])
		set_env(&sh);
	else
		set_no_env(&sh);
	run_shell(&sh);
	free_sh(&sh, 0);
	return (sh.exit_c);
}

int	main(int argi, char **argv, char **env)
{
	int	exit;

	(void)argi;
	(void)argv;
	exit = all(env);
	// sleep(10);
	return (exit);
}
