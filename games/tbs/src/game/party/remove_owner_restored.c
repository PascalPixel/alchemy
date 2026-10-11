#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "btlsys.h"
#include "party_state.h"
#include "battle_unit.h"

static __inline__ s32 Vitals_Ratio(s32 value, s32 max)
{
    s32 ratio = (value << 14) / max;
    s32 clamped = 0x4000;

    if (ratio <= 0x4000) {
        clamped = 0;
        if (ratio >= 0)
            clamped = ratio;
    }
    return clamped;
}

void Party_RemoveOwnerRestored(s32 owner)
{
    struct BtlUnit *vitals;
    s32 i;
    s32 count;
    s32 standing;

    Party_RemoveActiveOwnerFar(owner);
    Event_ClearInvalidPackedValues();
    vitals = UnitGetFar(owner);
    Unit_Recover(vitals);
    vitals->hp_gauge = Vitals_Ratio(vitals->hp, vitals->max_hp);
    if (vitals->hp_gauge == 0 && vitals->hp != 0)
        vitals->hp_gauge = 1;
    vitals->ep_gauge = Vitals_Ratio(vitals->ep, vitals->max_ep);
    if (vitals->ep_gauge == 0 && vitals->ep != 0)
        vitals->ep_gauge = 1;
    vitals->doku = 0;
    standing = 0;
    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++) {
        vitals = UnitGetFar(gGameState.active_owners[i]);
        if (vitals->hp != 0)
            standing++;
    }
    if (standing == 0) {
        vitals = UnitGetFar(gGameState.selected_actor);
        vitals->hp = 1;
        vitals->hp_gauge = Vitals_Ratio(vitals->hp, vitals->max_hp);
        if (vitals->hp_gauge == 0 && vitals->hp != 0)
            vitals->hp_gauge = 1;
        vitals->ep_gauge = Vitals_Ratio(vitals->ep, vitals->max_ep);
        if (vitals->ep_gauge == 0 && vitals->ep != 0)
            vitals->ep_gauge = 1;
    }
}
