#include <string.h>

#include "module.h"
#include "framework.h"

enum
{
        TYPE_COLUMN = 0,
        ACTION_COLUMN,
        FILE_COLUMN,
        NUM_COLUMNS
} ;

static GtkTreeStore *glTreeStore = NULL;

#define STAGED_PREFIX "Changes to be committed:"
#define MODIFIED_PREFIX "Changed but not updated:"
#define UNTRACK_PREFIX "Untracked files:"
#define UNTRACK_PROMPT "use \"git add <file>...\" to include in what will be committed"

#define ACTION_MODIFY "modified:"
#define ACTION_DELETED "deleted:"
#define ACTION_NEW "new file:"

enum
{
	PREFIX_UNDEFINED = -1,
	PREFIX_STAGED,
	PREFIX_MODIFIED,
	PREFIX_UNTRACK
};

#define PARTTERN "^#\\s+(\\S[\\S|\\s]+):\\s+(\\S[\\S|\\s]+)\n"
#define UNTRACK_PARTTERN "^#\\s+(\\S[\\S|\\s]+)\n"
//#define PARTTERN "#[[:space:]+](\\S+)"
static gboolean GitRegrex(const gchar * line,gint prefix,gchar ** action, gchar **file)
{
	GRegex* regex;
	GMatchInfo *match_info;
	GError *error = NULL;
	*action = NULL;
	*file = NULL;
	MLOG("reg =%s prefix=%d",line,prefix);
	if(prefix == PREFIX_UNTRACK)
	{
		regex = g_regex_new(UNTRACK_PARTTERN, 0 , 0, NULL);
		if(regex)
		{
			if(g_regex_match(regex, line, 0, &match_info))
			{
				*action = g_strdup("Untracked:");
				*file = g_match_info_fetch(match_info, 1);
			}
			g_match_info_free(match_info);
			g_regex_unref(regex);
		}
	}
	else
	{
		regex = g_regex_new(PARTTERN, 0 , 0, NULL);
	
		if(regex)
		{
			if(g_regex_match(regex, line, 0, &match_info))
			{
				*action = g_match_info_fetch(match_info, 1);
				*file = g_match_info_fetch(match_info, 2);
			}
			g_match_info_free(match_info);
			g_regex_unref(regex);
		}
	}
	
	if(*action && * file)
	{
		MLOG("action = %s,file =%s",*action,*file);
		return TRUE;
	}
	return FALSE;


}

static void GitCmd(const gchar * path ,const gchar * cmd)
{
	FILE * fp =NULL;
	gchar buf[1024];
	GtkTreeIter iterPrefix;
	GtkTreeIter child;
	gchar *fullCmd = g_strdup_printf("git --work-tree=%s --git-dir=%s/.git %s",path,path,cmd);
	if(fullCmd)
	{
		
		gchar * action;//[64];
		gchar * file;//[1024];
		gint prefix = PREFIX_UNDEFINED;
		fp = popen(fullCmd,"r");
		if(fp)
		{
			while(fgets(buf, 1024,fp))
			{
				MLOG("line =%s",buf);
				
				if(g_strrstr(buf,STAGED_PREFIX))
				{
					gdk_threads_enter();
					gtk_tree_store_append(glTreeStore, &iterPrefix, NULL);
					gtk_tree_store_set(glTreeStore, &iterPrefix, 
						TYPE_COLUMN, ("Staged"),
						ACTION_COLUMN, "",
						FILE_COLUMN, "",-1);
					gdk_threads_leave(); 
					prefix = PREFIX_STAGED;
					MLOG("Add %d",PREFIX_STAGED);
					continue;
				}
				else if(g_strrstr(buf,MODIFIED_PREFIX))
				{
					gdk_threads_enter();
					gtk_tree_store_append(glTreeStore, &iterPrefix, NULL);
					gtk_tree_store_set(glTreeStore, &iterPrefix, 
						TYPE_COLUMN, "Modified",
						ACTION_COLUMN, "",
						FILE_COLUMN, "",-1);
					gdk_threads_leave(); 
					prefix = PREFIX_MODIFIED;
					MLOG("Add %d",PREFIX_MODIFIED);
					continue;
				}
				else if(g_strrstr(buf,UNTRACK_PREFIX))
				{
					gdk_threads_enter();
					gtk_tree_store_append(glTreeStore, &iterPrefix, NULL);
					gtk_tree_store_set(glTreeStore, &iterPrefix, 
						TYPE_COLUMN, "Untracked",
						ACTION_COLUMN, "",
						FILE_COLUMN, "",-1);
					gdk_threads_leave(); 
					prefix = PREFIX_UNTRACK;
					MLOG("Add %d",PREFIX_UNTRACK);
					continue;
				}
				
				else if(g_strrstr(buf,ACTION_MODIFY) ||g_strrstr(buf,ACTION_DELETED) ||g_strrstr(buf,ACTION_NEW))
				{
					//sscanf(buf,"#\t%s: %s",action,file);
					if(GitRegrex(buf,prefix,&action,&file))
					{
						gdk_threads_enter();
						gtk_tree_store_append(glTreeStore, &child, &iterPrefix);
						gtk_tree_store_set(glTreeStore, &child, 
							TYPE_COLUMN, "",
							ACTION_COLUMN, (action),
							FILE_COLUMN, (file),-1);
						gdk_threads_leave(); 
						MLOG("Add %s %s",action,file);
					}
				}
				else if(prefix == PREFIX_UNTRACK)
				{
					MLOG("HERE");
					if(!g_strrstr(buf,UNTRACK_PROMPT) && GitRegrex(buf,prefix,&action,&file))
					{
						gdk_threads_enter();
						gtk_tree_store_append(glTreeStore, &child, &iterPrefix);
						gtk_tree_store_set(glTreeStore, &child, 
							TYPE_COLUMN, "",
							ACTION_COLUMN, (action),
							FILE_COLUMN, (file),-1);
						gdk_threads_leave(); 
						MLOG("Add %s %s",action,file);						
					}
				}
				
			}
			pclose(fp); 
		}
		g_free(fullCmd);
	}
}

gpointer StatusRefresh(gpointer data)
{
	const gchar * path = (const gchar *)data;
	MLOG("Path = %s",path);
	
	
	GitCmd(path,"status");
	
	return NULL;
	
}

void StatusInit(GtkWidget * treeview)
{
	
	GtkTreeSelection *selection;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *col;
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Type");
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer,
				"text", TYPE_COLUMN);
				
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Status");
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer,
				"text", ACTION_COLUMN);
				
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "File");
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer,
				"text", FILE_COLUMN);
				
	glTreeStore = gtk_tree_store_new(NUM_COLUMNS,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
		
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(glTreeStore));
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	//g_signal_connect(selection, "changed",  G_CALLBACK(onSelectChanged), NULL);

	return;
}

