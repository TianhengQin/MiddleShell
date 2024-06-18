/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiqin <tiqin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 03:31:59 by tiqin             #+#    #+#             */
/*   Updated: 2023/11/10 12:29:53 by tiqin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include <fcntl.h>

# define HERE_DOC ".here_doc_a72>7!e;wL=roaso(7uN]>uO46#bI1av85:ZX<tkCl8CPR.txt"
# define HISTORY ".midsh_history_a72>7!e;wL=roasfaso(7uN]>uO46#bI1aZX<tkCl8CPR.txt"
# define RS '\36'
# define RSS "\36"
# define BF_SZ 8192

long long	g_s;

typedef struct s_sh
{
	int			runing;
	char		*pwd;
	char		*odpwd;
	char		**env;
	char		*home;
	char		*user;
	char		**evpth;
	int			child;
	int			exe;
	int			*pip;
	char		***iof;
	int			stdi;
	int			stdo;
	int			ifd;
	int			ofd;
	long long	exit_c;
	int			pidf;
	int			prs;

	char		*bf;
	int			bf_inx;
	int			bf_sz;

	char		**bff;
	int			bff_inx;
	int			bff_sz;

	char		*cmd;
	char		**cs;

	int			quo;
	int			i;
	int			j;
	int			tokn;
	int			lp;
	int			rp;
}	t_sh;

//utils
char *read_line(t_sh *sh, int fd);

int			fprint(int fd, const char *s, ...);
char		*sjoin(char const *s1, char const *s2);
char		*sjoinf1(char const *s1, char const *s2);
char		*sjoinf2(char const *s1, char const *s2);
char		*sjoin2(int size, char **strs, char *sep);
int		len(const char *str);
int		len2(char **str);
int		len3(char ***css);
char		**ft_split(char const *s, char *c);
char	**split(char const *s, char *c);
char		*sdup(const char *src);
char		*sdupf(const char *src);
char	*sdupr(const char *src);
int			sncmp(const char *s1, const char *s2, int n);
long long	ati(const char *str);
int			is_aphnum(char c);
int			is_apha(char c);
int			all_sp(char *s);
int			is_in(char *s, char c);
char		*trm_sp(char *s);
//init
void init_bf(t_sh *sh);
void apend_bf(t_sh *sh, char c);
void apends_bf(t_sh *sh, char *s);

void		set_env(t_sh *shell);
void		set_env_r(t_sh *shell);
void		set_no_env(t_sh *sh);
void		set_envpth(t_sh *sh);
int			find_var(char **str, char *s);
void		set_pwd(t_sh *sh);
//signal
void		set_signal_b(void);
void		set_signal_a(void);
void		set_signal_exe(void);
void		rm_signal(void);
// void		sig_quit(int sig);
// void		sig_newline(int sig);
//prase quo
char *reload(t_sh *sh, char *cmd);
char *load_var(t_sh *sh, char *cmd);
int check(t_sh *sh, char *cmd);
int check_quo(int status, char c, int self);
int is_tokn(char *s);
char *load_var(t_sh *sh, char *cmd);
char *load_wikd(t_sh *sh, char *cmd);
char *repls_wikd(t_sh *sh, char *cmd);

char	trans(char c);
void sde_trans(char *c);
void sde_trans2(char **cs);
void dequo(char *c);

char		*quo(t_sh *sh, char *c);
char		**quo2(t_sh *sh, char **cs);
char		***quo3(t_sh *sh, char ***css);
char		***dequo3(char ***re);
void		isin_quo(char c, int *in_quo);
void		write_quo(char c, char *re, int *j, int in_quo);
int			write_dolor(char *c, t_sh *sh, char *re, int *j);
char		save_quo(char c);
char		load_quo(char c);
//prase io
char		is_io(char *s);
void		ld_io3(t_sh *sh, char **cs);
//split cmd
char		***all_cmd(t_sh *sh, char *s);
char		***splt3(char ***css);
//vaild cmd
// int			check(char *s, char ***css, t_sh *sh);
int			ck_start(char *cmd);
//exe
int exe_all(t_sh *sh, char *cmd, int fork);
int exe_and(t_sh *sh, char *cmd1, char *cmd2);
int exe_one(t_sh *sh, char *cmd, int fork);
int exe_or(t_sh *sh, char *cmd1, char *cmd2);
int exe_pip(t_sh *sh, char **cs);
int run_one(t_sh *sh, char **cs, int fork);
int trm_prth(char *s);

void		exe_cmd(t_sh *sh, char **cs);
// void		exe_all(t_sh *sh, char ***css);
// int		exe_pip(t_sh *sh, char ***css);
char		*get_pth(char *pwd, char **evpth, char *cs);
//buildin
void		run_echo(t_sh *sh, char **cs);
void		run_pwd(t_sh *sh, char **cs);
void		run_cd(t_sh *sh, char **cs);
void		run_env(t_sh *sh, char **cs);
void		env_append(t_sh *sh, char *cs);
void		env_delete(t_sh *sh, int i);
void		run_export(t_sh *sh, char **cs);
char		*prase_exp(char *cs);
int			valid_exp(char *s);
void		run_unset(t_sh *sh, char **cs);
void		run_exit(t_sh *sh, char **cs);
//redir
int		redir(t_sh *sh, char *cmd);
void		hir_doc(t_sh *sh, char *lmt, char m);
void		crt_hirdoc(t_sh *sh);
void		send_line(char *cs, int fd);
void		reset_io(t_sh *sh);
//pipe
void		set_pip(t_sh *sh);
void		close_pip(t_sh *sh, int j);
//free
void	free1(char *p);
void		free2(char **cs);
void		free3(char ***css);
char		*free_sh(t_sh *sh, int if_exit);
void		free_cmd(char *cmd, char ***css, t_sh *sh);
void		free_fail(t_sh *sh, char **cs);
//error
int			error_code(int ext);
//debug
// void		prnt(char ***css);

#endif