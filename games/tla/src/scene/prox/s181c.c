#include "types.h"

struct RisingObject
{
    u8  unk00[8];
    s32 x;
    s32 y;
    u8  unk10[8];
    s32 scaleX;
    s32 scaleY;
    u8  unk20[0x44];
    s16 drift;
    u8  unk66[2];
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
    object->scaleX += 0x51e;
    object->scaleY += 0x51e;
    object->drift += 2;
    if (--object->life == 0)
        Func_020043e4(object);
}
