#include "types.h"
#include "four_object_motion.h"

void Func_08009038(void *);
void ScheduleCallback(s32);
extern u8 Data_080ad40d;

void Func_080ad658(void)
{
    struct FourObjectMotionState *state = Data_03001f2c;
    s32 index = 0;

    do {
        void *object = state->objects[index];

        if (object != 0) {
            Func_08009038(object);
            state->objects[index] = 0;
        }
        index++;
    } while (index < 4);
    ScheduleCallback((s32)&Data_080ad40d);
}
