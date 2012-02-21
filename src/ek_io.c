#include <errno.h>
#include <png.h>

#include "ek_io.h"

GQuark ek_io_error_quark(void)
{
  return g_quark_from_static_string("ek-io-error-quark");
}

EkImage *ek_image_new_from_file(const gchar *filename,
                                GError **error)
{
    FILE *stream;
    EkImage *image;

    stream = fopen(filename, "rb");
    if (stream == NULL) {
        g_set_error(error,
                    EK_IO_ERROR,
                    EK_IO_ERROR_OPEN,
                    "Failed to open file: %s",
                    g_strerror(errno));
        return NULL;
    }

    image = ek_image_new_from_stream(stream, error);

    fclose(stream);
    return image;
}

static EkColorType ek_color_type_for_png_color_type(ct)
{
    switch (ct) {

    case PNG_COLOR_TYPE_GRAY:
    case PNG_COLOR_TYPE_GRAY_ALPHA:
        return EK_COLOR_TYPE_GRAY;

    default:
        return EK_COLOR_TYPE_RGB;
    }
}

EkImage *ek_image_new_from_stream(FILE *stream,
                                  GError **error)
{
    const gsize bytes_to_check = 8;
    png_byte check_buffer[bytes_to_check];

    EkImage *image;
    png_structp png;
    png_infop info;

    png_uint_32 width, height;
    gint bit_depth, color_type, interlace_type, intent;
    EkColorType image_color_type;
    png_color_16 background, *image_background;
    gdouble screen_gamma, image_gamma;
    png_bytep *rows;
    guint32 bytes_per_row;
    guint32 i;

    if (fread(check_buffer, 1, bytes_to_check, stream) != bytes_to_check) {
        if (feof(stream))
            g_set_error(error,
                        EK_IO_ERROR,
                        EK_IO_ERROR_INVALID_FILE,
                        "Wrong and/or small file");
        else
            g_set_error(error,
                        EK_IO_ERROR,
                        EK_IO_ERROR_IO,
                        "I/O error while checking PNG header");
        return NULL;
    }

    if (png_sig_cmp(check_buffer, 0, bytes_to_check)) {
        g_set_error(error,
                    EK_IO_ERROR,
                    EK_IO_ERROR_INVALID_FILE,
                    "File is not PNG");
        return NULL;
    }

    png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                 NULL,  /* pointer to data for error-reporting functions */
                                 NULL,  /* function for reporting errors */
                                 NULL   /* function for reporting warnings */
                                 );
    if (png == NULL) {
        g_set_error(error,
                    EK_IO_ERROR,
                    EK_IO_ERROR_FAILED,
                    "Could not allocate and initialize png_struct");
        return NULL;
    }

    info = png_create_info_struct(png);
    if (info == NULL) {
        g_set_error(error,
                    EK_IO_ERROR,
                    EK_IO_ERROR_FAILED,
                    "Could not allocate and initialize info_struct");
        png_destroy_read_struct(&png, NULL, NULL);
        return NULL;
    }

    if (setjmp(png_jmpbuf(png))) {
        g_set_error(error,
                    EK_IO_ERROR,
                    EK_IO_ERROR_FAILED,
                    "Could not set error-handling approach");
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    png_init_io(png, stream);
    png_set_sig_bytes(png, bytes_to_check);
    png_read_info(png, info);
    png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);

    /* TODO: checking of image dimensions */

    png_set_scale_16(png);
    png_set_packing(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    if (png_get_bKGD(png, info, &image_background))
        png_set_background(png, image_background,
                           PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
    else {
        background.index = 0;
        background.gray = 255;
        background.red = background.green = background.blue = 255;
        png_set_background(png, &background,
                           PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
    }

    screen_gamma = 2.2;
    if (png_get_sRGB(png, info, &intent))
        png_set_gamma(png, screen_gamma, 0.45455);
    else {
        if (png_get_gAMA(png, info, &image_gamma))
            png_set_gamma(png, screen_gamma, image_gamma);
        else
            png_set_gamma(png, screen_gamma, 0.45455);
    }

    png_read_update_info(png, info);

    image_color_type = ek_color_type_for_png_color_type(color_type);
    image = ek_image_new(width, height, image_color_type);

    rows = png_malloc(png, height * sizeof(png_bytep));
    for (i = 0; i < height; i++)
        rows[i] = png_malloc(png, png_get_rowbytes(png, info));

    png_read_image(png, rows);
    png_read_end(png, info);

    bytes_per_row = width * ek_image_bytes_per_pixel(image);

    guint8 *data = image->data.raw;
    for (i = 0; i < height; i++) {
        memmove(data, rows[i], bytes_per_row);
        data += bytes_per_row;
    }

    png_destroy_read_struct(&png, &info, NULL);

    return image;
}

gboolean ek_image_save_to_file(const EkImage *image,
                               const gchar *filename,
                               GError **error)
{
    FILE *stream;
    gboolean status;

    stream = fopen(filename, "wb");
    if (stream == NULL) {
        g_set_error(error,
                    EK_IO_ERROR,
                    EK_IO_ERROR_OPEN,
                    "Failed to open file: %s",
                    g_strerror(errno));
        return FALSE;
    }

    status = ek_image_save_to_stream(image, stream, error);
    fclose(stream);

    return status;
}

static gint png_color_type_for_ek_color_type(EkColorType ct)
{
    switch (ct) {

    case EK_COLOR_TYPE_GRAY:
        return PNG_COLOR_TYPE_GRAY;

    case EK_COLOR_TYPE_RGB:
        return PNG_COLOR_TYPE_RGB;

    default:
        g_warn_if_reached();
        /* Returning value to get rid of IDE/compiler warning */
        return PNG_COLOR_TYPE_RGB;
    }
}

gboolean ek_image_save_to_stream(const EkImage *image,
                                 FILE *stream,
                                 GError **error)
{
    png_structp png;
    png_infop info;
    gint color_type;
    gsize bytes_per_pixel, bit_depth;
    png_bytep *rows;
    guint32 i;

    png = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                  NULL,  /* pointer to data for error-reporting functions */
                                  NULL,  /* function for reporting errors */
                                  NULL   /* function for reporting warnings */
                                  );
    if (png == NULL) {
        g_set_error(error,
                    EK_IO_ERROR,
                    EK_IO_ERROR_FAILED,
                    "Could not allocate and initialize png_struct");
        return FALSE;
    }

    info = png_create_info_struct(png);
    if (info == NULL) {
        g_set_error(error,
                    EK_IO_ERROR,
                    EK_IO_ERROR_FAILED,
                    "Could not allocate and initialize info_struct");
        png_destroy_write_struct(&png,  NULL);
        return FALSE;
    }

    if (setjmp(png_jmpbuf(png)))
    {
        g_set_error(error,
                    EK_IO_ERROR,
                    EK_IO_ERROR_FAILED,
                    "Could not set error-handling approach");
        png_destroy_write_struct(&png, &info);
        return FALSE;
    }

    png_init_io(png, stream);

    color_type = png_color_type_for_ek_color_type(image->color_type);
    bit_depth = ek_image_bit_depth(image);
    png_set_IHDR(png, info, image->width, image->height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    /* TODO: add gamma info to image structure */
    /* png_set_gAMA(png_ptr, info_ptr, gamma); */

    png_write_info(png, info);
    png_set_packing(png);

    rows = png_malloc(png, image->height * sizeof(png_bytep));
    bytes_per_pixel = ek_image_bytes_per_pixel(image);

    guint8 *data = image->data.raw;
    for (i = 0; i < image->height; i++) {
        rows[i] = data;
        data += image->width * bytes_per_pixel;
    }

    png_write_image(png, rows);
    png_write_end(png, info);

    png_destroy_write_struct(&png, &info);

    return TRUE;
}
