/*
 * Draft: Object_SetTargetAndCallback, ported from its ☀️ twin; it goes
 * before ObjectMotion_StepAngle in SRC/FIELD/COMMON/OBJECT/VISUAL_ATTRIBUTES.C.
 * The listing loads the zero it stores at 0x59 from a literal pool
 * (ldr r1, =0) ahead of the acceleration copy and places the pool after a
 * branch; this C builds the zero with movs after the copies.
 * API context measured 2026-10-02 with ordinary target flags:
 * all six prior successful objects retain every allocated byte and
 * normalized call/pool operand; no new match or adoption is claimed.
 * The shared void DispatchObject/u32 contract uses ordinary data casts;
 * every original matching-body and trial annotation is retained.
 */
#include "OBJECT_RUNTIME.H"
#include "OBJDISP.H"

void Object_SetTargetAndCallback(u32 object_id, s32 target_id, const void *callback)
{
    struct ObjectRuntime *first = ObjectTable_Get(object_id);
    struct ObjectRuntime *second = ObjectTable_Get(target_id & 0xff);

    if (first != NULL && second != NULL) {
        first->linked_object = second;
        if (!(target_id & 0x10000)) {
            first->action = 40;
            first->acceleration = second->acceleration * 2;
            first->speed_limit = second->speed_limit;
            first->unknown_57[2] = 0;
        }
        ObjectDispatch_InitializeFar((struct DispatchObject *)first, (u32)callback);
    }
}
