/* Near miss, differ 2 of 97 (one reordered pair): the native stores the new
 * scale_y before it addresses the action (adds r2, r6, #0); sched2 gives the
 * reload copy priority 39 against the store's 38. Overlay 6aa's
 * Func_02001f90 form (and Func_02000864's) with both barriers leaves differ 4:
 * the second barrier keeps the call's first argument and the zero in r5 from
 * rising into the block. With the first barrier only, differ 2; plain,
 * differ 2. A 6-minute permute of this form found nothing better. */
#include "../../../../games/tla/src/field/overlay_653/local.h"

s32 Func_02000824(struct ObjectRuntime *a, struct ObjectRuntime *b);

void Func_02000a0c(struct ObjectRuntime *object)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    struct ObjectRuntime *other = Object_GetById(12);
    s32 vx = object->bounce;
    s32 vz;
    struct FieldSprite *sprite;
    s32 rate;
    s32 scale;

    object->x += vx;
    object->y += object->gravity;
    vz = object->unknown_4c;
    object->z += vz;
    object->bounce = vx - vx / 18;
    object->unknown_4c = vz - vz / 16;
    /* FAKEMATCH: ⚓️ stores the Z speed before it loads the scale rate */
    asm volatile("");
    rate = object->speed_limit;
    scale = object->scale_x;
    sprite = object->animation;
    object->scale_x = scale + rate;
    object->scale_y += object->acceleration;
    sprite->rotation += object->action;
    if (Func_02000824(object, leader)) {
        ((struct FieldSprite *)object->animation)->priority = 3;
        object->z -= object->unknown_4c;
        object->gravity += object->unknown_4c >> 2;
        object->unknown_4c = 0;
    }
    if (Func_02000824(object, other)) {
        ((struct FieldSprite *)object->animation)->priority = 3;
        object->z -= object->unknown_4c;
        object->gravity += object->unknown_4c >> 3;
        object->unknown_4c = 0;
    }
}
