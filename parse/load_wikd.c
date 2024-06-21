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

char *empt_arg(char *p)
{
    int i;
    int j;
    char *re;
    int quo;

    i = 0;
    while (p[i] != RS)
		i++;
    re = malloc(i + 1);
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
		{
            if (p[i] == '\22')
                re[j++] = '\'';
            else if (p[i] == '\23')
                re[j++] =  '"';
            else
                re[j++] =  p[i];
		}
    }
    re[j] = 0;
    return (re);
}

int match_wikd_all(char *s, char *p, int ls, int lp)
{
    int sIdx = 0, pIdx = 0, lastWildcardIdx = -1,
        sBacktrackIdx = -1, nextToWildcardIdx = -1;
    while (sIdx < ls)
    {
        if (pIdx < lp && (p[pIdx] == '\21' || p[pIdx] == s[sIdx]))
        {
            ++sIdx;
            ++pIdx;
        }
        else if (pIdx < lp && p[pIdx] == '\24')
        {
            lastWildcardIdx = pIdx;
            nextToWildcardIdx = ++pIdx;
            sBacktrackIdx = sIdx;
        }
        else if (lastWildcardIdx == -1)
            return 0;
        else
        {
            pIdx = nextToWildcardIdx;
            sIdx = ++sBacktrackIdx;
        }
    }
    while (pIdx < lp)
    {
        if (p[pIdx] != '\24')
            return 0;
        pIdx++;
    }
    return 1;
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
        // printf("%s ", dent->d_name);
        match = match_wikd(dent->d_name, wikd);
        if (match == 2)
            free_sh(sh, 2);
        else if (match && (dent->d_name[0] != '.' || wikd[0] == '.'))
        {
            // printf(" match");
            sh->tokn = 1;
            apends_bf(sh, dent->d_name);
            apend_bf(sh, RS);
        }
        // printf("\n");
        dent = readdir(dir);
    }
    closedir(dir);
    if (!sh->tokn)
    {
        // void dequo(char *c)
        wikd = sdupr(wikd);
        dequo(wikd);
        apends_bf(sh, wikd);
        apend_bf(sh, RS);
        free(wikd);
    }
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
