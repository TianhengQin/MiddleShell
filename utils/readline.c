#include "shell.h"

char *read_line(t_sh *sh, int fd)
{
    char a;
    int re;

    a = 0;
    init_bf(sh);
    while (1) {
        re = read(fd, &a, 1);
        // printf("rad %d\n", re);
        if (re < 0)
            return (0);
        if (re == 0 && sh->bf_inx == 0)
            return (0);
        if (re == 0 || a == '\n')
            return (sdup(sh->bf));
        else
            apend_bf(sh, a);
    }
}
