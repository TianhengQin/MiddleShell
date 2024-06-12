#include "shell.h"

int exe_or(t_sh *sh, char *cmd1, char *cmd2)
{
    sh->exit_c = exe_all(sh, sdup(cmd1));
    if (!sh->exit_c)
    {
        return (0);
    }
    sh->exit_c = exe_all(sh, sdup(cmd2));
    free(cmd1);
    free(cmd2);
    return (sh->exit_c);
}
