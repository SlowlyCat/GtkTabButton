#ifndef _FRAME_WINDOW_
#define _FRAME_WINDOW_
#include <gtk/gtk.h>

#define TYPE_FRAME_WINDOW	(gtk_frame_window_get_type())
#define GTK_FRAME_WINDOW(obj)	(GTK_CHECK_CAST(obj,TYPE_FRAME_WINDOW,GtkFrameWindow))

typedef struct _GtkFrameWindow GtkFrameWindow;
typedef struct _GtkFrameWindowClass GtkFrameWindowClass;

struct _GtkFrameWindow
{
	GtkScrolledWindow parent;
};

struct _GtkFrameWindowClass
{
	GtkScrolledWindowClass parent_class;
};

GtkType gtk_frame_window_get_type(void);
GtkWidget* gtk_frame_window_new(void);

#endif //__OUR_ITEM_H__
 



