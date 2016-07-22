

#include "module.h"
#include "framework.h"



enum
{
        BASENAME_COLUMN = 0,
        FULLNAME_COLUMN,
        NUM_COLS
} ;

static GtkTreeStore *glTreeStore = NULL;



static void ProjectsLoad(const gchar * path,GtkTreeIter * parent)
{
	GtkTreeIter child;
	GDir * dir = g_dir_open(path,0,NULL);
	const gchar * filename = NULL;
	gchar * fullFilename = NULL;
	
	if(dir)
	{
		while(NULL != (filename = g_dir_read_name(dir)))
		{
			
			fullFilename = g_strdup_printf("%s/%s",path,filename);
			if(g_file_test(fullFilename,G_FILE_TEST_IS_DIR) )
			{
				MLOG("%s",fullFilename);
				if(g_strcmp0(".",filename)==0 ||g_strcmp0("..",filename)==0 )
				{
					continue;
				}
				else if(g_strrstr(g_basename(filename),GIT_SUFFIX))
				{
					gtk_tree_store_append(glTreeStore, &child, parent);
					gtk_tree_store_set(glTreeStore, &child, 
						BASENAME_COLUMN, filename,
						FULLNAME_COLUMN, (fullFilename),-1);
					continue;
				}
				else
				{
					gtk_tree_store_append(glTreeStore, &child, parent);
					gtk_tree_store_set(glTreeStore, &child, 
						BASENAME_COLUMN, filename,
						FULLNAME_COLUMN, (fullFilename),-1);
					ProjectsLoad(fullFilename,&child);
				}
			}
			g_free(fullFilename);
		}
		g_dir_close(dir);
	}
		
}

static gint SortFunc(GtkTreeModel *model,
                     GtkTreeIter  *a,
                     GtkTreeIter  *b,
                     gpointer      user_data)
{
        gchar *name_a, *name_b;
        int ret;
        /* We need this function because we want to sort
         * folders before files.
         */
        gtk_tree_model_get(model, a,
                           0/*key*/, &name_a,
                           -1);
        gtk_tree_model_get(model, b,
                           0/*key*/, &name_b,
                           -1);
        ret = g_utf8_collate(name_a, name_b);
        g_free(name_a);
        g_free(name_b);
        return ret;
}

void onPageSwitched(GtkNotebook     *notebook, GtkNotebookPage *page,guint page_num, gpointer user_data)
{
	const gchar * path = PathGetEntry();
	
}


void ProjectsInit(GtkWidget * treeview)
{
	glTreeStore = gtk_tree_store_new(NUM_COLS,G_TYPE_STRING,G_TYPE_STRING);
		
	gtk_tree_sortable_set_default_sort_func(GTK_TREE_SORTABLE(glTreeStore),SortFunc,NULL, NULL);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(glTreeStore),
			GTK_TREE_SORTABLE_DEFAULT_SORT_COLUMN_ID,
			GTK_SORT_ASCENDING);
		
	GtkTreeSelection *selection;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *col;
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Projects:");
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer,
				"text", BASENAME_COLUMN);
				
	ProjectsLoad(SOURCE_ROOT GIT_ROOT,NULL);
		
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(glTreeStore));
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	g_signal_connect(selection, "changed",  G_CALLBACK(onSelectChanged), NULL);

	
	return;
	
}


const gchar * PathGetEntry()
{
	GtkBuilder * builder = ModuleGetBuilder();
	GtkEntry * entry = GTK_ENTRY(gtk_builder_get_object (builder, "ENTRY"));
	if(entry)
	{
		return gtk_entry_get_text(entry);
	}
	return NULL;
	
}

static  void PathSetEntry(const gchar * gitPath)
{
	GtkBuilder * builder = ModuleGetBuilder();
	GtkEntry * entry = GTK_ENTRY(gtk_builder_get_object (builder, "ENTRY"));
	gchar * realPath = NULL;
	gchar * ind =NULL;
	gchar *  indexer = g_strrstr(gitPath,GIT_ROOT);
	indexer += strlen(GIT_ROOT)+1;
	realPath =  g_strdup_printf("%s%s",SOURCE_ROOT,indexer);
	
	ind =  g_strrstr(realPath,GIT_SUFFIX);
	if(ind)
	{
		ind[0] ='\0';
	}
	MLOG("realPath = %s",realPath);
	gtk_entry_set_text(entry, realPath);
	g_free(realPath);
}

void  onSelectChanged(GtkWidget *widget, gpointer data)
{
	GtkBuilder * builder = ModuleGetBuilder();
	GtkWidget * detailNote = GTK_WIDGET (gtk_builder_get_object (builder, "DETAIL"));
	gint currentPage = gtk_notebook_get_current_page(GTK_NOTEBOOK(detailNote));
	if(detailNote)
	{
		GtkTreeIter iter;
		GtkTreeModel *model;
		gchar *project = NULL;
		MLOG("");
		if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter))
		{
			gtk_tree_model_get(model, &iter, FULLNAME_COLUMN, &project, -1);
			PathSetEntry(project);
			MLOG("current Page =%d",currentPage);
			switch(currentPage)
			{
				case 0:
					InfoRefresh(project);
					break;
				case 1:
					g_thread_create(StatusRefresh,(gpointer)PathGetEntry(),FALSE,NULL);
					break;
				default:
					break;
			}
		}
	}
}

