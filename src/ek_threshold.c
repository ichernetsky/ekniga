#include "ek_threshold.h"

void ek_image_threshold(const EkImage *src,
                        EkImage *dst,
                        EkGray threshold)
{
    guint32 i, npixels;
    EkGray *src_data, *dst_data;

    g_assert(ek_image_is_gray(src));
    g_assert(ek_image_is_gray(dst));

    g_assert(src->width  == dst->width);
    g_assert(src->height == dst->height);

    g_assert(threshold != EK_GRAY_BLACK);
    g_assert(threshold != EK_GRAY_WHITE);

    npixels = src->width * src->height;
    src_data = src->data.gray;
    dst_data = dst->data.gray;

    for (i = 0; i < npixels; i++)
        dst_data[i] = src_data[i] <= threshold ? EK_GRAY_BLACK : EK_GRAY_WHITE;
}
