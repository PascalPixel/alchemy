#ifndef ALCHEMY_UNIT_GAME_CHARACTER_DJINN_H
#define ALCHEMY_UNIT_GAME_CHARACTER_DJINN_H

#include "types.h"

const u16 *Djinn_GetDefinition(u32 group, u32 index);
s32 Djinn_AddToLeastLoadedOwner(s32 index, u8 *state);
s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit);
s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit);
u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
s32 Djinn_Activate(s32 owner, s32 index, s32 bit);
u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit);
s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit);
u32 *Trade_AddOffer(u32 kind, u32 first, u32 second);
s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target);
s32 Trade_CountPendingOffers(u8 *counts);
s32 Djinn_GetDefinitionHeader(u32 group, u32 index);
s32 Party_SumDjinnCounts(s32 index);

#endif
