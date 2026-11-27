#ifndef ALCHEMY_INVENTORY_H
#define ALCHEMY_INVENTORY_H

/* OverflowItem, an item slot the spoils code uses when items are handed out. */

#include "edition.h"
#include "types.h"
#include "battle/battle_action_run.h"
#include "battle/playback_queue.h"
#include "game/item.h"
#include "stubs.h"
#include "system.h"
#include "game/main_routine_state.h"

/* The item the battle spoils could not fit in the inventory. It is
   MainRoutineStateWork.pendingItem under a second name (system/ewram.s places
   it at that offset), which fdModeRun gives on the return to the field when the
   party has room. */
extern Uint16 OverflowItem;

#endif
