
#include "env.h"

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif


gchar * afsGetExcutablePath()
{
	int pid = getpid();
	gchar retPath[PATH_MAX] = {0,};
	//memset(retPath,0x00,PATH_MAX);
	gchar * cwd = g_strdup_printf("/proc/%d/exe",pid);
	ssize_t n =0;
	n = readlink(cwd,retPath,PATH_MAX);
	if(n > 0)
	{
		
		return g_path_get_dirname(retPath);
	}
	return g_strdup("/");
}

/*
void LOG()
{
	
}
*/
