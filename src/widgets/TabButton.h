#ifndef _FLIP_BUTTON_
#define _FLIP_BUTTON_
#include <gtk/gtk.h>

#define TYPE_FLIP_BUTTON	(gtk_tab_button_get_type())
#define GTK_FLIP_BUTTON(obj)	(GTK_CHECK_CAST(obj,TYPE_FLIP_BUTTON,GtkTabButton))

typedef struct _GtkTabButton GtkTabButton;
typedef struct _GtkTabButtonClass GtkTabButtonClass;

struct _GtkTabButton
{
	GtkRadioButton parent;
	GtkLabel*label;
	GtkImage*icon;
	gint style;
};

struct _GtkTabButtonClass
{
	GtkRadioButtonClass parent_class;

};
typedef enum 
{
	ICON_FRAMEWORK,
	ICON_LOG,
	ICON_EMULATOR,
	ICON_SYNC,
	ICON_TOOL,
	ICON_TERMINAL,
	ICON_DOCUMENT,
	ICON_ABOUT,
	ICON_HOME,
	
}TabButtonIcon;

enum
{
	FLIP_BUTTON_ICON_ELEGANT,
	FLIP_BUTTON_ICON_ANDROID,
};
GtkType gtk_tab_button_get_type(void);
GtkWidget* gtk_tab_button_new(void);
void gtk_tab_button_set_label(GtkTabButton* item, gchar* label);
void gtk_tab_button_set_icon_from_file(GtkTabButton* item, gchar* icon);
void gtk_tab_button_set_icon(GtkTabButton* item, TabButtonIcon icon);
GtkWidget* gtk_tab_button_new_with_label(gchar* label);

#endif //__OUR_ITEM_H__
 


