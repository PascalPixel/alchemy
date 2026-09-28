#include "GROUP_DEPARTURE.H"

void HaidiaArashi_FlashLightning(void)
{
    u32 i;
    s32 record;

    Task_Wait(20);
    GameFlag_Set(0x166);
    Map_SetLayerEntryFlag(0);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Map_SetLayerEntryFlag(3);
    Map_SetLayerEntryFlag(4);
    Map_SetLayerEntryFlag(5);
    ColorBuffer_ApplyTarget(0x10003, 1);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(1);
    Task_Wait(120);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    GameFlag_Clear(0x166);
    Map_ClearLayerEntryFlag(0);
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
    Map_ClearLayerEntryFlag(3);
    Map_ClearLayerEntryFlag(4);
    Map_ClearLayerEntryFlag(5);
}
