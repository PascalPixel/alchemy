/* Draft, differ 28 of 86: this form hoists the turn step 0x1000 out of the
 * scale loop into r6 from the literal pool; the native builds it inside the
 * loop (movs r1 #128, lsls #5). An int temporary for the angle, a for (;;)
 * loop and a forwarding inline for the wait leave it hoisted. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void WaitFrames(s32 frames);
struct ObjectRuntime *Func_02000da4(s32 type, s32 x, s32 y, s32 z);
void Func_02000ebc(s32 scene, s32 entrance);

void Func_02000a10(s32 mode, s32 x, s32 z, s32 scene, s32 entrance)
{
    struct ObjectRuntime *object;

    if (mode == 1) {
        object = Func_02000da4(354, x << 16, 0, z << 16);
        if (object != NULL) {
            ObjectDispatch_SetSingleChildFlags(object, 0);
            object->priority_flags = 2;
        }
    }
    if (mode == 2) {
        object = Func_02000da4(252, x << 16, 0, z << 16);
        if (object != NULL) {
            s32 scale;

            ObjectDispatch_SetSingleChildFlags(object, 0);
            object->velocity_y = 0x60000;
            object->scale_x = 0x6666;
            object->scale_y = 0x6666;
            for (;;) {
                WaitFrames(1);
                scale = object->scale_x + 0x51e;
                object->angle += 0x1000;
                object->scale_x = scale;
                object->scale_y = scale;
                if (scale > 0xffff)
                    break;
            }
            object->scale_x = 0x10000;
            object->scale_y = 0x10000;
            object->angle = Object_GetById(gPartyState.current_owner)->angle + 0x8000;
            Func_02000ebc(scene, entrance);
        }
    }
}
