#include "TYPES.H"
#include "BATTLE_RUNTIME.H"

u8 *Item_GetData(u16);

void BattleUnit_ResetStateByMode(s32 id, s32 mode)
{
    struct BattleUnit *unit;

    unit = BattleUnit_Get(id);
    if (mode == 0) {
        unit->hp = unit->max_hp;
        BattleUnit_UpdateRatios(id);
    } else if (mode == 1) {
        unit->poison = 0;
    } else if (mode == 2) {
        unit->evil_spirit = 0;
    } else if (mode == 3) {
        s32 i;

        for (i = 0; i < 15; i++) {
            if (unit->inventory[i] & 0x200) {
                if (Item_GetData(unit->inventory[i])[3] & 1) {
                    unit->inventory[i] ^= 0x200;
                    BattleUnit_Recalculate(id);
                }
            }
        }
    }
}
