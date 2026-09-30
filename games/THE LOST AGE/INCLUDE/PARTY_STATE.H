#ifndef ALCHEMY_PARTY_STATE_H
#define ALCHEMY_PARTY_STATE_H

#include "TYPES.H"

struct PartyState {
    u8 unknown_000[0x10];
    s32 coins;
    u8 unknown_014[0x1cc];
    s16 scene;
    s16 entrance;
    u8 unknown_1e4[0x30];
    s32 current_owner;
    u8 active_owners[8];
    u8 unknown_220[0x2b];
    /* 6 in the battles whose foes cannot fall below 1 HP. */
    u8 battle_rule_24b;
};

extern struct PartyState gPartyState;
s32 Party_CountActiveOwners(void);
s32 Party_ListActiveOwners(s16 *owners);

#endif
