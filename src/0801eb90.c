#include "render_input.h"
#include "types.h"

s32 Func_08004080();
s32 Func_0801a2a4(s32 arg0, s32 arg1, s32 arg2);
void *Func_0801eb90(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 no;
    void *result;

    no = Func_08004080();
    result = NULL;
    if (no != 0x60) {
        Func_0801a2a4(arg0, arg1, no);
        result = Func_0801eadc(no, 0x40000000, arg2, arg3, arg4);
    }
    return result;
}
