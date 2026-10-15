#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "owner_state.h"
#include "battle_party.h"
#include "battle_system.h"
#include "types.h"
#include "battle_type.h"


s32 BattleParty_ResetActiveRuntimeFields(void)
{
    u16 owners[10];
    s32 count;
    s32 i;

    count = BattleParty_PrepareActiveOwners(owners);
    count += BattleParty_PrepareReserveOwners(owners + count);

    i = 0;
    if (i < count) {
        struct BtlUnit *unit;
        u8 *cursor;
        s32 remaining;

        do {
            unit = UnitGet(owners[i]);
            remaining = 3;
            cursor = (u8 *)&unit->elm_lv[3];

            do {
                remaining--;
                *cursor-- = 0;
            } while (remaining >= 0);

            unit->atk_cnt = 0;
            unit->atk_lv = 0;
            unit->def_cnt = 0;
            unit->def_lv = 0;
            unit->res_cnt = 0;
            unit->res_lv = 0;
            unit->delusion = 0;
            unit->confusion = 0;
            unit->charm = 0;
            unit->stun = 0;
            unit->sleep = 0;
            unit->psy_seal = 0;
            unit->refrain = 0;
            unit->reflect = 0;
            unit->grim_reaper = 0;
            unit->unknown_142[0] = 0;
            unit->unknown_142[1] = 0;
            unit->ready = 0;
            unit->immobile = 0;
            unit->agi_cnt = 0;
            unit->agi_lv = 0;
            unit->challenge = 0;

            BattleUnit_Recalculate(owners[i]);
            i++;
        } while (i < count);
    }
}
