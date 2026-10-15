#include "yampi_desert_cave.h"

void Func_020013c8(s32 a)
{
    YampiDesertCave_Call1920(a, 64, 0x302);
}

const u8 *Func_020013d8(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov662Room2)
        return Data_02001ddc;
    if (gPartyState.scene == (s32)&SceneId_Ov662Room3)
        return Data_02001e78;
    return Data_02001d28;
}
