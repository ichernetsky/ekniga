#include "ek_basicops.h"

void ek_image_gray(const EkImage *src,
                   EkImage *dst)
{
    g_assert(ek_image_is_rgb(src));
    g_assert(ek_image_is_gray(dst));

    g_assert(src->width == dst->width);
    g_assert(src->height == dst->height);

    EkRgb *src_data = src->data.rgb;
    EkGray *dst_data = dst->data.gray;
    EkRgb pixel;

    guint32 npixels = src->width * src->height;
    guint32 i;

    for (i = 0; i < npixels; i++) {
        pixel = src_data[i];
        dst_data[i] = (6968 * pixel.r + 23434 * pixel.g + 2366 * pixel.b) / 32768;
    }

}

void ek_image_scale(const EkImage *src,
                    EkImage *dst)
{
    gint i, j, src_i, src_j;

    guint32 src_width  = src->width;
    guint32 dst_width  = dst->width;
    guint32 src_height = src->height;
    guint32 dst_height = dst->height;

    guint32 width_ratio  = (src_width << 16) / dst_width + 1;
    guint32 height_ratio = (src_height << 16) / dst_height + 1;

    g_assert(src->color_type == dst->color_type);

    switch (src->color_type) {

    case EK_COLOR_TYPE_GRAY:
    {
        EkGray *src_data = src->data.raw;
        EkGray *dst_data = dst->data.raw;

        for (i = 0; i < dst_height; i++) {
            for (j = 0; j < dst_width; j++) {
                src_i = (i * height_ratio) >> 16;
                src_j = (j * width_ratio)  >> 16;

                dst_data[ek_index(i, j, dst_width)] = src_data[ek_index(src_i, src_j, src_width)];
            }
        }
        break;
    }

    case EK_COLOR_TYPE_RGB:
    {
        EkRgb *src_data = src->data.rgb;
        EkRgb *dst_data = dst->data.rgb;

        for (i = 0; i < dst_height; i++) {
            for (j = 0; j < dst_width; j++) {
                src_i = (i * height_ratio) >> 16;
                src_j = (j * width_ratio)  >> 16;

                dst_data[ek_index(i, j, dst_width)] = src_data[ek_index(src_i, src_j, src_width)];
            }
        }
        break;
    }

    default:
        g_warn_if_reached();
        break;
    }
}
