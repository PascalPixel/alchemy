#include "types.h"
#include "scene.h"

/* resource/load_by_mode.c */
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
        Sys_unk2_3(value, 0, result);
        break;
    case 4:
        Sys_unk2_5(value, 1, &result, &output, 0);
        break;
    }
    return result;
}

/* resource/load_by_mode_into_slot.c */
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
        Sys_unk2_5(value, option, (s32)&result, (s32)&output, 1);
        break;
    case 8:
        Sys_unk2_3(value, 0, result);
        break;
    case 9:
        Sys_unk3_3(value, 0, (void *)result);
        break;
    }

    return result;
}
