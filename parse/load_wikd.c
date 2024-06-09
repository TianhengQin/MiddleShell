#include "shell.h"

int is_wikd(char *cmd)
{
    int i = 0;

    while (cmd[i] && cmd[i] != '^' )
    {
        if (cmd[i] == '\22' || cmd[i] == '\23')
            return (1);
        ++i;
    }
    return (0);
}

int match_wikd(char *s, char *p)
{
    int lens = len(s);
    int sIdx = 0, pIdx = 0, lastWildcardIdx = -1,
        sBacktrackIdx = -1, nextToWildcardIdx = -1;
    int lenp;
	lenp = 0;
	while (p[lenp] != '^')
		lenp++;
    while (sIdx < lens) {
        if (pIdx < lenp
            && (p[pIdx] == '\23' || p[pIdx] == s[sIdx])) {
            ++sIdx;
            ++pIdx;
        }
        else if (pIdx < lenp && p[pIdx] == '\22') {
            lastWildcardIdx = pIdx;
            nextToWildcardIdx = ++pIdx;
            sBacktrackIdx = sIdx;
        }
        else if (lastWildcardIdx == -1) {
            return 0;
        }
        else {
            pIdx = nextToWildcardIdx;
            sIdx = ++sBacktrackIdx;
        }
    }
    for (int i = pIdx; i < lenp; i++) {
        if (p[i] != '\22')
            return 0;
    }
    return 1;
}

void apend_wikd(t_sh *sh, char *wikd)
{
    struct dirent *dent;
    DIR *dir;

    sh->tokn = 0;
    dir = opendir(sh->pwd);
    if (dir == NULL)
        free_sh(sh, 2);
    dent = readdir(dir);
    dent = readdir(dir);
    dent = readdir(dir);
    while (dent)
    {
        if (match_wikd(dent->d_name, wikd))
        {
            sh->tokn = 1;
            apends_bf(sh, dent->d_name);
            apend_bf(sh, '^');
        }
        dent = readdir(dir);
    }
    closedir(dir);
    if (!sh->tokn)
    {
        apends_bf(sh, wikd);
        apend_bf(sh, '^');
    }
}

char *load_wikd(t_sh *sh, char *cmd)
{
	sh->i = 0;
	sh->j = 0;
	init_bf(sh);
	sh->quo = 0;
    apend_bf(sh, '^');
	while (cmd[++(sh->i)])
	{
		if (cmd[sh->i - 1] == '^' && cmd[sh->i] != '^' && is_wikd(cmd + sh->i))
        {
            apend_wikd(sh, cmd + sh->i);
            while (cmd[sh->i] != '^')
                ++(sh->i);
        }
        else
            apend_bf(sh, cmd[sh->i]);
	}
    free(cmd);
	return (sdup(sh->bf));
}
