#include "types.h"
#include "scene.h"
#include "abi/object/motion/four_object/initialize_bottom_row.h"
#include "four_object_motion.h"

extern s32 gRom[];

void *Obj_Run(s32);
void Object_InitializeMode(void *, s32);

s32 ScheduleCallbackAfterFrames(void (*)(void), s32);

void FourObjectMotion_InitializeBottomRow(void)
{
    struct FourObjectMotionState *state = gIw;
    s32 index;

    for (index = 0; index < 4; index++) {
        void *object = state->objects[index];

        if (object != NULL) {
            Obj_Do(object);
            state->objects[index] = NULL;
        }
    }

    for (index = 0; index < 4; index++) {
        void *object = Obj_Run(gRom[index]);

        if (object != NULL) {
            Object_InitializeMode(object, 2);
        }
        state->objects[index] = object;
        state->phases[index] = 0x10000;
        state->positions_x[index] = 0x10;
        state->positions_y[index] = 0xc8;
    }
    ScheduleCallbackAfterFrames(Obj_Run2, 0xc80);
}
