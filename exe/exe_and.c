#include "shell.h"

int exe_and(t_sh *sh, char *cmd1, char *cmd2)
{
    // printf("--- and: %s --- %s\n",cmd1, cmd2);
    sh->exit_c = exe_all(sh, sdup(cmd1), 0);
    free(cmd1);
    if (sh->exit_c)
    {
        free(cmd2);
        return (sh->exit_c);
    }
    sh->exit_c = exe_all(sh, sdup(cmd2), 0);
    free(cmd2);
    return (sh->exit_c);
}
