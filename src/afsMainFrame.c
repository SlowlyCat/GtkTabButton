#include <gtk/gtk.h>
#include "afs.h"
#include "env.h"
#include "TabButton.h"
#include "ScrolledWindow.h"
#include "modules/module.h"
#ifdef ENABLE_NLS
#  include <libintl.h>
#endif



#define UI_FILE "res/mainFrame.ui"
extern AfsFrame glAfsFrame ;


#define LOOP_IN	"PB_In%05d.tga"
#define LOOP_IN_OFFSET	11
#define LOOP_IN_END	39

#define LOOP	"PB_Loop%05d.tga"
#define LOOP_OFFSET	40
#define LOOP_END	124

#define LOOP_OUT	"PB_Out%05d.tga"
#define LOOP_OUT_OFFSET	221
#define LOOP_OUT_END	257

typedef struct 
{
	gchar * text;
	gint  icon;
}TabButtonInfo;

static GdkPixbuf * afsRealPixbuf();




static  gboolean afsMiddleAnim	(gpointer	   data)
{
	static gint frame = LOOP_IN_OFFSET;
	GtkImage* image = GTK_IMAGE(data);
	if(image)
	{
		gchar * filename = NULL;
		if(frame  <= LOOP_IN_END)
		{
			filename = g_strdup_printf("%s/res/processing/"LOOP_IN,glAfsFrame.afsPath,frame);
			frame++;
		}
		else if(frame  <= LOOP_END)
		{
			//frame = LOOP_OFFSET;
			//LOG("Frame =%d",frame);
			filename = g_strdup_printf("%s/res/processing/"LOOP,glAfsFrame.afsPath,frame);
			frame++;
			if(frame  > LOOP_END)
			{
				frame = LOOP_OFFSET;//LOOP_OUT_OFFSET
			}
		}
		
		else /*should never out ?*/
		{
			filename = g_strdup_printf("%s/res/processing/"LOOP_OUT,glAfsFrame.afsPath,frame);
			frame++;
		}
		
		if(frame > LOOP_OUT_END)
		{
			frame = LOOP_IN_OFFSET;
		}
		filename =g_strdup_printf("%s/res/""Android_Logo_Wallpaper.jpg",glAfsFrame.afsPath);
		LOG("filename =%s",filename);
		GdkPixbuf * pbuf = gdk_pixbuf_new_from_file_at_scale(filename,-1,100,TRUE,NULL);
		GdkPixbuf * pbgbuf = gdk_pixbuf_rotate_simple(pbuf,GDK_PIXBUF_ROTATE_UPSIDEDOWN);
	
		gtk_image_set_from_pixbuf(image,pbgbuf);
		g_free(filename);
		g_object_unref(pbuf);
		g_object_unref(pbgbuf);
	}
	return FALSE;
}

static GdkPixbuf * afsRealPixbuf()
{
	GdkPixbuf *pbuf,*prealBuf;
	gchar * bgPath = g_strdup_printf("%s/res/bg.png",glAfsFrame.afsPath);
	pbuf = gdk_pixbuf_new_from_file(bgPath ,NULL);
	g_free(bgPath);
	
	if(!pbuf)
	{
		LOG("--fail --%s\n",bgPath);  
		return;
	}
	prealBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,gdk_pixbuf_get_bits_per_sample(pbuf),46,46);
	gdk_pixbuf_copy_area (pbuf,4,4,46,46,prealBuf,0,0);

	gdk_pixbuf_unref(pbuf);
	return prealBuf;
}


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



static void afsMainWindowBackground(GtkWidget *widget)
{
	GdkPixbuf *pbuf = afsRealPixbuf();
	afsWindowSetBackground(widget,pbuf);
	gdk_pixbuf_unref(pbuf);

}

static void TabRemoveChild(gpointer child,gpointer data)
{
	GtkContainer * container = GTK_CONTAINER(data);
	gtk_container_remove(container,GTK_WIDGET(child));
}
static void TabLoad(GtkWidget * wid)
{
	//GtkWidget * window = GTK_WIDGET (gtk_builder_get_object (glAfsFrame.afsBuilder, "FRAME"));
	if(wid)
	{
		GList * children = gtk_container_get_children(GTK_CONTAINER(glAfsFrame.afsContainer));
		g_list_foreach(children ,TabRemoveChild,(gpointer) (glAfsFrame.afsContainer));
		gtk_container_add(GTK_CONTAINER(glAfsFrame.afsContainer),wid);
		//gtk_viewport_add(GtkContainer * container,GtkWidget * child)
		//gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(glAfsFrame.afsContainer),wid);
	}
}

static void TabToggled(GtkToggleButton *togglebutton, gpointer  user_data)     
{
	if(gtk_toggle_button_get_active(togglebutton))
	{
		LOG("change to %d",GPOINTER_TO_INT(user_data));
		
		switch(GPOINTER_TO_INT(user_data))
		{
			case 0:/*framework*/
				TabLoad( afsGetModules("framework"));
				break;
			case 1:/*log*/
				TabLoad( afsGetModules("log"));
				break;
			default:
				break;
		}
	}
	else
	{
		LOG("change from %d",GPOINTER_TO_INT(user_data));
	}
		
}
 
