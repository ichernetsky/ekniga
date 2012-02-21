#include "ekniga.h"

static void usage(void)
{
    g_printerr("usage: ekniga <input> <output>\n");
}

int main(int argc, char *argv[])
{
    EkImage *image;
    GError *error = NULL;
    GOptionContext *context;
    gchar *input, *output;

    /* Parsing command-line options */

    context = g_option_context_new("- EBook processing application.");
    if (!g_option_context_parse(context, &argc, &argv, &error))
    {
        g_printerr("option parsing failed: %s\n", error->message);
        return 1;
    }

    if (argc != 3) {
        usage();
        return 1;
    }

    input  = argv[1];
    output = argv[2];

    /* Processing image */

    image = ek_image_new_from_file(input, &error);
    if (image == NULL) {
        g_printerr("%s\n", error->message);
        return 1;
    }

    if (!ek_image_save_to_file(image, output, &error)) {
        g_printerr("%s\n", error->message);
        return 1;
    }

    return 0;
}
