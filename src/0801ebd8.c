#include "render_input.h"
#include "types.h"

s32 Func_08004080(void);
void Func_08019ee4(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
void *Func_0801ebd8(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 count;
    s32 unused;
    u8 *result;

    count = Func_08004080();
    if (count == 0x60) {
        return NULL;
    }
    Func_08019ee4(arg0, 1, &count, &unused, 1);
    result = Func_0801eadc(count, 0x40000000, arg1, arg2, arg3);
    result[15] = 251;
    return result;
}
