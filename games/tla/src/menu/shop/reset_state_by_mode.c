#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "btlsys.h"
#include "owner_state.h"

#define Item_Get ((u8 * (*)(u16))Item_Get)


void BattleUnit_ResetStateByMode(s32 id, s32 mode)
{
    struct BtlUnit *unit;

    unit = UnitGet(id);
    if (mode == 0) {
        unit->hp = unit->max_hp;
        UnitRatioFar(id);
    } else if (mode == 1) {
        unit->doku = 0;
    } else if (mode == 2) {
        unit->akuryo = 0;
    } else if (mode == 3) {
        s32 i;

        for (i = 0; i < 15; i++) {
            if (unit->inventory[i] & 0x200) {
                if (Item_Get(unit->inventory[i])[3] & 1) {
                    unit->inventory[i] ^= 0x200;
                    UnitCalcFar(id);
                }
            }
        }
    }
}
