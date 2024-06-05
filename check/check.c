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
    char *rcmd = reload(sh, cmd);
    char **cmds = ft_split(rcmd, "\20");
    sh->bff = ft_split(sh->bf, "\20");

int i = -1;
while(sh->bff[++i]){
    fprint(1, "[%s]",sh->bff[i]);
}
    fprint(1, "\n");


    if (!cmds)
        return (char **)free_sh(sh, 2);

    i = -1;
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
            free(rcmd);
            return (0);
        }
        sh->tokn = tokn;
    }
    // fprint(1, "\n");
    if (sh->lp > sh->rp || tokn > 2)
    {
        fprint(2, "\nminish: parse error near `\\n'\n");
        free2(cmds);
        free(rcmd);
        sh->exit_c = 1;
        return (0);
    }
    free(rcmd);
    fprint(1, "\n");
    return (cmds);
}
