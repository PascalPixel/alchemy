#ifndef ALCHEMY_INVENTORY_H
#define ALCHEMY_INVENTORY_H

// A unit's fifteen inventory slots: counting, finding, adding and taking items, and finding who in
// the party carries one.

#include "game/item.h"

#include "game/class_djinn.h"
#include "system.h"
#include "game/main_routine_state.h"

ITEM_DEFINITION *gmInventoryGetEquippedDefinition(UNIT *unit, Sint32 type);
Sint32           mnInventoryRemoveFirstUnflagged(Sint32 owner);

#endif
