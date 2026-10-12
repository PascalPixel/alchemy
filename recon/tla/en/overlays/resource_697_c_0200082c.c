/* Draft, differ 10 of 52: the native keeps the cell column in r0 (asrs r0 r3 #20) for the four compares; this form keeps it in r3. The id parameter is reused for the target, as the native does. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

extern const u8 Data_02005a7c[];

static __inline__ struct ObjectRuntime *Object_Create(s32 type, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Func_020057d8(type, x, y, z);
}

static __inline__ void Map_Stamp(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005820(a, b, c, d, e, f);
}

static __inline__ void Shadow_SetHeight(s32 id, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200070c(id, value);
}

void Func_0200082c(s32 a, s32 id)
{
    s32 x;

    if ((u32)(id - 15) <= 1) {
        Func_0200062c(id);
        return;
    }
    x = Object_GetById(id)->x;
    x >>= 20;
    id = 0;
    if (x == 13)
        id = 17;
    if (x == 15)
        id = 18;
    if (x == 17)
        id = 19;
    if (x == 19)
        id = 20;
    if (id != 0) {
        s32 flag = id + 0x203;

        if (GameFlag_Test(flag) == 0) {
            struct ObjectRuntime *object = Object_GetById(id);

            object->update = NULL;
            Animation_ApplyChildValues(object, 0);
            Object_SetModeById(id, 6);
            Func_0200070c(id, -16);
            GameFlag_SetBit(flag);
        }
    }
}
