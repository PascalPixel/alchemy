#include "render_input.h"
#include "types.h"

#define M2C_FIELD(addr, type, offset) (*(type)((char *)(addr) + (offset)))

s32 Func_08004080(void);
void Func_080216e8(s32 index, s32 value, s32 flag);
void *Func_08021750(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 temp_r0;
    u8 *var_r0;

    temp_r0 = Func_08004080();
    var_r0 = NULL;
    if (temp_r0 != 0x60) {
        Func_080216e8(arg0, temp_r0, arg1);
        var_r0 = Func_0801eadc(temp_r0, 0x80000000, arg2, arg3, arg4);
        M2C_FIELD(var_r0, u8 *, 0x15) =
            (u8)(M2C_FIELD(var_r0, u8 *, 0x15) | 0x20);
        M2C_FIELD(var_r0, u8 *, 0x0f) = 0xfb;
    }
    return var_r0;
}
