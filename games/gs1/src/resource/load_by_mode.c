#include "types.h"

extern s32 Resource_FindFreeSlot(void);
extern void Func_08019ee4(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
extern s32 Func_0801a2a4(s32 arg0, s32 arg1, s32 arg2);
extern s32 Func_0801a32c(u32 value, s32 unused, void *destination);
extern void Func_0801a3d0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

s32 Resource_LoadByMode(s32 mode, s32 value)
{
    s32 output;
    s32 result = -1;

    switch (mode) {
    case 1:
    case 6:
        Func_08019ee4(value, 0, &result, &output, 0);
        break;
    case 2:
        result = Resource_FindFreeSlot();
        if (result == 0x60)
            return -1;
        Func_0801a2a4(value, 0x1a, result);
        break;
    case 9:
        result = Resource_FindFreeSlot();
        if (result == 0x60)
            return -1;
        Func_0801a32c(value, 0, result);
        break;
    case 4:
        Func_0801a3d0(value, 1, &result, &output, 0);
        break;
    }
    return result;
}
