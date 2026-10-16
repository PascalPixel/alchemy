#ifndef ALCHEMY_ITEM_GIFT_H
#define ALCHEMY_ITEM_GIFT_H

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

#if GAME_TLA
s32 PartyInventory_GiveItem(s32 item, s32 unused);
s32 PartyInventory_GiveItemFar(s32 item, s32 unused);
#endif

#endif
