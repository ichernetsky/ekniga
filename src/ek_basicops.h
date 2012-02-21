#ifndef __EK_BASICOPS_H_
#define __EK_BASICOPS_H_

#include "ek_image.h"

void ek_image_gray(const EkImage *src,
                   EkImage *dst);
void ek_image_scale(const EkImage *src,
                    EkImage *dst);

#endif  /* EK_BASICOPS_H_ */
