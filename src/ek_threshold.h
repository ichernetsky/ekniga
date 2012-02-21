#ifndef __EK_THRESHOLD_H_
#define __EK_THRESHOLD_H_

#include "ek_image.h"

void ek_image_threshold(const EkImage *src,
                        EkImage *dst,
                        EkGray threshold);

#endif  /* EK_THRESHOLD_H_ */
