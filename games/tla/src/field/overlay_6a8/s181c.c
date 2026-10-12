#include "types.h"

struct RisingObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    u8 unknown_10[8];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[0x44];
    s16 drift;
    u8 unknown_66[2];
    s32 life;
};

void Func_020043e4(struct RisingObject *object);

void Func_0200181c(struct RisingObject *object)
{
    s32 y;

    object->x += object->drift << 8;
    y = object->y;
    asm volatile(""); /* FAKEMATCH: ⚓️ loads y before it builds 0x8000 */
    object->y = y + 0x8000;
    asm volatile(""); /* FAKEMATCH: ⚓️ stores y before it builds 0x51e */
    object->scale_x += 0x51e;
    object->scale_y += 0x51e;
    object->drift += 2;
    if (--object->life == 0)
        Func_020043e4(object);
}
