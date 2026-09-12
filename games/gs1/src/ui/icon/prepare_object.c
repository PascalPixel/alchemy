#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct UiIconObject { u8 unknown_00[0x16]; u16 value_16 : 9; u16 unknown_16b : 7; };

void UiIcon_PrepareObject(void *object)
{
    if (object != NULL) {
        FIELD_AT_OFFSET(object, s8, 5) = 1;
        ((struct UiIconObject *) object)->value_16 = FIELD_AT_OFFSET(object, u16, 6);
        FIELD_AT_OFFSET(object, s8, 0x14) = FIELD_AT_OFFSET(object, u16, 8);
        FIELD_AT_OFFSET(object, s8, 0x17) = -0x3F & FIELD_AT_OFFSET(object, s8, 0x17);
        FIELD_AT_OFFSET(object, s8, 0x15) = -4 & FIELD_AT_OFFSET(object, s8, 0x15);
    }
}
