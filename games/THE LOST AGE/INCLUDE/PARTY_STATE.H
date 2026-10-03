#ifndef ALCHEMY_PARTY_STATE_H
#define ALCHEMY_PARTY_STATE_H

#include "TYPES.H"

struct PartyState {
    u8 unknown_000[0x10];
    s32 coins;
    u8 unknown_014[0x124];
    /* A second six-digit counter, as ☀️'s Party_AdjustSixDigitCounterB keeps. */
    s32 counter_b;
    /* A counter capped at 28. */
    s8 counter_c;
    u8 unknown_13d[0xa3];
    s16 scene;
    s16 entrance;
    u8 unknown_1e4[4];
    /* The debug selection's remembered pair; ☀️ keeps it at 0x1c8. */
    s16 primary;
    s16 secondary;
    u8 unknown_1ec[2];
    /* Two halfword pairs; ☀️ keeps its first pair at 0x1ce. */
    u16 pair_1ee[2];
    u16 pair_1f2[2];
    /* The halfword Event_GetSpecialValue reads; ☀️ keeps it at 0x1d6. */
    s16 special;
    u8 unknown_1f8[0x1c];
    s32 current_owner;
    u8 active_owners[8];
    u8 unknown_220[0x20];
    /* Two packed event ids, cleared once they fail validation. */
    u16 packed_events[2];
    u8 unknown_244[7];
    /* 6 in the battles whose foes cannot fall below 1 HP. */
    u8 battle_rule_24b;
    u8 unknown_24c[0x1d];
    /* What BattleEffect_PauseObject keeps of the object it pauses; ☀️ keeps
       them 0x20 earlier. */
    u8 saved_byte;
    u8 unknown_26a[6];
    u32 saved_callback;
};

extern struct PartyState gPartyState;
s32 Party_CountActiveOwners(void);
s32 Party_ListActiveOwners(s16 *owners);
void Party_AdvanceOwnerCountToTarget(s32 owner, s32 level);

#endif
