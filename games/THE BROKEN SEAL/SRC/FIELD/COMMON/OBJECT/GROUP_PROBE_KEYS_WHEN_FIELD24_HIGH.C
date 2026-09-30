#include "TYPES.H"
#include "GLOBAL_CELLS.H"
extern u8 gBattleFxWork[];
extern u8 Data_03001ae8[];

void ObjectGroup_ProbeKeysWhenField24High(void)
{
    u8 *state = *(u8 **)((u32)&gBattleFxWork);
    u8 *object = *(u8 **)(state + 0x7828);

    if (*(s16 *)(object + 0x24) > 0x7f)
        (void)*(volatile s32 *)((u32)&Data_03001ae8);
}
