#include "TYPES.H"
#include "PARTY_STATE.H"
#include "OWNER_STATE.H"

struct OwnerVitals {
    u8 unknown_000[20];
    s16 hp_ratio;
    s16 pp_ratio;
    u8 unknown_018[28];
    s16 max_hp;
    s16 max_pp;
    s16 hp;
    s16 pp;
    u8 unknown_03c[0x131 - 60];
    u8 status;
};

s32 Event_ValidatePackedId(u32 packed);
s32 Math_Div(s32, s32);
void Party_RemoveActiveOwnerFar(s32 owner);
s32 Party_CountActiveOwnersFar(void);

/* Clears each of the party's two packed event ids that validates. */
void Event_ClearValidPackedIds(void)
{
    if (Event_ValidatePackedId(gPartyState.packed_events[0]))
        gPartyState.packed_events[0] = 0;
    if (Event_ValidatePackedId(gPartyState.packed_events[1]))
        gPartyState.packed_events[1] = 0;
}

static __inline__ s32 Vitals_Ratio(s32 value, s32 max)
{
    s32 ratio = Math_Div(value << 14, max);
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
    struct OwnerVitals *vitals;
    s32 i;
    s32 count;
    s32 standing;

    Party_RemoveActiveOwnerFar(owner);
    Event_ClearValidPackedIds();
    vitals = Owner_GetState(owner);
    /* FAKEMATCH: the do-while keeps both vital copies ahead of the ratio */
    do {
        vitals->hp = vitals->max_hp;
        vitals->pp = vitals->max_pp;
    } while (0);
    vitals->hp_ratio = Vitals_Ratio(vitals->hp, vitals->max_hp);
    if (vitals->hp_ratio == 0 && vitals->hp != 0)
        vitals->hp_ratio = 1;
    vitals->pp_ratio = Vitals_Ratio(vitals->pp, vitals->max_pp);
    if (vitals->pp_ratio == 0 && vitals->pp != 0)
        vitals->pp_ratio = 1;
    vitals->status = 0;
    standing = 0;
    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++) {
        vitals = Owner_GetState(gPartyState.active_owners[i]);
        if (vitals->hp != 0)
            standing++;
    }
    if (standing == 0) {
        vitals = Owner_GetState(gPartyState.current_owner);
        vitals->hp = 1;
        vitals->hp_ratio = Vitals_Ratio(vitals->hp, vitals->max_hp);
        if (vitals->hp_ratio == 0 && vitals->hp != 0)
            vitals->hp_ratio = 1;
        vitals->pp_ratio = Vitals_Ratio(vitals->pp, vitals->max_pp);
        if (vitals->pp_ratio == 0 && vitals->pp != 0)
            vitals->pp_ratio = 1;
    }
}
