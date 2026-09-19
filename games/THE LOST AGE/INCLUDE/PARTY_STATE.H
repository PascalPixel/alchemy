#ifndef ALCHEMY_PARTY_STATE_H
#define ALCHEMY_PARTY_STATE_H

#include "TYPES.H"

struct PartyState {
    u8 unknown_000[0x214];
    s32 current_owner;
    u8 active_owners[8];
};

extern struct PartyState gPartyState;
s32 Party_CountActiveOwners(void);
s32 Party_ListActiveOwners(s16 *owners);

#endif
