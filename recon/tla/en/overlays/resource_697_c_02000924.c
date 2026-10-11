/* Draft, differ 4 of 24: the native builds -16 before it copies the id, and stores collision_flags before it copies the object for the priority flags; this form schedules both copies earlier. */
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

void Func_02000924(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != NULL) {
        Shadow_SetHeight(id, -16);
        object->collision_flags = 0;
        object->priority_flags |= 2;
        Func_020057c8(object, 6);
    }
}
