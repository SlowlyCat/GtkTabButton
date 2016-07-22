
#include <string.h>

#include "module.h"
#include "framework.h"

#define MODULE_NAME "framework"
#define MODULE_UI_FILE MODULE_NAME".ui"


static GtkBuilder * mBuilder = NULL;
static gchar * glAppPath =NULL;

static void afsWindowSetBackground(GtkWidget *widget, GdkPixbuf *pixbuf)
{
        GtkStyle *	style;
        GdkPixmap *	pixmap;
	GdkPixbuf *	psizedBuf;

	gint width, height;

	psizedBuf = gdk_pixbuf_scale_simple(pixbuf,widget->allocation.width,widget->allocation.height,GDK_INTERP_NEAREST);

	width = gdk_pixbuf_get_width(psizedBuf);
	height = gdk_pixbuf_get_height(psizedBuf);
	pixmap = gdk_pixmap_new(widget->window, width, height, 24);
	 
	gdk_pixbuf_render_to_drawable(psizedBuf,pixmap, 
		widget-> style-> fg_gc[GTK_STATE_NORMAL], 
		0,0,0,0, 
		-1, -1,
		GDK_RGB_DITHER_NONE,0,0); 

	style = gtk_style_copy(GTK_WIDGET (widget)->style);

	if (style->bg_pixmap[GTK_STATE_NORMAL])
		g_object_unref(style->bg_pixmap[GTK_STATE_NORMAL]);

	style->bg_pixmap[GTK_STATE_NORMAL] = g_object_ref(pixmap);
	style->bg_pixmap[GTK_STATE_ACTIVE] = g_object_ref(pixmap);
	style->bg_pixmap[GTK_STATE_PRELIGHT] = g_object_ref(pixmap);
	style->bg_pixmap[GTK_STATE_SELECTED] = g_object_ref(pixmap);
	style->bg_pixmap[GTK_STATE_INSENSITIVE] = g_object_ref(pixmap);

	gtk_widget_set_style(GTK_WIDGET (widget), style);
	g_object_unref(style);
	gdk_pixbuf_unref(psizedBuf);
}

static GdkPixbuf * ModuleRealPixbuf()
{
	GdkPixbuf *pbuf,*prealBuf;
	gchar * bgPath = g_strdup_printf("%s/res/bg.png",glAppPath);
	pbuf = gdk_pixbuf_new_from_file(bgPath ,NULL);
	g_free(bgPath);
	
	if(!pbuf)
	{
		MLOG("--fail --%s\n",bgPath);  
		return;
	}
	prealBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,gdk_pixbuf_get_bits_per_sample(pbuf),46,46);
	gdk_pixbuf_copy_area (pbuf,4,4,46,46,prealBuf,0,0);

	gdk_pixbuf_unref(pbuf);
	return prealBuf;
}


void ModuleSetBackgroud(GtkWidget *widget)
{

	GdkPixbuf *pbuf = ModuleRealPixbuf();
	afsWindowSetBackground(widget,pbuf);
	gdk_pixbuf_unref(pbuf);

}


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
		glAppPath = g_strdup(appPath);
		return (MODULE_NAME);
	}
	return NULL;
}

GtkWidget * ModuleReload()
{
	GtkWidget * window = GTK_WIDGET (gtk_builder_get_object (mBuilder, "MODULE"));
	GtkWidget * prjTreeView = GTK_WIDGET (gtk_builder_get_object (mBuilder, "PROJECTS"));
	GtkWidget * info = GTK_WIDGET (gtk_builder_get_object (mBuilder, "INFO"));
	GtkWidget * status = GTK_WIDGET (gtk_builder_get_object (mBuilder, "STATUS"));
	GtkWidget * detailNote = GTK_WIDGET (gtk_builder_get_object (mBuilder, "DETAIL"));

	if(prjTreeView && info && status && detailNote && window)
	{
		ProjectsInit(prjTreeView);
		InfoInit(info);
		StatusInit(status);
		g_signal_connect(detailNote, "switch-page",  G_CALLBACK(onPageSwitched), NULL);

		ModuleSetBackgroud(window);
		return window;
		
	}

	return NULL;
}

GtkBuilder * ModuleGetBuilder()
{
	return mBuilder;
}
	


