#include <gtk/gtk.h>

static void on_main_window_destroy(GtkWidget *widget,
                                   gpointer   data)
{
    gtk_main_quit();
}


int main(int argc, char *argv[])
{
    GtkWidget *window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    g_signal_connect(window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}
