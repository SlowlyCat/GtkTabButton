/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * afsMain.c
 * Copyright (C) John Coppla 2011 <john.coppla@hotmail.com>
 * 
 * AndroidDevSuit is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * AndroidDevSuit is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <glib.h>

#include "afs.h"
#include "env.h"
#ifdef ENABLE_NLS
#  include <libintl.h>
#endif

AfsFrame glAfsFrame ;

int main (int argc, char *argv[])
{
 	GtkWidget *window;


#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif
	//if ( !g_thread_supported() )
	{
		g_thread_init(NULL);
		gdk_threads_init();
	}

	gtk_set_locale ();
	gtk_init (&argc, &argv);
	glAfsFrame.afsBuilder = gtk_builder_new ();
	glAfsFrame.afsPath = afsGetExcutablePath();
	afsModulesInit();
//	afsPrintModules();
	afsMainWindow ();
	//gdk_threads_enter();
	gtk_main ();
	//gdk_threads_leave(); 
	return 0;
}

