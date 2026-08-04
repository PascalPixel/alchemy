#include "types.h"
#include "four_object_motion.h"

void Func_08009038(void *);
void Func_08004278(s32);

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
    Func_08004278(0x080ad40d);
}
