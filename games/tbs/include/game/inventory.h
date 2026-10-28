#ifndef ALCHEMY_INVENTORY_H
#define ALCHEMY_INVENTORY_H

/// @file
/// gOverflowItem, an item slot the spoils code uses when items are handed out.

#include "item.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "playback_queue.h"
#include "battle_action_run.h"
#include "main_routine_work.h"
#include "far_call.h"

/// The item the battle spoils could not fit in the inventory. It is
/// gMainRoutineWork.pendingItem under a second name (system/ewram.s
/// places it at that offset), which FieldSequence_Run gives on the return to
/// the field when the party has room.
extern u16 gOverflowItem;

#endif
