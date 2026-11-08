#include "types.h"
#include "ram_buffer.h"
#include "item.h"
#include "battle_unit.h"
#include "game.h"
#include "owner_state.h"

struct InventoryMenuState {
  u8 unk000[0x1c4];
  u16 items[32];
  u8 unk204[0x10];
  u8 itemCount;
};

s32 ItemMenu_CollectItems(struct OwnerInventoryState *owner, u16 *items,
                          s32 mode);
s32 BattleEffect_ApplyToTargets(s32 effectId, s32 sourceId, s32 targetId,
                                s32 fixedScale);

s32 Func_08100738(s32 slot, s32 ownerId, s32 targetId) {
  struct InventoryMenuState *menu;
  s32 useResult;
  s32 itemId;
  s32 entry;
  struct OwnerInventoryState *unit;
  struct ItemDefinition *item;

  unit = Unit_Get(ownerId);
  menu = Ram_HeapSlots->menuRuntime;
  /* FAKEMATCH(temporary): the inventory entry read into its own local first, so
   * the barrier sits between the mask and the and; as itemId &=
   * unit->inventory[slot] after the asm, 24 of 90 instructions differ in
   * tla-en. */
  entry = unit->inventory[slot];
  itemId = ITEM_ID_MASK;
  /* FAKEMATCH(barrier): an empty asm that claims to change ownerId and
   * clobbers r2, so reload reads the mask back from sl instead of reusing r2;
   * as the same code without it, 22 of 90 instructions differ in tla-en. */
  __asm__("" : "+l"(ownerId) : : "r2");
  itemId &= entry;
  item = Item_Get(itemId);
  useResult = BattleEffect_ApplyToTargets(OWNER_ACTION_ID_MASK & item->actionId,
                                          ownerId, targetId, 1);
  if (useResult != -1) {
    item = Item_Get(unit->inventory[slot]);
    if (item->useType == 1) {
      Inventory_RemoveFar(ownerId, slot);
      menu->itemCount = ItemMenu_CollectItems(unit, menu->items, 0);
    }
    if (item->useType == 4) {
      if (itemId == 184)
        itemId = 185;
      unit->inventory[slot] = itemId;
    }
    useResult = 0;
  }
  return useResult;
}
