/* gcc test-gtk.c -g -O0 -o test-gtk `pkg-config --cflags --libs gtk+-3.0` && ./test-gtk */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

int main (int argc, char *argv[])
{
	GtkWidget *dialog, *container, *widget;
	GtkTreeModel *model;
	GtkListStore *list_store;
	GtkCellRenderer *cell;
	GtkCssProvider *css_provider;
	GtkStyleContext *style_context;
	const gchar *css;
	gint ii;
	GError *error = NULL;

	gtk_init (&argc, &argv);

	dialog = gtk_dialog_new_with_buttons ("test-gtk", NULL, 0, "_Cancel", GTK_RESPONSE_CANCEL, NULL);

	container = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

	g_object_set (G_OBJECT (dialog),
		"default-width", 640,
		"default-height", 480,
		NULL);

	widget = gtk_combo_box_new ();
	gtk_container_add (GTK_CONTAINER (container), widget);

	cell = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start ((GtkCellLayout *) widget, cell, TRUE);
	gtk_cell_layout_set_attributes ((GtkCellLayout *) widget, cell, "text", 0, NULL);

	list_store = gtk_list_store_new (1, G_TYPE_STRING);
	//for (ii = 0; ii < 500; ii++) {
	for (ii = 0; ii < 10; ii++) {
		GtkTreeIter iter;
		gchar *text = g_strdup_printf ("item %d", ii);

		gtk_list_store_append (list_store, &iter);
		gtk_list_store_set (list_store, &iter, 0, text, -1);

		g_free (text);
	}

	gtk_combo_box_set_model (GTK_COMBO_BOX (widget), (GtkTreeModel *) list_store);
	g_object_unref (list_store);

	css_provider = gtk_css_provider_new ();
	css = "GtkComboBox { -GtkComboBox-appears-as-list: 1; }";
	gtk_css_provider_load_from_data (css_provider, css, -1, &error);
	style_context = gtk_widget_get_style_context (widget);
	if (error == NULL) {
		gtk_style_context_add_provider (
			style_context,
			GTK_STYLE_PROVIDER (css_provider),
			GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	} else {
		g_warning ("%s: %s", G_STRFUNC, error->message);
		g_clear_error (&error);
	}
	g_object_unref (css_provider);

	gtk_widget_show_all (gtk_dialog_get_content_area (GTK_DIALOG (dialog)));
	gtk_dialog_run (GTK_DIALOG (dialog));

	gtk_widget_destroy (dialog);

	return 0;
}
