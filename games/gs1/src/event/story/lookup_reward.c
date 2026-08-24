#include "types.h"

extern s32 Func_02007708(s32);

extern s32 Data_0200e718[];

s32 StoryReward_LookupBySelection(u32 selection)
{
    s32 flag_base = 0;
    u32 offset;

    switch (selection) {
    case 0:
        flag_base = 0x92C;
        break;
    case 1:
        flag_base = 0x935;
        break;
    case 2:
        flag_base = 0x917;
        break;
    case 3:
        flag_base = 0x990;
        break;
    }
    for (offset = 0; offset < 9; offset++) {
        if (Func_02007708(flag_base + offset) != 0) return Data_0200e718[offset];
    }
    return 0;
}
