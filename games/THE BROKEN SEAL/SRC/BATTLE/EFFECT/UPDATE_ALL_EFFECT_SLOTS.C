#include "EFFECT_0809B11C.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001f30[];

void BattleFx_UpdateAllEffectSlots(void)
{
    s32 p;
    s32 cnt;

    p = *(s32 *)((u32)&Data_03001f30) + 0x58;
    cnt = 0x17;
    do {
        cnt -= 1;
        EffectSlot_Update((struct EffectSlot *)p);
        p += 0x48;
    } while (cnt >= 0);
}
