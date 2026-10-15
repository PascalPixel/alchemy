#include "local.h"

void Func_02000280(void)
{
    Func_02005308(11, 27);
}

u32 Func_0200028c(struct ObjectRuntime *object)
{
    s32 left;

    left = --*(s16 *)&object->unk66;
    return (u32)(-left | left) >> 31;
}

s32 Func_020002a0(struct ObjectRuntime *object)
{
    if (object->action != 0) {
        object->action--;
    } else {
        Func_020052c8((s16)object->unk66, 0x105, 0);
        object->action = ((Random16Far() * 240) >> 16) + 240;
    }
    return 1;
}
