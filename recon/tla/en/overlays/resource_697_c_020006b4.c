/* Draft, differ 2 of 38: the native stores the owner link before it copies the object for the priority flags (str r6 [r5, #104]; adds r0 r5); this form copies first. An explicit flag pointer is worse (differ 27). */
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

void Func_020006b4(s32 id)
{
    struct ObjectRuntime *owner = Object_GetById(id);
    struct ObjectRuntime *object;

    if (owner != NULL) {
        object = Object_Create(489, owner->x, owner->y, owner->z);
        if (object != NULL) {
            ObjectDispatch_SetSingleChildFlags(object, 0);
            Func_020057d0(object, Data_02005a7c);
            object->linked_object = owner;
            object->priority_flags |= 0x80;
            object->motion_flags = 0;
            object->collision_flags = 0;
            OBJECT_BYTE(object, 99) = 16;
            owner->linked_object = object;
        }
    }
}
