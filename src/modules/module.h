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

#ifndef _MODULE_INCLUDE_
#define _MODULE_INCLUDE_

#include <stdio.h>
#include <unistd.h>

#include <glib.h>
#include <gtk/gtk.h>

#define MLOG(fmt, ...) \
	fprintf(stderr,"[Module]%s:%d:",__FUNCTION__,__LINE__),\
	fprintf(stderr,fmt,##__VA_ARGS__),\
	fprintf(stderr,"\n")

GtkWidget * afsGetModules(const gchar * name);


#endif

