#include "local.h"

s32 Math_Cosine(s32 angle);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Func_02000930(struct ObjectRuntime *object, s32 priority);
void Func_02000a0c(struct ObjectRuntime *object);

void Func_02000adc(void)
{
    struct ObjectRuntime *object = Func_02005140(0x11d, 0x2b80000, 0x1c0000, 0x9c0000);

    if (object) {
        s32 timer;

        object->bounce = Math_Cosine(((Random16Far() * 0x3000) >> 16) + 0x3000);
        object->gravity = (u16)Random16Far();
        object->unknown_4c = (u16)(Random16Far() * 2) + 0x58000;
        timer = (u16)Random16Far() - 0x8000;
        object->action = timer;
        Func_02000930(object, 1);
        object->update = (void (*)(void))Func_02000a0c;
        Animation_SetStateFlags(object, 1);
    }
}
