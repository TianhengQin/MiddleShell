#include "shell.h"

/*
0 arg
1 (
2 )
3 >
4 <
5 |
6 &&
7 ||
8 >>
9 <<
10 2>
11 2>>
*/

int mrge_tokn(int tokn)
{
    if (tokn == 10)
        return (3);
    if (tokn == 11)
        return (8);
    return (tokn);
}

int val_tokn(int tokn, int prev, t_sh *sh)
{
    if (tokn == 1)
        ++sh->lp;
    else if (tokn == 2)
        ++sh->rp;
    if (sh->lp < sh->rp)
        return 1;
    if (tokn == 0 && prev == 2)
        return 1;
    if (tokn == 1 && (prev == 0 || (prev > 1 && prev < 5) || prev > 7))
        return 1;
    if (tokn == 2 && (prev == -1 || prev == 1 || prev > 2))
        return 1;
    if ((tokn == 3 || tokn == 4 || tokn == 8 || tokn == 9) && (prev == 3 || prev == 4 || prev == 8 || prev == 9))
        return 1;
    if (tokn == 5 && (prev == -1 || prev == 1 || prev > 2))
        return 1;
    if (tokn == 6 && (prev == -1 || prev == 1 || prev > 2))
        return 1;
    if (tokn == 7 && (prev == -1 || prev == 1 || prev > 2))
        return 1;
    return 0;
}

void here_doc(t_sh *sh, char *dlm)
{
    int hd;
    char *line;

	hd = open(sh->hirdoc, O_CREAT | O_WRONLY | O_TRUNC, 00644);
    if (hd < 0)
    {
        perror("heredoc:");
        return ;
    }
    // printf("dlm %s\n", dlm);
    while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			write(1,"\n",1);
			break ;
		}
		if (sncmp(line, dlm, 2147483647) == 0)
		{
			free(line);
			break ;
		}
		write(hd, line, len(line));
        write(hd, "\n", 1);
		free(line);
	}
    close(hd);
    (sh->hirdoc)[10] = (sh->hirdoc)[10] + 1;
}

int check(t_sh *sh, char *cmd)
{
    char **cmds;
    int i;
    int tokn;

    reload(sh, cmd);
    cmds = ft_split(sh->bf, RSS);
    if (!cmds)
        free_sh(sh, 2);
    i = -1;
    sh->tokn = -1;
    sh->lp = 0;
    sh->rp = 0;
    sh->hirdoc = sdup(HERE_DOC);
    if (!sh->hirdoc)
        free_sh(sh, 2);
    while (cmds[++i])
    {
        // fprint(1, "[%s]",cmds[i]);
        tokn = mrge_tokn(is_tokn(cmds[i]));
        if (val_tokn(tokn, sh->tokn, sh))
        {
            fprint(2, "minish: parse error near `%s'\n", cmds[i]);
            sh->exit_c = 2;
            free2(cmds);
            return (0);
        }
        if (sh->tokn == 9)
            here_doc(sh, cmds[i]);
        sh->tokn = tokn;
    }
    if (sh->lp > sh->rp || tokn > 2)
    {
        fprint(2, "minish: parse error near `\\n'\n");
        free2(cmds);
        sh->exit_c = 2;
        return (0);
    }
    // fprint(1, "\n");
    free2(cmds);
    return (1);
}
