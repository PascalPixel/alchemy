#ifndef ALCHEMY_ITEMGIFT_H
#define ALCHEMY_ITEMGIFT_H

#include "TYPES.H"

/* TBS field scripts and the field loop pass a second zero through this
 * entry's veneers. The callee leaves that argument unused; no flag meaning
 * is established by its current consumers. */
s32 PartyInventory_GiveItem(s32 item, s32 unused);
s32 PartyInventory_GiveItemFar(s32 item, s32 unused);

#endif
