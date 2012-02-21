#include "ek_image.h"

EkImage *ek_image_new(guint16     width,
                      guint16     height,
                      EkColorType color_type)
{
    EkImage *image;
    guint64 size;

    g_assert(width  >= EK_WIDTH_MIN);
    g_assert(width  <= EK_WIDTH_MAX);
    g_assert(height >= EK_HEIGHT_MIN);
    g_assert(height <= EK_HEIGHT_MAX);

    image = g_malloc(sizeof(EkImage));
    image->width      = width;
    image->height     = height;
    image->color_type = color_type;

    size = width * height;
    size *= ek_color_type_size(color_type);
    g_assert(size > 0);

    image->data_size = size;
    image->data = g_malloc(size);

    return image;
}

void ek_image_free(EkImage *image)
{
    if (image)
        g_free(image->data);
    g_free(image);
}

gboolean ek_color_type_is_gray(EkColorType ct)
{
    return ct == EK_COLOR_TYPE_GRAY;
}

gboolean ek_color_type_is_rgb(EkColorType ct)
{
    return ct == EK_COLOR_TYPE_RGB;
}

gboolean ek_image_is_gray(const EkImage *image)
{
    return ek_color_type_is_gray(image->color_type);
}

gboolean ek_image_is_rgb(const EkImage *image)
{
    return ek_color_type_is_rgb(image->color_type);
}

gsize ek_color_type_size(EkColorType ct)
{
    switch (ct) {

    case EK_COLOR_TYPE_GRAY:
        return sizeof(EkGray);

    case EK_COLOR_TYPE_RGB:
        return sizeof(EkRgb);

    default:
        g_warn_if_reached();
        break;
    }

    return 0;

}

gsize ek_image_bytes_per_pixel(const EkImage *image)
{
    return ek_color_type_size(image->color_type);
}

gsize ek_image_bit_depth(const EkImage *image)
{
    return 8;
}
