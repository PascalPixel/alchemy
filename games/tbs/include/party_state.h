#ifndef ALCHEMY_PARTY_STATE_H
#define ALCHEMY_PARTY_STATE_H

#include "layout_guard.h"

#include "game_state.h"

s32 Party_CountActiveOwners(void);
s32 Party_ListActiveOwners(s16 *owners);
void Party_AdvanceOwnerCountToTarget(s32 owner, s32 level);

#endif
