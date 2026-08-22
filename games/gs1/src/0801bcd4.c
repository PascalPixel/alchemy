#include "types.h"

extern s32 Func_08004080(void);
extern void Func_08019ee4(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
extern s32 Func_0801a2a4(s32 arg0, s32 arg1, s32 arg2);
extern s32 Func_0801a2ec(s32 arg0, s32 arg1, s32 arg2);
extern s32 Func_0801a32c(u32 value, s32 unused, void *destination);
extern void Func_0801a3d0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

s32 Func_0801bcd4(s32 mode, s32 value, s32 result, s32 option)
{
    s32 output;
    s32 original = result;

    if (result == -1) {
        result = Func_08004080();
        if (result == 0x60)
            return original;
    }

    switch (mode) {
    case 1:
    case 6:
        Func_08019ee4(value, option, &result, &output, 1);
        break;
    case 2:
        Func_0801a2a4(value, 58, result);
        break;
    case 7:
        Func_0801a2a4(value, 42, result);
        break;
    case 4:
        Func_0801a3d0(value, option, (s32)&result, (s32)&output, 1);
        break;
    case 8:
        Func_0801a2ec(value, 0, result);
        break;
    case 9:
        Func_0801a32c(value, 0, (void *)result);
        break;
    }

    return result;
}
