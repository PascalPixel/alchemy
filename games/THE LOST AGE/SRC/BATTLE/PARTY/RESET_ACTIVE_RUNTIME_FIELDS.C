#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "OWNER_STATE.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_RUNTIME.H"
#include "TYPES.H"
#include "BATTLE_TYPES.H"


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
            unit->maboroshi = 0;
            unit->konran = 0;
            unit->charm = 0;
            unit->shibire = 0;
            unit->nemuri = 0;
            unit->fuuji = 0;
            unit->refrain = 0;
            unit->reflect = 0;
            unit->shiryo = 0;
            unit->unknown_142[0] = 0;
            unit->unknown_142[1] = 0;
            unit->sonae = 0;
            unit->miugoki = 0;
            unit->agi_cnt = 0;
            unit->agi_lv = 0;
            unit->battle_end_state = 0;

            BattleUnit_Recalculate(owners[i]);
            i++;
        } while (i < count);
    }
}
