#include "effect_0809b11c.h"
#include "global_cells.h"

#define BattleEffect_UpdateAllEffectSlots Func_08096f8c

void BattleEffect_UpdateAllEffectSlots(void) {
    s32 p;
    s32 cnt;

    p = *(s32 *)ADDR_03001F30 + 0x58;
    cnt = 0x17;
    do {
        cnt -= 1;
        EffectSlot_Update((struct EffectSlot *)p);
        p += 0x48;
    } while (cnt >= 0);
}
