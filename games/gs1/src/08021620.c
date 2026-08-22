#include "render_input.h"
#include "types.h"

s32 Func_08004080(void);
void Func_080215e0(s32 arg0, s32 arg1);
void *Func_08021620(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 index;
    struct RenderOutput *first;
    struct RenderOutput *second;

    index = Func_08004080();
    if (index > 95)
        return NULL;

    Func_080215e0(arg0, index);
    first = Func_0801eadc(index, 0x80004000, arg1, arg2, arg3);
    first->sentinel = 0xFD;
    second = Func_0801eadc(index, 0x80004000, arg1, arg2 + 32, arg3);
    second->sentinel = 0xFD;
    second->table.bits.index += 8;
    return first;
}
