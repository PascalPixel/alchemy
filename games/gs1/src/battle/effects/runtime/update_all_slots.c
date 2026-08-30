#include "effect_0809b11c.h"
#include "global_cells.h"

void BattleEffect_UpdateAllSlots(void) {
    s32 slot;
    s32 remaining_slots;

    slot = *(s32 *)ADDR_03001F30 + 0x58;
    remaining_slots = 0x17;
    do {
        remaining_slots -= 1;
        EffectSlot_Update((struct EffectSlot *)slot);
        slot += 0x48;
    } while (remaining_slots >= 0);
}
