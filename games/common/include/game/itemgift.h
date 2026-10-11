#ifndef ALCHEMY_ITEMGIFT_H
#define ALCHEMY_ITEMGIFT_H

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

#if GAME_TLA
s32 PartyInventory_GiveItem(s32 item, s32 unused);
#endif
#if GAME_TLA
s32 PartyInventory_GiveItemFar(s32 item, s32 unused);
#endif

#endif
