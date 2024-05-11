#ifndef FOC_UTILS_H
#define FOC_UTILS_H

#define _PI 3.1415927f
#define _2PI 6.2831853f

#define CONSTRAIN(value, min, max) (value < min ? \
                                    min :         \
                                    value > max ? max : value);

#endif //FOC_UTILS_H
