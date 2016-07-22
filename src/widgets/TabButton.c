#include <gtk/gtk.h>
//#include <gdk/gdkkeysyms.h>
#include "TabButton.h"

#include "env.h"

const gchar * icons[]=
{
	"framework.png",
	"log.png",
	"emulator.png",
	"sync.png",
	"tool.png",
	"terminal.png",
	"document.png",
	"about.png",
	"home.png",
};


static GSList * group;
static void gtk_tab_button_init(GtkTabButton *tabbutton);
static void gtk_tab_button_class_init(GtkTabButtonClass *tabbuttonclass);
static void _gtk_tab_button_paint (GtkTabButton          *button,
		   const GdkRectangle *area,
		   GtkStateType        state_type,
		   GtkShadowType       shadow_type,
		   const gchar        *main_detail,
		   const gchar        *default_detail);
		   

static gboolean gtk_tab_button_expose (GtkWidget  *widget,   GdkEventExpose *event);
#if 0
static gboolean gtk_tab_button_leave_notify (GtkWidget        *widget , GdkEventCrossing *event);
static gboolean gtk_tab_button_enter_notify (GtkWidget        *widget , GdkEventCrossing *event);
static gboolean gtk_tab_button_button_press (GtkWidget      *widget, GdkEventButton *event);
static gboolean gtk_tab_button_button_release (GtkWidget        *widget , GdkEventButton *event);
static void gtk_tab_button_clicked (GtkButton *button);
#endif
static void gtk_tab_button_pressed (GtkButton *button);



G_DEFINE_TYPE (GtkTabButton, gtk_tab_button, GTK_TYPE_RADIO_BUTTON)

/*GtkType	gtk_tab_button_get_type(void)
{
	static GtkType tab_button_type = 0;
	if(!tab_button_type)
	{
		GtkTypeInfo tab_button_info =
		{
			"TabButton",
			sizeof(GtkTabButton),
			sizeof(GtkTabButtonClass),
			(GtkClassInitFunc)gtk_tab_button_class_init,
			(GtkObjectInitFunc)gtk_tab_button_init,
			NULL,
			NULL,
			(GtkClassInitFunc) NULL
		};
		tab_button_type = gtk_type_unique(GTK_TYPE_VBOX, &tab_button_info);
	}
	return tab_button_type;
}
*/

static void gtk_tab_button_init(GtkTabButton *tab_button)
{
	GtkVBox * vBox = GTK_VBOX(gtk_vbox_new(FALSE,0));
	gtk_container_add(GTK_CONTAINER(tab_button),GTK_WIDGET(vBox));
	
	tab_button->icon = GTK_IMAGE(gtk_image_new());
	gtk_box_pack_start(GTK_BOX(vBox),GTK_WIDGET(tab_button->icon),TRUE,TRUE,2);

	tab_button->label = GTK_LABEL(gtk_label_new("Init"));
	gtk_box_pack_start(GTK_BOX(vBox),GTK_WIDGET(tab_button->label),FALSE,FALSE,2);

	//gtk_radio_button_set_group(GTK_RADIO_BUTTON(tab_button),group);
	
	//gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(tab_button),FALSE);
	//gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tab_button),FALSE);
	GTK_TOGGLE_BUTTON (tab_button)->active = TRUE;

	//GTK_BUTTON (tab_button)->depress_on_activate = FALSE;

  	GTK_RADIO_BUTTON(tab_button)->group = g_slist_prepend (NULL, tab_button);
	//gtk_button_set_depressed (GTK_BUTTON (tab_button), TRUE);
	gtk_widget_set_state (GTK_WIDGET (tab_button), GTK_STATE_ACTIVE);

}

static void gtk_tab_button_class_init(GtkTabButtonClass *tabbuttonclass)
{
	GtkObjectClass *object_class;	
	GtkWidgetClass *widget_class;
	GtkButtonClass *button_class;
	  
	object_class = (GtkObjectClass*)tabbuttonclass;
	widget_class = (GtkWidgetClass*) tabbuttonclass;
	button_class = (GtkButtonClass*) tabbuttonclass;

					
	//widget_class->button_press_event = gtk_tab_button_button_press;
	//widget_class->button_release_event = gtk_tab_button_button_release;
	//widget_class->enter_notify_event = gtk_tab_button_enter_notify;
	//widget_class->leave_notify_event = gtk_tab_button_leave_notify;	
	widget_class->expose_event = gtk_tab_button_expose;
	
	//button_class->clicked = gtk_tab_button_clicked;
	 button_class->pressed = gtk_tab_button_pressed;
}


GtkWidget* gtk_tab_button_new(void)
{
	return GTK_WIDGET(g_object_new(TYPE_FLIP_BUTTON,0));
}

void gtk_tab_button_set_label(GtkTabButton* item, gchar* label)
{
	gtk_label_set_text(GTK_LABEL(item->label),label);
}

