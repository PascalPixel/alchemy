#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct Object_080a17c4 { u8 unknown_00[0x16]; u16 value_16 : 9; u16 unknown_16b : 7; };

void Func_080a17c4(void *object)
{
    if (object != NULL) {
        M2C_FIELD(object, s8, 5) = 1;
        ((struct Object_080a17c4 *) object)->value_16 = M2C_FIELD(object, u16, 6);
        M2C_FIELD(object, s8, 0x14) = M2C_FIELD(object, u16, 8);
        M2C_FIELD(object, s8, 0x17) = -0x3F & M2C_FIELD(object, s8, 0x17);
        M2C_FIELD(object, s8, 0x15) = -4 & M2C_FIELD(object, s8, 0x15);
    }
}
