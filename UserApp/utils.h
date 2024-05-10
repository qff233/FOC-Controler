#ifndef FOC_UTILS_H
#define FOC_UTILS_H

#define CONSTRAIN(value, min, max) (value < min ? \
                                    min :         \
                                    value > max ? max : value);

#endif //FOC_UTILS_H
