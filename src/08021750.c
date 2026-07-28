#include "render_input.h"
#include "types.h"

s32 Func_08004080(void);
void Func_080216e8(s32 index, s32 value, s32 flag);
void *Func_08021750(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 index;
    struct RenderOutput *entity;

    index = Func_08004080();
    entity = NULL;
    if (index != 0x60) {
        Func_080216e8(arg0, index, arg1);
        entity = Func_0801eadc(index, 0x80000000, arg2, arg3, arg4);
        ((u8 *)&entity->packed)[1] |= 0x20;
        entity->sentinel = 0xfb;
    }
    return entity;
}
