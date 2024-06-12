#include "shell.h"

char *read_line(t_sh *sh)
{
    char a;
    int re;

    a = 0;
    init_bf(sh);
    while (1) {
        re = read(0, &a, 1);
        if (re < 0)
            return 0;
        if (re == 0)
            return sdup("");
        if (a == '\n')
            return sdup(sh->bf);
        else
            apend_bf(sh, a);
    }
}
