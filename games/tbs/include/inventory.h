#ifndef ALCHEMY_INVENTORY_H
#define ALCHEMY_INVENTORY_H

/// @file
/// gOverflowItem, an item slot the spoils code uses when items are handed out.

#include "item.h"
#include "owner_state.h"
#include "party_state.h"
#include "farcall.h"

/// The item the battle spoils could not fit in the inventory. It is
/// gGameState.pendingItem under a second name (system/ewram.s places it at
/// that offset), which Map_Run gives on the return to the field when
/// the party has room.
extern u16 gOverflowItem;

#endif
