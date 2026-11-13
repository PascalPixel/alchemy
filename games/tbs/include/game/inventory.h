#ifndef ALCHEMY_INVENTORY_H
#define ALCHEMY_INVENTORY_H

/// @file
/// gOverflowItem, an item slot the spoils code uses when items are handed out.

#include "game/item.h"
#include "types.h"
#include "edition.h"
#include "gamepak/far_call.h"
#include "battle/playback_queue.h"
#include "battle/battle_action_run.h"
#include "system/main_routine_work.h"

/// The item the battle spoils could not fit in the inventory. It is
/// gMainRoutineWork.pendingItem under a second name (system/ewram.s
/// places it at that offset), which FieldSequence_Run gives on the return to
/// the field when the party has room.
extern u16 gOverflowItem;

#endif
