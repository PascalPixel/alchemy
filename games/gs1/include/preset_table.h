#ifndef GUARD_PRESET_TABLE_H
#define GUARD_PRESET_TABLE_H

#include "types.h"

/* One 24-byte preset record per class: the default element in the leading
 * word, four digit bytes behind it, then the four preset words. Read by
 * owner/get_digit_values, owner/get_default_element, and
 * owner/build_digit_tiles. */
struct PresetValues {
    s32 first;
    u8 digits[4];
    s32 values[4];
};

extern const struct PresetValues Data_08088e38[44];

#endif
