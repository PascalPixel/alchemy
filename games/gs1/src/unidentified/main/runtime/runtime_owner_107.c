#include "types.h"

struct Triple08095fcc {
    s32 x;
    s32 y;
    s32 z;
};

struct Object08095fcc {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[80];
    u16 timer;
    s16 angle;
};

extern s32 Data_02000240[];

s32 Object_GetById(u32);
void RotateVectorByMagnitude(s32, s32, void *);
void Object_Destroy();

void Func_08095fcc(struct Object08095fcc *arg)
{
    struct Triple08095fcc local;
    struct Object08095fcc *other;
    s32 raw;
    s16 value;
    s32 y;

    other = (struct Object08095fcc *)Object_GetById(Data_02000240[125]);
    raw = arg->timer - 1;
    arg->timer = raw;
    value = arg->timer;
    local.x = other->x;
    local.z = other->z;
    RotateVectorByMagnitude(value * 0x6666,
                  (value << 11) + arg->angle,
                  &local);
    arg->x = local.x;
    arg->z = local.z;
    y = arg->y + 0xFFFF0000;
    arg->y = y;
    if (y < other->y + 0x140000)
        Object_Destroy(arg);
}
