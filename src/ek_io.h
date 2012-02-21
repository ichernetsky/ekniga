#ifndef __EK_IO_H_
#define __EK_IO_H_

#include <stdio.h>

#include "ek_image.h"

#define EK_IO_ERROR  ek_io_error_quark()

enum EkIOError {
    EK_IO_ERROR_OPEN,
    EK_IO_ERROR_IO,
    EK_IO_ERROR_INVALID_FILE,
    /* Other */
    EK_IO_ERROR_FAILED
};

GQuark ek_io_error_quark(void);

EkImage *ek_image_new_from_file(const gchar *filename,
                                GError **error);
EkImage *ek_image_new_from_stream(FILE *stream,
                                  GError **error);

gboolean ek_image_save_to_file(const EkImage *image,
                               const gchar *filename,
                               GError **error);
gboolean ek_image_save_to_stream(const EkImage *image,
                                 FILE *stream,
                                 GError **error);

#endif  /* __EK_IO_H_ */
