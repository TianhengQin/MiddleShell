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
*/

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
    if (tokn == 1 && (prev == 0 || (prev > 1 && prev < 6) || prev > 7))
        return 1;
    if (tokn == 2 && (prev == -1 || prev == 1 || prev > 2))
        return 1;
    if (tokn == 5 && (prev == -1 || prev == 1 || prev > 2))
        return 1;
    if (tokn == 6 && (prev == -1 || prev == 1 || prev > 2))
        return 1;
    if (tokn == 7 && (prev == -1 || prev == 1 || prev > 2))
        return 1;
    return 0;
}

char **check(t_sh *sh, char *cmd)
{
    reload(sh, cmd);
    char **cmds = ft_split(sh->bf, "\21");

    if (!cmds)
        return (char **)free_sh(sh, 2);

    int i = -1;
    int tokn;
    sh->tokn = -1;
    sh->lp = 0;
    sh->rp = 0;
    while (cmds[++i])
    {
        fprint(1, "[%s]",cmds[i]);
        tokn = is_tokn(cmds[i]);
        if (val_tokn(tokn, sh->tokn, sh))
        {
            fprint(2, "\nminish: parse error near `%s'\n", cmds[i]);
            free2(cmds);
            return (0);
        }
        sh->tokn = tokn;
    }
    if (sh->lp > sh->rp || tokn > 2)
    {
        fprint(2, "\nminish: parse error near `\\n'\n");
        free2(cmds);
        sh->exit_c = 1;
        return (0);
    }
    fprint(1, "\n");
    return (cmds);
}
