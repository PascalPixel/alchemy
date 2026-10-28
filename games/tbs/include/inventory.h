#ifndef ALCHEMY_INVENTORY_H
#define ALCHEMY_INVENTORY_H

/// @file
/// gOverflowItem, an item slot the spoils code uses when items are handed out.

#include "item.h"
#include "game/unit.h"
#include "main_routine_work.h"
#include "farcall.h"

/// The item the battle spoils could not fit in the inventory. It is
/// gMainRoutineWork.pendingItem under a second name (cartridge/ewram.s
/// places it at that offset), which FieldSequence_Run gives on the return to
/// the field when the party has room.
extern u16 gOverflowItem;

#endif
