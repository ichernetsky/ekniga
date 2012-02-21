#ifndef __EK_IMAGE_H_
#define __EK_IMAGE_H_

#include <glib.h>

typedef enum EkColorType {
    EK_COLOR_TYPE_GRAY,
    EK_COLOR_TYPE_RGB
} EkColorType;

typedef guint8 EkGray;

#define EK_GRAY_BLACK  (0)
#define EK_GRAY_WHITE  (255)

typedef struct __attribute__ ((__packed__)) EkRgb
{
    guint8 r;
    guint8 g;
    guint8 b;
} EkRgb;

#define EK_WIDTH_MIN   (1)
#define EK_WIDTH_MAX   G_MAXUINT16
#define EK_HEIGHT_MIN  (1)
#define EK_HEIGHT_MAX  G_MAXUINT16

typedef struct EkImage
{
    guint16 width;
    guint16 height;

    EkColorType color_type;

    union {
        guint8 *raw;
        EkGray *gray;
        EkRgb  *rgb;
    } data;

    guint64 data_size;
} EkImage;

#define ek_index(i, j, width)  ((i) * (width) + (j))

EkImage *ek_image_new(guint16      width,
                      guint16      height,
                      EkColorType  color_type);
void ek_image_free(EkImage *image);

gboolean ek_color_type_is_gray(EkColorType ct);
gboolean ek_color_type_is_rgb(EkColorType ct);

gboolean ek_image_is_gray(const EkImage *image);
gboolean ek_image_is_rgb(const EkImage *image);

gsize ek_color_type_size(EkColorType ct);
gsize ek_image_bytes_per_pixel(const EkImage *image);
gsize ek_image_bit_depth(const EkImage *image);

#endif  /* __EK_IMAGE_H_ */
