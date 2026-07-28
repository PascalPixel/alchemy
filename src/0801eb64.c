#include "render_input.h"
#include "types.h"

s32 Func_0801bc34(s32 mode, s32 value);
void *Func_0801eb64(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 no;

    no = Func_0801bc34(arg0, arg1);
    if (no < 0) {
        return NULL;
    }
    return Func_0801eadc(no, 0x40000000, arg2, arg3, arg4);
}
