#include "ekniga.h"

static gint scale_width = 0;
static gint scale_height = 0;
static gboolean gray = FALSE;

static GOptionEntry entries[] =
{
    { "scale-width",  0, 0, G_OPTION_ARG_INT,  &scale_width,  "Width of output image",  "X" },
    { "scale-height", 0, 0, G_OPTION_ARG_INT,  &scale_height, "Height of output image", "Y" },
    { "gray",         0, 0, G_OPTION_ARG_NONE, &gray,         "Make image gray", NULL },
    { NULL }
};

static void usage(void)
{
    g_printerr("usage: ekniga [options] <input> <output>\n");
    g_printerr("run ekniga --help to see available options");
}

int main(int argc, char *argv[])
{
    EkImage *image;
    GError *error = NULL;
    GOptionContext *context;
    gchar *input_filename, *output_filename;

    /* Parsing command-line options */

    context = g_option_context_new("- EBook processing application.");
    g_option_context_add_main_entries (context, entries, NULL);
    if (!g_option_context_parse(context, &argc, &argv, &error))
    {
        g_printerr("option parsing failed: %s\n", error->message);
        return 1;
    }

    if (argc != 3) {
        usage();
        return 1;
    }

    if (scale_width < 0 || scale_height < 0) {
        g_printerr("scale options must be >= 0\n");
        return 1;
    }

    input_filename  = argv[1];
    output_filename = argv[2];

    /* Processing image */

    image = ek_image_new_from_file(input_filename, &error);
    if (image == NULL) {
        g_printerr("%s\n", error->message);
        return 1;
    }

    if (scale_width || scale_height) {
        if (!scale_width)
            scale_width = image->width * scale_height / image->height;
        if (!scale_height)
            scale_height = image->height * scale_width / image->width;
    }

    if (gray) {
        EkImage *gray_image = ek_image_new(image->width, image->height, EK_COLOR_TYPE_GRAY);
        ek_image_gray(image, gray_image);
        ek_image_free(image);
        image = gray_image;
    }

    if (scale_width || scale_height) {
        EkImage *scaled_image = ek_image_new(scale_width, scale_height, image->color_type);
        ek_image_scale(image, scaled_image);
        ek_image_free(image);
        image = scaled_image;
    }

    if (!ek_image_save_to_file(image, output_filename, &error)) {
        g_printerr("%s\n", error->message);
        return 1;
    }

    return 0;
}
