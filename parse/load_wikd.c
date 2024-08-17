#include "shell.h"

int is_wikd(char *cmd)
{
    int i = 0;

    while (cmd[i] && cmd[i] != RS )
    {
        if (cmd[i] == '\21' || cmd[i] == '\24')
            return (1);
        ++i;
    }
    return (0);
}

void    decode_quo(char *p, char *re, int i, int *j)
{
    if (p[i] == '\22')
        re[(*j)++] = '\'';
    else if (p[i] == '\23')
        re[(*j)++] =  '"';
    else
        re[(*j)++] =  p[i];
}

char *empt_arg(char *p)
{
    int i;
    int j;
    char *re;
    int quo;

    re = malloc(lenr(p) + 1);
    if (!re)
        return (0);
    i = -1;
    j = 0;
    quo = 0;
    while (p[++i] != RS)
    {
        quo = check_quo(quo, p[i], 10);
		if (quo >= 10)
			quo = quo - 10;
        else if (quo)
            re[j++] =  p[i];
		else
            decode_quo(p, re, i, &j);
    }
    re[j] = 0;
    return (re);
}

// typedef struct s_wikd
// {
//     int sIdx;
//     int pIdx;
//     int lastWildcardIdx;
//     int sBacktrackIdx;
//     int nextToWildcardIdx;
// }   t_wikd;

void    init_wikd(t_wikd *w)
{
    w->sIdx = 0;
    w->pIdx = 0;
    w->lastWildcardIdx = -1;
    w->sBacktrackIdx = -1;
    w->nextToWildcardIdx = -1;
}

void    wikd_move_both(t_wikd *w)
{
    ++(w->sIdx);
    ++(w->pIdx);
}

void    wikd_qm(t_wikd *w)
{
    ++(w->sIdx);
    ++(w->pIdx);
}

void    wikd_star(t_wikd *w)
{
    w->lastWildcardIdx = w->pIdx;
    w->nextToWildcardIdx = ++(w->pIdx);
    w->sBacktrackIdx = w->sIdx;
}

int match_wikd_all(char *s, char *p, int ls, int lp)
{
    t_wikd  w;

    init_wikd(&w);
    while (w.sIdx < ls)
    {
        if (w.pIdx < lp && (p[w.pIdx] == '\21' || p[w.pIdx] == s[w.sIdx]))
            wikd_qm(&w);
        else if (w.pIdx < lp && p[w.pIdx] == '\24')
            wikd_star(&w);
        else if (w.lastWildcardIdx == -1)
            return 0;
        else
        {
            w.pIdx = w.nextToWildcardIdx;
            w.sIdx = ++w.sBacktrackIdx;
        }
    }
    while (w.pIdx < lp)
    {
        if (p[w.pIdx] != '\24')
            return 0;
        w.pIdx++;
    }
    return (1);
}

int match_wikd(char *s, char *p)
{
    char *tmp;
    int lens;
    int lenp;
    int re;

    tmp = empt_arg(p);
    if (!tmp)
        return (2);
    lens = len(s);
    lenp = len(tmp);
    re = match_wikd_all(s, tmp, lens, lenp);
    free(tmp);
    return re;
}

int is_hiden(char *wikd)
{
    if (wikd[0] == '.')
        return (1);
    if (wikd[0] == '"' || wikd[0] == '\'')
    {
        if (wikd[1] == '.')
            return (1);
    }
    return (0);
}

void apend_no_wikd(t_sh *sh, char *wikd)
{
    wikd = sdupr(wikd);
    dequo(wikd);
    apends_bf(sh, wikd);
    apend_bf(sh, RS);
    free(wikd);
}

void apend_wikd(t_sh *sh, char *wikd)
{
    struct dirent *dent;
    DIR *dir;
    int match;

    sh->tokn = 0;
    dir = opendir(sh->pwd);
    if (dir == NULL)
        free_sh(sh, 2);
    dent = readdir(dir);
    while (dent)
    {
        match = match_wikd(dent->d_name, wikd);
        if (match == 2)
            free_sh(sh, 2);
        else if (match && (dent->d_name[0] != '.' || is_hiden(wikd)))
        {
            sh->tokn = 1;
            apends_bf(sh, dent->d_name);
            apend_bf(sh, RS);
        }
        dent = readdir(dir);
    }
    closedir(dir);
    if (!sh->tokn)
        apend_no_wikd(sh, wikd);
}

char *load_wikd(t_sh *sh, char *cmd)
{
	sh->i = 0;
	sh->j = 0;
	init_bf(sh);
	sh->quo = 0;
    apend_bf(sh, RS);
	while (cmd[++(sh->i)])
	{
		if (cmd[sh->i - 1] == RS && cmd[sh->i] != RS && is_wikd(cmd + sh->i))
        {
            apend_wikd(sh, cmd + sh->i);
            while (cmd[sh->i] != RS)
                ++(sh->i);
        }
        else
            apend_bf(sh, cmd[sh->i]);
	}
    free(cmd);
	return (sdup(sh->bf));
}
