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
    u8 unknown_1e4[0xa];
    /* Two halfword pairs; ☀️ keeps its first pair at 0x1ce. */
    u16 pair_1ee[2];
    u16 pair_1f2[2];
    u8 unknown_1f6[0x1e];
    s32 current_owner;
    u8 active_owners[8];
    u8 unknown_220[0x20];
    /* Two packed event ids, cleared once they fail validation. */
    u16 packed_events[2];
    u8 unknown_244[7];
    /* 6 in the battles whose foes cannot fall below 1 HP. */
    u8 battle_rule_24b;
};

extern struct PartyState gPartyState;
s32 Party_CountActiveOwners(void);
s32 Party_ListActiveOwners(s16 *owners);

#endif
