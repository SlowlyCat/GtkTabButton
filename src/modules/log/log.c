
#include <glib.h>
#include <gtk/gtk.h>

#include "module.h"

#define MODULE_NAME "log"
#define MODULE_UI_FILE MODULE_NAME".ui"


static GtkBuilder * mBuilder = NULL;

gchar * ModuleRegister(const gchar * appPath)
{
	gchar * uiPath = g_strdup_printf("%s/modules/res/"MODULE_UI_FILE,appPath);
	mBuilder = gtk_builder_new ();
	if(uiPath && mBuilder)
	{
		guint ret = gtk_builder_add_from_file (mBuilder, uiPath, NULL);
		g_free(uiPath);
		if(ret == 0)
		{
			MLOG ("Couldn't load builder file: %d",ret);
			return NULL;
		}
		return (MODULE_NAME);
	}
	return NULL;
}
 
GtkWidget * ModuleReload()
{
	GtkWidget * window = GTK_WIDGET (gtk_builder_get_object (mBuilder, "MODULE"));
	if(window)
	{
		return window;
	}
	return NULL;
}

