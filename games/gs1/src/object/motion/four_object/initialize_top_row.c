#include "types.h"
#include "scene.h"
#include "four_object_motion.h"

extern u8 RomBytes_080ad35d[];
extern u8 RomBytes_080ad40d[];

/* object/motion/four_object/initialize_top_row.c */
extern s32 RomBytes_080af304[];

void *Obj_Run(s32);
void Object_InitializeMode(void *, s32);

s32 ScheduleCallbackAfterFrames(void (*)(void), s32);
void FunctionHead_080ad35c(void);
void FunctionHead_080ad40c(void);

void FourObjectMotion_InitializeTopRow(void)
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
        void *object = Obj_Run(RomBytes_080af304[index]);

        if (object != NULL) {
            Object_InitializeMode(object, 2);
        }
        state->objects[index] = object;
        state->positions_x[index] = 0x10;
        state->positions_y[index] = 0x20;
    }
    ScheduleCallbackAfterFrames(FunctionHead_080ad35c, 0xc80);
}
