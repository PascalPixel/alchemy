#include "types.h"
#include "scene.h"
#include "abi/object/motion/four_object/replace_slot.h"
#include "four_object_motion.h"

extern s32 gRom[];

void *Obj_Run(s32);
void Object_InitializeMode(void *, s32);

s32 FourObjectMotion_ReplaceSlot(s32 index, s32 kind, s32 value)
{
    struct FourObjectMotionState *state = gIw;

    if (state->objects[index] != 0) {
        Obj_Do(state->objects[index]);
        state->objects[index] = 0;
    }
    {
        void *object = Obj_Run(gRom[kind]);

        if (object != 0) {
            Object_InitializeMode(object, value);
        }
        state->objects[index] = object;
    }
    return 1;
}
