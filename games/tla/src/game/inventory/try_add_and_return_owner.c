/// @file
/// Inventory: give an item and return its owner.

#include "inventory.h"
#include "scene.h"

s32 Inventory_AddItemFar(s32, s32);

/// Gives the item. Returns the character ID on success, or -1 if the
/// inventory is full.
/// item: item ID.
/// unused: unused.
/// owner: recipient character ID.
s32 Inventory_TryAddAndReturnOwner(s32 item, s32 unused, s32 owner) {
  if (Inventory_AddItemFar(owner, item) >= 0)
    return owner;
  return -1;
}

/// Does nothing.
void Inventory_NoOpCallback(void) {}
