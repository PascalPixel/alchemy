#include "local.h"

s32 Math_Cosine(s32 angle);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Func_02000930(struct ObjectRuntime *object, s32 priority);
s32 Func_02000864(struct ObjectRuntime *object);

void Func_0200098c(s32 *position, s32 value)
{
    struct ObjectRuntime *object = Func_02005140_(0x11d, position[0], position[1], position[2] - 0x20000);

    if (object) {
        s32 timer;

        object->bounce = Math_Cosine(((Random16Far() * 0x3000) >> 16) + 0x3000);
        object->gravity = (u16)Random16Far();
        object->unknown_4c = (u16)(Random16Far() * 2) + value;
        timer = (u16)Random16Far() - 0x8000;
        object->action = timer;
        Func_02000930(object, 3);
        object->update = (void (*)(void))Func_02000864;
        Animation_SetStateFlags(object, 1);
    }
}
