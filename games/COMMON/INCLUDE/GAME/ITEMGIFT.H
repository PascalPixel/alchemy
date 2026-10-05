#ifndef ALCHEMY_ITEMGIFT_H
#define ALCHEMY_ITEMGIFT_H

#include "TYPES.H"

/* TBS field callers pass an unused second argument of zero. */
s32 PartyInventory_GiveItem(s32 item, s32 unused);
s32 PartyInventory_GiveItemFar(s32 item, s32 unused);

#endif