void* afsMainWindow ()
{
	GtkWidget *window = NULL;
	GtkWidget *widget = NULL;
	GtkVBox * vBoxMain = NULL;
	GtkHBox * vBoxLeft = NULL;
	GtkHBox * vBoxRight = NULL;
	GtkWidget * image;
	GError* error = NULL;
	GtkTabButton * btn = NULL;
	gchar * uiPath = g_strdup_printf("%s/"UI_FILE,glAfsFrame.afsPath);
	if(uiPath)
	{
		if (!gtk_builder_add_from_file (glAfsFrame.afsBuilder, uiPath, &error))
		{
			g_warning ("Couldn't load builder file: %s", error->message);
			g_error_free (error);
		}
		g_free(uiPath);
	}
	window = GTK_WIDGET (gtk_builder_get_object (glAfsFrame.afsBuilder, "main_window"));
	gtk_window_set_title(GTK_WINDOW(window),"hello");
	afsMainWindowBackground(window);
	
/*	frame = GTK_WIDGET (gtk_builder_get_object (glAfsFrame.afsBuilder, "FRAME"));
	if(frame)
	{
		//LOG("Set Back!");
		afsWidgetSetBackground(frame);

	}
*/
	vBoxMain=GTK_VBOX(gtk_builder_get_object (glAfsFrame.afsBuilder, "BOX_MAIN"));
	if(vBoxMain)
	{
		glAfsFrame.afsContainer=gtk_frame_window_new();
		gtk_container_set_border_width(GTK_CONTAINER(glAfsFrame.afsContainer),8);
		gtk_box_pack_end(GTK_BOX(vBoxMain),GTK_WIDGET(glAfsFrame.afsContainer),1,1,0);
	}
	vBoxLeft=GTK_HBOX(gtk_builder_get_object (glAfsFrame.afsBuilder, "BOX_LEFT"));
	vBoxRight=GTK_HBOX(gtk_builder_get_object (glAfsFrame.afsBuilder, "BOX_RIGHT"));
	GSList* group = NULL;//g_malloc0(sizeof(GSList));
	if(vBoxLeft && vBoxRight)
	{
		gint i = 0;
		const TabButtonInfo bInfo[]=
		{
			{"Framework",ICON_FRAMEWORK},
			{"Log",ICON_LOG},
			{"Emulator",ICON_EMULATOR},
			{"Sync",ICON_SYNC},
			{"Tools",ICON_TOOL},
			{"Terminal",ICON_TERMINAL},
			{"Document",ICON_DOCUMENT},
			{"About",ICON_ABOUT},
		};
		gint SIZE = sizeof(bInfo)/sizeof(TabButtonInfo);
		for(i =0; i< SIZE; i++)
		{
			btn = GTK_FLIP_BUTTON(gtk_tab_button_new_with_label(bInfo[i].text));
			gtk_tab_button_set_icon(btn,bInfo[i].icon);
			gtk_radio_button_set_group(GTK_RADIO_BUTTON(btn),group);
			group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(btn));
			if(i<SIZE/2)
			{
				gtk_box_pack_start(GTK_BOX(vBoxLeft),GTK_WIDGET(btn),1,1,0);
			}
			else
			{
				gtk_box_pack_start(GTK_BOX(vBoxRight),GTK_WIDGET(btn),1,1,0);
			}
			g_signal_connect(G_OBJECT(btn),"toggled",*(GCallback)TabToggled,GINT_TO_POINTER(i));
			if(i == 0)
			{
				gtk_toggle_button_toggled(GTK_TOGGLE_BUTTON(btn));
			}
		}
		
	}
	
	
	

	
	/* This is important */
	gtk_builder_connect_signals (glAfsFrame.afsBuilder, NULL);

	
	gtk_widget_show_all(window);
	image = GTK_WIDGET(gtk_builder_get_object (glAfsFrame.afsBuilder, "IMAGE"));
	if(image)
	{
		afsMainWindowBackground(image);
		gtk_timeout_add(1000/15/*FPS*/,afsMiddleAnim,image);
	}
	widget =  GTK_WIDGET(gtk_builder_get_object (glAfsFrame.afsBuilder, "MENUBAR"));
	if(widget)
	{
		afsMainWindowBackground(widget);
	}
}


#if 0
gboolean afsMenuExpose(GtkWidget *widget,GdkEventExpose *event,gpointer user_data)
{
	LOG("");
	//GtkWidget *child = GTK_CONTAINER (widget)->child;
	//gtk_menu_bar_expose(GtkWidget * widget,GdkEventExpose * event)
	//gtk_container_propagate_expose (GTK_CONTAINER (widget), child, event);
	/*GdkPixbuf *pbuf = afsRealPixbuf();
	GdkPixbuf *psizedBuf = gdk_pixbuf_scale_simple(pbuf,widget->allocation.width,widget->allocation.height,GDK_INTERP_NEAREST);
	
	
	gdk_draw_pixbuf(widget ->window,
		widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		psizedBuf,
		0,0,
		widget->allocation.x,widget->allocation.y,
		-1,-1,
		GDK_RGB_DITHER_NORMAL,1,1);
	gdk_pixbuf_unref(pbuf);
	gdk_pixbuf_unref(psizedBuf);
	*/
	//GTK_MENU_BAR_GET_CLASS(widget)->parent_class->expose_event(widget,event);
	//afsMainWindowBackground(widget);
	GTK_WIDGET_GET_CLASS(widget)->expose_event(widget,event);

	//
	return TRUE;
}
#endif

//

