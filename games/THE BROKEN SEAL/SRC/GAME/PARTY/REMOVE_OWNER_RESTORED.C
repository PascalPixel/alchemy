#include "TYPES.H"
#include "PARTY_STATE.H"

struct BattleUnit *Owner_GetStateFar(s32 owner);
void Party_RemoveActiveOwnerFar(s32 owner);
void Event_ClearInvalidPackedValues(void);
s32 Party_CountActiveOwnersFar(void);

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

/* Takes an owner out of the active party fully restored; if no active member
   is left standing, the current owner gets 1 HP. */
void Party_RemoveOwnerRestored(s32 owner)
{
    struct BattleUnit *vitals;
    s32 i;
    s32 count;
    s32 standing;

    Party_RemoveActiveOwnerFar(owner);
    Event_ClearInvalidPackedValues();
    vitals = Owner_GetStateFar(owner);
    /* FAKEMATCH: the do-while keeps both vital copies ahead of the ratio */
    do {
        vitals->hp = vitals->max_hp;
        vitals->pp = vitals->max_pp;
    } while (0);
    vitals->hp_gauge = Vitals_Ratio(vitals->hp, vitals->max_hp);
    if (vitals->hp_gauge == 0 && vitals->hp != 0)
        vitals->hp_gauge = 1;
    vitals->pp_gauge = Vitals_Ratio(vitals->pp, vitals->max_pp);
    if (vitals->pp_gauge == 0 && vitals->pp != 0)
        vitals->pp_gauge = 1;
    vitals->poison = 0;
    standing = 0;
    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++) {
        vitals = Owner_GetStateFar(gGameState.active_owners[i]);
        if (vitals->hp != 0)
            standing++;
    }
    if (standing == 0) {
        vitals = Owner_GetStateFar(gGameState.selected_actor);
        vitals->hp = 1;
        vitals->hp_gauge = Vitals_Ratio(vitals->hp, vitals->max_hp);
        if (vitals->hp_gauge == 0 && vitals->hp != 0)
            vitals->hp_gauge = 1;
        vitals->pp_gauge = Vitals_Ratio(vitals->pp, vitals->max_pp);
        if (vitals->pp_gauge == 0 && vitals->pp != 0)
            vitals->pp_gauge = 1;
    }
}
