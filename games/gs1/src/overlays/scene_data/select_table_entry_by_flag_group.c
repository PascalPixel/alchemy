#include "types.h"

extern s32 Func_02007764(s32);

extern s32 Data_0200db08[];

s32 SceneData_SelectTableEntryByFlagGroup(u32 sel)
{
    s32 base = 0;
    u32 i;

    switch (sel) {
    case 0:
        base = 0x92C;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (Func_02007764(base + i) != 0) return Data_0200db08[i];
    }
    return 0;
}
