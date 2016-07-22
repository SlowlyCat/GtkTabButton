
#ifndef _FRAMEWORK_MODULE_H_
#define _FRAMEWORK_MODULE_H_

#include <glib.h>
#include <gtk/gtk.h>

#define GIT_SUFFIX ".git"
#define GIT_ROOT ".repo/projects/"
#define SOURCE_ROOT "/android/"



void ProjectsInit(GtkWidget * treeview);
void  onSelectChanged(GtkWidget *widget, gpointer data);

void InfoInit(GtkWidget * textView);
void InfoRefresh( gchar * git);

void StatusInit(GtkWidget * treeview);

void onPageSwitched(GtkNotebook     *notebook, GtkNotebookPage *page,guint page_num, gpointer user_data);

const gchar * PathGetEntry();
gpointer StatusRefresh(gpointer path);


GtkBuilder * ModuleGetBuilder();
void ModuleSetBackgroud(GtkWidget *widget);


#endif
