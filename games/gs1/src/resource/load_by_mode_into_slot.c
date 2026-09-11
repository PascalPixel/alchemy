#include "types.h"
#include "scene.h"
#include "abi/resource/load_by_mode_into_slot.h"

extern s32 Resource_FindFreeSlot(void);

s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option)
{
    s32 output;
    s32 original = result;

    if (result == -1) {
        result = Resource_FindFreeSlot();
        if (result == 0x60)
            return original;
    }

    switch (mode) {
    case 1:
    case 6:
        Sys_SetRange(value, option, &result, &output, 1);
        break;
    case 2:
        Sys_Place(value, 58, result);
        break;
    case 7:
        Sys_Place(value, 42, result);
        break;
    case 4:
        Sys_SetRange2(value, option, (s32)&result, (s32)&output, 1);
        break;
    case 8:
        Sys_Place2(value, 0, result);
        break;
    case 9:
        Sys_Place3(value, 0, (void *)result);
        break;
    }

    return result;
}
