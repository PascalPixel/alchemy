#include "render_input.h"
#include "types.h"

s32 Func_08004080();
s32 Func_0801a32c(u32 value, s32 unused, void *destination);
void *Func_0801ec24(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 temp_r0;
    void *var_r0;

    temp_r0 = Func_08004080();
    var_r0 = NULL;
    if (temp_r0 != 0x60) {
        Func_0801a32c(arg0, 0, temp_r0);
        var_r0 = Func_0801eadc(temp_r0, 0x40000000, arg1, arg2, arg3);
    }
    return var_r0;
}
