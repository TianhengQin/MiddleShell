#include "shell.h"

void init_hbf(t_sh *sh)
{
    sh->hbf_inx = 0;
    sh->hbf[0] = 0;
}

void apend_hbf(t_sh *sh, char c)
{
    int i;
    char *nbf;

    if (sh->hbf_inx == sh->hbf_sz)
    {
        sh->hbf_sz *= 2;
        nbf = malloc(sh->hbf_sz + 1);
        if (!nbf)
            free_sh(sh, 2);
        i = -1;
        while (++i < sh->hbf_inx)
            nbf[i] = sh->hbf[i];
        nbf[i] = 0;
        free(sh->hbf);
        sh->hbf = nbf;
    }
    sh->hbf[sh->hbf_inx] = c;
    ++(sh->hbf_inx);
    sh->hbf[sh->hbf_inx] = 0;
}

char *read_hline(t_sh *sh, int fd)
{
    char a;
    int re;

    a = 0;
    init_hbf(sh);
    while (1) {
        re = read(fd, &a, 1);
        if (re < 0)
            return (0);
        if (re == 0 && sh->hbf_inx == 0)
            return (0);
        if (re == 0 || a == '\n')
            return (sdup(sh->hbf));
        else
            apend_hbf(sh, a);
    }
}
