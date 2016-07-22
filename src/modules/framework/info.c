

#include <string.h>

#include "module.h"
#include "framework.h"

static void InfoAppend(GtkTextBuffer *buffer,
                          const gchar   *text,
                          gint           len)
{
	GtkTextIter iter;
	gtk_text_buffer_get_end_iter (buffer,  &iter);
	gtk_text_buffer_insert(buffer, &iter,text,len);
}


static void InfoCmd(GtkTextBuffer *buffer,const gchar * cmd)
{
	GtkBuilder * builder = ModuleGetBuilder();
	GtkEntry * entry = GTK_ENTRY(gtk_builder_get_object (builder, "ENTRY"));
	const gchar * fullPath = gtk_entry_get_text(entry);
	gchar * fullCmd =NULL;
	FILE * fp =NULL;
	gchar buf[1024];
	
	fullCmd = g_strdup_printf("git --work-tree=%s --git-dir=%s/.git %s",fullPath,fullPath,cmd);
	if(fullCmd)
	{
		fp = popen(fullCmd,"r");
		if(fp)
		{
			while(fgets(buf, 1024,fp))
			{
				MLOG("%s",buf);
				InfoAppend(buffer,"\n",strlen("\n"));
				InfoAppend(buffer,buf,strlen(buf));
			}
			pclose(fp); 
		}
		g_free(fullCmd);
	}
}

void InfoRefresh(gchar * git)
{
	GtkBuilder * builder = ModuleGetBuilder();
	GtkWidget * textView = GTK_WIDGET (gtk_builder_get_object (builder, "INFO"));
	GtkTextBuffer * textBuffer =GTK_TEXT_BUFFER (gtk_builder_get_object (builder, "TEXTBUFFER"));
	if(textView && textBuffer)
	{
		gchar * confFile = g_strdup_printf("%s/%s",git,"config");
		if(confFile)
		{
			FILE * fp = fopen(confFile,"r");
			gtk_text_buffer_set_text(textBuffer , " " ,1);
			if(fp)
			{
				gchar buf[1024];
				gint nRead = fread(buf, 1,1024,fp);
				MLOG("n= %d",nRead);
				
				while(nRead > 0)
				{
					InfoAppend(textBuffer,buf,nRead);
					nRead = fread(buf, 1024,1,fp);
				}
				fclose(fp);
			}
			
			//InfoCmd(textBuffer,"status");			
			InfoCmd(textBuffer,"branch");
			//InfoCmd(textBuffer,"remote show korg");
			
			gtk_text_view_set_buffer(GTK_TEXT_VIEW(textView),textBuffer);
		}
	}
}

void InfoInit(GtkWidget * textView)
{
	
}