void gtk_tab_button_set_icon_from_file(GtkTabButton* item, gchar* icon)
{
	GdkPixbuf *pbuf,*prealBuf;
	pbuf = gdk_pixbuf_new_from_file(icon ,NULL);
	if(!pbuf)
	{
		g_print("fail\n");  
		return;
	}
	
	prealBuf = gdk_pixbuf_scale_simple(pbuf,48,48,GDK_INTERP_HYPER);
	gtk_image_set_from_pixbuf(GTK_IMAGE(item->icon),prealBuf);
	//gtk_image_set_from_file(GTK_IMAGE(item->icon),icon);
	//
	gdk_pixbuf_unref(pbuf);
	gdk_pixbuf_unref(prealBuf);
}

void gtk_tab_button_set_icon(GtkTabButton * item,TabButtonIcon icon)
{
	const gchar * styles[]=
	{
		"elegant",
		"android",
	};
	gchar * path = afsGetExcutablePath();
	gchar * bgPath = g_strdup_printf("%s/res/widgets/%s/%s",path,styles[item->style],icons[icon]);
	gtk_tab_button_set_icon_from_file(item,bgPath);
	g_free(bgPath);
	
}
GtkWidget* gtk_tab_button_new_with_label(gchar* label)
{
	GtkWidget* item;
	item = gtk_tab_button_new();
	gtk_tab_button_set_label(GTK_FLIP_BUTTON(item),label);
	gtk_widget_set_size_request(item,100,80);
	gtk_widget_show(GTK_WIDGET(GTK_FLIP_BUTTON(item)->label));
	gtk_widget_show(GTK_WIDGET(GTK_FLIP_BUTTON(item)->icon));
	//gtk_radio_button_set_group(GTK_RADIO_BUTTON(item),group);
	GTK_FLIP_BUTTON(item)->style = FLIP_BUTTON_ICON_ELEGANT;
	return item;
}

static gboolean gtk_tab_button_expose (GtkWidget  *widget,   GdkEventExpose *event)
{
	GtkTabButton *button = GTK_FLIP_BUTTON (widget);
	GtkWidget *child = GTK_BIN (widget)->child;
	if (gtk_widget_is_drawable (widget))
	{
		_gtk_tab_button_paint (button, &event->area,
			gtk_widget_get_state( widget),
			//button->in_button?GTK_STATE_ACTIVE:GTK_STATE_NORMAL,
			GTK_SHADOW_OUT,
			 "tabbutton",
			 "buttondefault");

		//GTK_WIDGET_CLASS (gtk_tab_button_parent_class)-> expose_event (widget, event);
	}
	
	if (child)
	{
		gtk_container_propagate_expose (GTK_CONTAINER (widget), child, event);
	}

	return FALSE;
}

static void gtk_tab_button_pressed (GtkButton *button)
{
	if (GTK_BUTTON_CLASS (gtk_tab_button_parent_class)->pressed)
	GTK_BUTTON_CLASS (gtk_tab_button_parent_class)->pressed (button);
	gtk_widget_set_state (GTK_WIDGET (button), GTK_STATE_SELECTED);
	gtk_widget_queue_draw (GTK_WIDGET (button));
}

#if 0



static void gtk_tab_button_set_background(GtkTabButton *tab_button,GtkStateType        state_type)
{
	GdkPixbuf *pbuf,*prealBuf,*bg;
	GdkPixmap *pixmap;
	gint width,height;
	gint x, y;
	GtkWidget *widget = GTK_WIDGET(tab_button);
	gint border_width = 0;
	const gchar * bgs[]=
	{
		
		"btn_square_overlay_normal.png",
		"btn_square_overlay_pressed.png",
		"btn_square_overlay_pressed.png",
		"btn_square_overlay_selected.png",
		"btn_square_overlay_disabled.png"
		
	};
	gchar * path = afsGetExcutablePath();
	gchar * bgPath = g_strdup_printf("%s/res/widgets/%s",path,bgs[state_type]);
	
	
   
	if(!GTK_WIDGET_REALIZED(widget))
	{
		gtk_widget_realize(widget);
	}
	border_width = 0;//GTK_CONTAINER (widget)->border_width;

	pbuf = gdk_pixbuf_new_from_file(bgPath  ,NULL);
	if(!pbuf)
	{
		g_print("fail\n");  
		return;
	}
	width = 46;//gdk_pixbuf_get_width(pbuf);
	height = 46;//gdk_pixbuf_get_height(pbuf);
	prealBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,gdk_pixbuf_get_bits_per_sample(pbuf),width,height);
	gdk_pixbuf_copy_area (pbuf,5,10,width, height,prealBuf,0,0);
	x = widget->allocation.x + border_width;
	y = widget->allocation.y + border_width;
	width = widget->allocation.width - border_width * 2;
	height = widget->allocation.height - border_width * 2;
	
	bg = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,gdk_pixbuf_get_bits_per_sample(pbuf),width,height);
       gdk_pixbuf_fill(bg,0);
       gdk_pixbuf_composite(prealBuf,bg,0,0,width,height,0,0,1,1,
               GDK_INTERP_BILINEAR,0x00);
   
       pixmap = gdk_pixmap_new(widget->window,width,height,-1);
       gdk_draw_pixbuf(pixmap,NULL,pbuf,0,0,0,0,-1,-1,GDK_RGB_DITHER_NORMAL,1,1);
  
       gdk_pixbuf_unref(pbuf);
       gdk_pixbuf_unref(prealBuf);
       gdk_pixbuf_unref(bg);
     
       gdk_draw_drawable(widget->window, widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
               pixmap,
               0, 0,
               x, y,
               width, height);
   
       //gtk_widget_set_app_paintable(widget,TRUE);
       //gdk_window_set_back_pixmap(widget->window,pixmap,FALSE);

       //gtk_widget_queue_draw(widget);
}


