#include <gtk/gtk.h>

#include "ScrolledWindow.h"

#include "env.h"
static gboolean gtk_frame_window_expose (GtkWidget  *widget,   GdkEventExpose *event);


G_DEFINE_TYPE (GtkFrameWindow, gtk_frame_window, GTK_TYPE_SCROLLED_WINDOW)


static void gtk_frame_window_init(GtkFrameWindow *frame_window)
{
	
}

static void gtk_frame_window_class_init(GtkFrameWindowClass *framewindowclass)
{
	GtkObjectClass *object_class;	
	GtkWidgetClass *widget_class;
	  
	object_class = (GtkObjectClass*)framewindowclass;
	widget_class = (GtkWidgetClass*) framewindowclass;

	widget_class->expose_event = gtk_frame_window_expose;

}



static void
_gtk_frame_window_paint (GtkWidget    *widget,GdkRectangle *area)
{
	GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW (widget);

	gint width, height;
	gint x, y;
	gint border_width = 0;//GTK_CONTAINER (widget)->border_width;

	x = widget->allocation.x + border_width;
	y = widget->allocation.y + border_width;
	width = widget->allocation.width - border_width * 2;
	height = widget->allocation.height - border_width * 2;

	GdkPixbuf *pbuf,*prealBuf,*psizedBuf;
	gchar * path = afsGetExcutablePath();
	gchar * bg = g_strdup_printf("%s/res/widgets/bg.png",path);
	pbuf = gdk_pixbuf_new_from_file(bg ,NULL);
	g_free(bg);
	g_free(path);
	
	if(!pbuf)
	{
		g_print("fail\n");  
		return;
	}
	
	psizedBuf = gdk_pixbuf_scale_simple(pbuf,width,height,GDK_INTERP_HYPER);
		
	gdk_draw_pixbuf(widget->window,
		widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		psizedBuf,
		0,0,
		widget->allocation.x,widget->allocation.y,
		width,height,
		GDK_RGB_DITHER_NORMAL,1,1);
	gdk_pixbuf_unref(pbuf);
	gdk_pixbuf_unref(psizedBuf);	
}


static gboolean gtk_frame_window_expose (GtkWidget  *widget,   GdkEventExpose *event)
{
	if (gtk_widget_is_drawable (widget))
	{
		_gtk_frame_window_paint (widget, &event->area);	
		GTK_WIDGET_CLASS (gtk_frame_window_parent_class)->expose_event (widget, event);
	}
	
	return FALSE;
}

GtkWidget* gtk_frame_window_new(void)
{
	GtkWidget * widget = GTK_WIDGET(g_object_new(TYPE_FRAME_WINDOW,0));
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(widget),GTK_POLICY_NEVER ,GTK_POLICY_AUTOMATIC);
	return widget;
}


