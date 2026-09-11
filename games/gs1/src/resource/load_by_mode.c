#include "types.h"
#include "scene.h"
#include "abi/resource/load_by_mode.h"

extern s32 Resource_FindFreeSlot(void);

s32 Resource_LoadByMode(s32 mode, s32 value)
{
    s32 output;
    s32 result = -1;

    switch (mode) {
    case 1:
    case 6:
        Sys_SetRange(value, 0, &result, &output, 0);
        break;
    case 2:
        result = Resource_FindFreeSlot();
        if (result == 0x60)
            return -1;
        Sys_Place(value, 0x1a, result);
        break;
    case 9:
        result = Resource_FindFreeSlot();
        if (result == 0x60)
            return -1;
        Sys_Place2(value, 0, result);
        break;
    case 4:
        Sys_SetRange2(value, 1, &result, &output, 0);
        break;
    }
    return result;
}
