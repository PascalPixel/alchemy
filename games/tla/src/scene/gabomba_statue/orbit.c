#include "types.h"

struct Orbiter {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[15];
    u8 flags_23;
    u8 unknown_24[62];
    u8 phase;
    u8 unknown_63;
    s16 center_x;
    s16 center_z;
    void *link;
};

extern s32 Data_02005af0;
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);
s32 __divsi3(s32 numerator, s32 denominator);

void Func_02000398(struct Orbiter *actor, s32 *x, s32 *z)
{
    s32 position[3];
    s32 angle = (actor->phase << 16) / 180;
    s32 turn;

    position[0] = (actor->center_x + 16) << 16;
    position[1] = 0x100000;
    position[2] = actor->center_z << 16;
    turn = Data_02005af0 * 0x3000 + angle;
    Vector_AddPolarOffsetFar(0xd0000, turn + 0x8000, position);
    *x = position[0];
    *z = position[2];
}

s32 Func_02000408(struct Orbiter *actor)
{
    u8 *phase = &actor->phase;
    s32 angle = (*phase << 16) / 180;
    s32 next;

    actor->flags_23 = 0;
    actor->x = (actor->center_x + 16) << 16;
    actor->y = 0x100000;
    actor->z = actor->center_z << 16;
    Vector_AddPolarOffsetFar(0xd0000, angle + 0x8000, &actor->x);
    next = *phase + Data_02005af0;
    if (next < 0)
        next = 179;
    if (next > 179)
        next = 0;
    *phase = next;
    actor->link = Func_02000398;
    return 1;
}
