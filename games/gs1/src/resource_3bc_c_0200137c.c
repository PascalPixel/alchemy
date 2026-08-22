#include "types.h"

struct Object {
    u8 filler00[8];
    s32 x;
    s32 y;
    u8 filler10[8];
    s32 field18;
    s32 field1C;
    u8 filler20[0x44];
    s16 speedX;
    s16 speedY;
};

s32 Func_0200137c(struct Object *object)
{
    object->x += object->speedX << 8;
    object->y += object->speedY << 8;
    object->field18 += 0x666;
    object->field1C += 0x666;
    object->speedX += 5;
    object->speedY -= 1;
    return 0;
}
