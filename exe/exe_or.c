#include "shell.h"

int exe_or(t_sh *sh, char *cmd1, char *cmd2)
{
    sh->exit_c = exe_all(sh, sdup(cmd1), 0);
    if (!sh->exit_c)
    {
        free(cmd1);
        free(cmd2);
        return (0);
    }
    sh->exit_c = exe_all(sh, sdup(cmd2), 0);
    free(cmd1);
    free(cmd2);
    return (sh->exit_c);
}
