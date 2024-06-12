#include "shell.h"

int exe_pip(t_sh *sh, char **cs)
{
    int i = -1;
    int len = len2(cs) - 1;
    int pip[2 * len];
    
    i = 0;
    while (i < len)
    {
        pipe(&pip[2 * i]);
    }

    while (cs[++i])
    {
        sh->stdi = dup(0);
        sh->stdo = dup(1);
        exe_all(sh, sdup(cs[i]));
    }
    return 0;
}