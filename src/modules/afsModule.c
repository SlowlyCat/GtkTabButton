


#include <gtk/gtk.h>
#include <glib.h>
#include <dlfcn.h>
#include "env.h"




#define MODULE_PATH "%s/modules/"



const gchar *  ModuleRegisterFuncName = "ModuleRegister";
typedef gchar* (*ModuleRegisterFunc)(const gchar * resPath);

const gchar *  ModuleReloadFuncName = "ModuleReload";
typedef GtkWidget* (*ModuleReloadFunc)();

typedef struct
{
	gchar * name;
	ModuleReloadFunc loadFunc;
}ModuleInfoStruct;

static GList * glModules = NULL;

gboolean afsModulesInit()
{
	gchar * appPath = afsGetExcutablePath();
	GDir * dir;
	const gchar * libFile = NULL;
	gpointer libRef =NULL;
	LOG("appPath %s",appPath);
	if(appPath)
	{
		gchar * libPath = g_strdup_printf(MODULE_PATH,appPath);
		//g_free(appPath);
		if(!libPath)
		{
			return FALSE;
		}
		dir = g_dir_open(libPath,0,NULL);
		//g_free(libPath);
		if(!dir)
		{
			return FALSE;
		}
		//glModules = g_list_alloc();
		while(NULL != (libFile = g_dir_read_name( dir)))
		{
			
			gchar * libFullFile = g_strdup_printf("%s%s", libPath, libFile);

			libRef = dlopen(libFullFile,RTLD_LAZY|RTLD_GLOBAL );
			//LOG("exame %s[%p]  %d",libFullFile,libRef,g_file_test(libFullFile,G_FILE_TEST_IS_REGULAR));
			if(libRef)
			{
				ModuleRegisterFunc registerFunc = dlsym(libRef,ModuleRegisterFuncName);
				ModuleReloadFunc loadFunc =dlsym(libRef,ModuleReloadFuncName);
				LOG("found %s[%p,%p] ",libFullFile,registerFunc,loadFunc);
				if(registerFunc && loadFunc)
				{
					ModuleInfoStruct * moduleInfo =g_malloc0(sizeof(ModuleInfoStruct));
					moduleInfo->name =  g_strdup(registerFunc(appPath));
					LOG("moduleInfo.name =%s",moduleInfo->name);
					moduleInfo->loadFunc = loadFunc;
					if(moduleInfo->name)
					{
						glModules = g_list_append(glModules, (gpointer)moduleInfo);
					}
				}
				//dlclose(libRef);
			}
		}
		g_dir_close(dir);
		g_free(libPath);
	}
	return TRUE;
}


void afsPrintModules()
{
	GList * list = glModules;
	ModuleInfoStruct * moduleInfo;
	while(list)
	{
		moduleInfo = (ModuleInfoStruct * ) list->data;
		LOG("name =%s, Func = [%p]",moduleInfo->name,moduleInfo->loadFunc);
		list = list ->next;
	}
}

GtkWidget * afsGetModules(const gchar * name)
{
	GList * list = glModules;
	ModuleInfoStruct * moduleInfo;
	while(list)
	{
		moduleInfo = (ModuleInfoStruct * ) list->data;
		LOG("name =%s, Func = [%p]",moduleInfo->name,moduleInfo->loadFunc);
		if(g_strcasecmp(moduleInfo->name,name)==0)
		{
			//break;
			return moduleInfo->loadFunc();
		}
		list = list ->next;
	}
	return NULL;
}
