#ifndef ALCHEMY_VAULT_ITEM_ICON_H
#define ALCHEMY_VAULT_ITEM_ICON_H

/* @file */
/* An item lying on the floor of Vault's rooms, drawn as its menu icon */
/* (shared/vault_item_icon.c). */

#include "fixed_math.h"
#include "types.h"

/* Places the item's icon at the 16.16 position. */
void scVaultPlaceItemIcon(s32 item, FIXED x, FIXED y, FIXED z);

#endif
