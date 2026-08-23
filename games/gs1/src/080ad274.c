#include "types.h"
#include "four_object_motion.h"
extern s32 Data_080af304[];

void Func_08009038(void *);
void *Func_08009030(s32);
void Object_InitializeMode(void *, s32);
void Func_080ad35c(void);
s32 ScheduleCallbackAfterFrames(void (*)(void), s32);

void Func_080ad274(void)
{
    struct FourObjectMotionState *state = Data_03001f2c;
    s32 index;

    for (index = 0; index < 4; index++) {
        void *object = state->objects[index];

        if (object != NULL) {
            Func_08009038(object);
            state->objects[index] = NULL;
        }
    }

    for (index = 0; index < 4; index++) {
        void *object = Func_08009030(Data_080af304[index]);

        if (object != NULL) {
            Object_InitializeMode(object, 2);
        }
        state->objects[index] = object;
        state->positions_x[index] = 0x10;
        state->positions_y[index] = 0x20;
    }
    ScheduleCallbackAfterFrames(Func_080ad35c, 0xc80);
}
