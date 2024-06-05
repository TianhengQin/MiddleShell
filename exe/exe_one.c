
#include "shell.h"

char *trm_prthsp(char *s)
{
    int		i;
	char	*re;

	i = len(s) - 1;
	while (i >= 0 && is_in(" \t\v\f\r\n()", s[i]))
		s[i--] = 0;
	i = 0;
	while (s[i] && is_in(" \t\v\f\r\n()", s[i]))
		i++;
	re = sdup(&s[i]);
	free(s);
	return (re);
}

int exe_one(t_sh *sh, char *cmd) {

    (void)sh;
    cmd = trm_prthsp(cmd);
    printf("%s\n", cmd);
    return 0;
}

int exe_and(t_sh *sh, char *cmd1, char *cmd2) {
    (void)sh;
    sh->exit_c = exe_all(sh, cmd1);
    if (sh->exit_c)
    {
        return (sh->exit_c);
    }
    sh->exit_c = exe_all(sh, cmd2);
    return (sh->exit_c);
}

int exe_or(t_sh *sh, char *cmd1, char *cmd2) {
    (void)sh;
    sh->exit_c = exe_all(sh, cmd1);
    // if (!sh->exit_c)
    // {
    //     return (0);
    // }
    sh->exit_c = exe_all(sh, cmd2);
    return (sh->exit_c);
}

int split_token(char *cmd) {
    int i = 0;
    while (cmd[++i]) {
        if (cmd[i - 1] == '(')
        {
            while (cmd[i] != ')')
                ++i;
        }
        if (cmd[i] == '&' && cmd[i - 1] == '&') {
            cmd[i] = 0;
            cmd[i-1] = 0;
            return (10 * i + 1);
        }
        if (cmd[i] == '|' && cmd[i - 1] == '|') {
            cmd[i] = 0;
            cmd[i-1] = 0;
            return (10 * i + 2);
        }
    }
    return (0);
}

int exe_all(t_sh *sh, char *cmd) {


    (void)sh;

    int find;

    cmd = trm_prthsp(cmd);

    find = split_token(cmd);

    if (find % 10 == 1)
        sh->exit_c = exe_and(sh, sdup(cmd), sdup(cmd + find / 10 + 1));
    else if (find % 10 == 2)
        sh->exit_c = exe_or(sh, sdup(cmd), sdup(cmd + find / 10 + 1));
    else
        sh->exit_c = exe_one(sh, cmd);
    return (sh->exit_c);
}