static void _tab_gtk_button_paint (GtkTabButton          *button,
		   const GdkRectangle *area,
		   GtkStateType        state_type,
		   GtkShadowType       shadow_type,
		   const gchar        *main_detail,
		   const gchar        *default_detail)
{
	GtkWidget *widget;
	gint width, height;
	gint x, y;
	gint border_width;
	GtkBorder default_border;
	GtkBorder default_outside_border;
	gboolean interior_focus;
	gint focus_width;
	gint focus_pad;
	const gchar * bgs[]=
	{
		"btn_square_overlay_normal.png",
		"btn_square_overlay_pressed.png",
		"btn_square_overlay_pressed.png",
		"btn_square_overlay_selected.png",
		"btn_square_overlay_disabled.png"
		
	};
	widget = GTK_WIDGET (button);

	if (gtk_widget_is_drawable (widget))
	{
		GdkPixbuf *pbuf;
		gchar * path = afsGetExcutablePath();
		gchar * bg = g_strdup_printf("%s/res/widgets/%s",path,bgs[state_type]);
		
		pbuf = gdk_pixbuf_new_from_file(bg ,NULL);
		if(!pbuf)
		{
			g_print("fail\n");  
			return;
		}

		gdk_draw_pixbuf(widget->window,
			widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
			pbuf,
			0,0,
			widget->allocation.x,widget->allocation.y,
			-1,-1,
			GDK_RGB_DITHER_NORMAL,1,1);
	}
}

#endif

void _gtk_tab_button_paint (GtkTabButton  *button,
		   const GdkRectangle *area,
		   GtkStateType        state_type,
		   GtkShadowType       shadow_type,
		   const gchar        *main_detail,
		   const gchar        *default_detail)
{
	GtkWidget *widget;
	gint width, height;
	gint x, y;
	gint border_width = 0;
	const gchar * bgs[]=
	{
		"btn_normal.png",
		"btn_selected.png",
		"btn_highlight.png",
		"btn_activated.png",
		"btn_highlight.png",
		"btn_square_overlay_pressed.png",
			
	};
	widget = GTK_WIDGET (button);

	//if (gtk_widget_is_drawable (widget))
	{
		border_width = 0;//GTK_CONTAINER (widget)->border_width;

		x = widget->allocation.x + border_width;
		y = widget->allocation.y + border_width;
		width = widget->allocation.width - border_width * 2;
		height = widget->allocation.height - border_width * 2;

		GdkPixbuf *pbuf,*prealBuf,*psizedBuf;
		gchar * path = afsGetExcutablePath();
		GtkToggleButton *toggle_button = GTK_TOGGLE_BUTTON (button);
		if(toggle_button->active == 1&&state_type ==GTK_STATE_PRELIGHT)
		{
			state_type = GTK_STATE_ACTIVE;
		}
		gchar * bg = g_strdup_printf("%s/res/widgets/%s",path,bgs[state_type]);
		//LOG("state = %d",state_type);
		pbuf = gdk_pixbuf_new_from_file(bg ,NULL);
		g_free(bg);
		if(!pbuf)
		{
			g_print("fail\n");  
			return;
		}
		prealBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,gdk_pixbuf_get_bits_per_sample(pbuf),46,46);

		if(state_type==0)
		{
			gdk_pixbuf_copy_area (pbuf,3,3,46, 46,prealBuf,0,0);
		}
		else
		{
			gdk_pixbuf_copy_area (pbuf,6,11,46, 46,prealBuf,0,0);
		}
		psizedBuf = gdk_pixbuf_scale_simple(prealBuf,width,height,GDK_INTERP_HYPER);
		
		//LOG("width=%d,height=%d\n",width,height);
		gdk_draw_pixbuf(widget->window,
			widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
			psizedBuf,
			0,0,
			widget->allocation.x,widget->allocation.y,
			width,height,
			GDK_RGB_DITHER_NORMAL,1,1);
		gdk_pixbuf_unref(pbuf);
		gdk_pixbuf_unref(prealBuf);
		gdk_pixbuf_unref(psizedBuf);
    
	}
}

