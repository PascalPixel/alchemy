/// @file
/// The bottle of Hermes' Water: Mercury Lighthouse's fountain fills the Empty
/// Bottle, and pouring it on Tret in Kolima Forest empties it again.

/// Replaces the first held oldItem with newItem in the same party member's
/// slot, so the bottle stays where it was in the items.
static void Inventory_ReplaceItem(s32 oldItem, s32 newItem) {
  s32 member = PartyInventory_FindMember(oldItem);

  if (member != -1) {
    s32 inventorySlot = Inventory_Find(member, oldItem);

    if (inventorySlot != -1) {
      Unit_Get(member)->inventory[inventorySlot] = newItem;
    }
  }
}
