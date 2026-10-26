// Draft of Item_Use (Func_08100738): applies the slot's action, then consumes
// or transforms the item on success; the twin of The Broken Seal's Item_Use
// (games/tbs/src/menu/menu/use.c), rewritten with ⚓️'s calls and menu work
// (items 0x1c4, count 0x214).
// Differ 16 of 90 (2026-10-26; 26 before): the item id starts as the mask
// and is masked in place, so it is set twice and stays in its own high
// register, reached through a low copy for the and (mov r1, sl; ands r1, r3;
// mov sl, r1), as natively. Left: the native builds the mask after Unit_Get
// (here, as the declaration's initial value, before it: assigned after the
// call, the mask is built in the and's register and the copy into sl
// vanishes, 28), so the slot's copy for the inventory offset takes r1 there
// and r2 here, and two loads sit a row from the native's.
#include "types.h"
#include "ram_buffer.h"
#include "item.h"
#include "battle_unit.h"

struct InventoryMenuState {
  u8 unk000[0x1c4];
  u16 items[32];
  u8 unk204[0x10];
  u8 itemCount;
};

struct BattleUnit *Unit_Get(s32 owner);
void Inventory_RemoveFar(s32 ownerId, s32 slot);
s32 ItemMenu_CollectItems(struct BattleUnit *owner, u16 *items, s32 mode);
s32 BattleEffect_ApplyToTargets(s32 effectId, s32 sourceId, s32 targetId,
                                s32 fixedScale);

s32 Item_Use(s32 slot, s32 ownerId, s32 targetId) {
  struct InventoryMenuState *menu;
  s32 useResult;
  s32 itemId = ITEM_ID_MASK;
  struct BattleUnit *unit;
  struct ItemDefinition *item;

  unit = Unit_Get(ownerId);
  menu = Ram_HeapSlots->menuRuntime;
  itemId &= unit->inventory[slot];
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
      if (itemId == 0xb8)
        itemId = 0xb9;
      unit->inventory[slot] = itemId;
    }
    useResult = 0;
  }
  return useResult;
}
