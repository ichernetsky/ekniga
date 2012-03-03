#include <gtk/gtk.h>

static void toggle_statusbar(GtkWidget *widget, gpointer status_bar)
{
    if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget)))
        gtk_widget_show(status_bar);
    else
        gtk_widget_hide(status_bar);
}

static void app_quit(GtkWidget *widget,
                     gpointer   data)
{
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *window_grid;
    GtkWidget *workdesk_grid;

    GtkWidget *menu_bar;
    GtkWidget *status_bar;

    GtkWidget *file_menu;
    GtkWidget *file_menu_item;
    GtkWidget *quit_menu_item;

    GtkWidget *view_menu;
    GtkWidget *view_menu_item;
    GtkWidget *statusbar_menu_item;

    GtkAccelGroup *accel_group;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 500);
    gtk_window_set_title(GTK_WINDOW(window), "eKniga");

    window_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), window_grid);

    menu_bar = gtk_menu_bar_new();
    file_menu = gtk_menu_new();
    view_menu = gtk_menu_new();

    accel_group = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

    file_menu_item = gtk_menu_item_new_with_mnemonic("_File");
    quit_menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);

    gtk_widget_add_accelerator(quit_menu_item, "activate", accel_group,
                               GDK_KEY_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    view_menu_item = gtk_menu_item_new_with_label("View");
    statusbar_menu_item = gtk_check_menu_item_new_with_label("View Statusbar");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(statusbar_menu_item), TRUE);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_menu_item), view_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), statusbar_menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view_menu_item);

    gtk_grid_attach(GTK_GRID(window_grid), menu_bar, 0, 0, 1, 1);

    workdesk_grid = gtk_grid_new();

    GtkWidget *operations = gtk_label_new("Operations");
    GtkWidget *preview    = gtk_label_new("Preview");
    GtkWidget *thumbnails = gtk_label_new("Thumbnails");

    gtk_widget_set_vexpand(operations, TRUE);
    gtk_widget_set_vexpand(preview, TRUE);
    gtk_widget_set_hexpand(preview, TRUE);
    gtk_widget_set_vexpand(thumbnails, TRUE);

    gtk_grid_attach(GTK_GRID(workdesk_grid), operations, 0, 0, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(workdesk_grid), preview, operations,
                            GTK_POS_RIGHT, 1, 1);
    gtk_grid_attach_next_to(GTK_GRID(workdesk_grid), thumbnails, preview,
                            GTK_POS_RIGHT, 1, 1);

    gtk_grid_attach_next_to(GTK_GRID(window_grid), workdesk_grid, menu_bar,
                            GTK_POS_BOTTOM, 1, 1);

    status_bar = gtk_statusbar_new();
    gtk_widget_set_hexpand(status_bar, TRUE);
    gtk_statusbar_push(GTK_STATUSBAR(status_bar), 1, "StatusBar");

    gtk_grid_attach_next_to(GTK_GRID(window_grid), status_bar, workdesk_grid,
                            GTK_POS_BOTTOM, 1, 1);

    g_signal_connect(window, "destroy", G_CALLBACK(app_quit), NULL);
    g_signal_connect(G_OBJECT(quit_menu_item), "activate", G_CALLBACK(app_quit), NULL);
    g_signal_connect(G_OBJECT(statusbar_menu_item), "activate",
                     G_CALLBACK(toggle_statusbar), status_bar);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
