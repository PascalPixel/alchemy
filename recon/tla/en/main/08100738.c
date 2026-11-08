// Matches: differ 0 (tla-en, 2026-11-08)
// Two tagged devices, a barrier and a temporary. The other editions' copies
// match as well (2026-11-08): ja 0x08100740, de 0x0810a904, es 0x0810688c, fr
// 0x081028a8 and it 0x08100854 differ 0 of 90. Draft of Item_Use
// (Func_08100738): applies the slot's action, then consumes or transforms the
// item on success; the twin of The Broken Seal's Item_Use
// (games/tbs/src/menu/menu/use.c), rewritten with ⚓️'s calls and menu work
// (items 0x1c4, count 0x214). Inventory_RemoveFar is declared as game.h
// declares it (returning s32), which replaced an earlier r1 pin.
// What the device does: the native builds the mask in r2 after Unit_Get,
// stores it to sl and reads it back into r1 for the and. Without the asm,
// reload inherits the mask's output reload (r2) for the and's in-out reload
// of itemId and drops the store (22 of 90). An r2 clobber between the mask and
// the and stops that; alone (`__asm__("" ::: "r2")`, volatile) it is also a
// scheduling barrier and leaves `mov sl, r2` above the ldrh (4); tied to
// ownerId, which nothing near it reads, it is not, and the schedule is the
// native's. The entry is read into its own local first so the asm can sit
// between the mask and the and (without it, the slot's reload follows the
// mask's and takes r2: 24).
// Plain forms tried (2026-11-08), none below 12: the mask as itemId's initial
// value (12: built before Unit_Get, in r1); assigned after the call, before or
// after the menu load (24); in one expression, either order, as the Broken
// Seal twin writes it (22); a mask local set at the top or after the call and
// copied in (24, combine folds the constant into the copy) or used in the and
// (60); `*&itemId = ITEM_ID_MASK` (24); u16 itemId (27 to 39); u32 (12 to 22);
// the and carried by a second local (12 to 46). Milder devices: empty asm
// barriers without the clobber (18 to 26).
// Compiler note (K3, not acted on): the native's store-and-reload is what a
// stock movsi gives a constant headed for a high register, an input reload,
// which reload never inherits (matched f0ce61c's `target = selector & 0xff`
// compiles that way). -mtune=gba's *thumb_movsi_gba_const allows only an
// output reload, which the next reload of the same pseudo inherits. Whether a
// tune that built such constants through movsi's input reload would let the
// one-expression form match without the device is unmeasured.
#include "types.h"
#include "ram_buffer.h"
#include "item.h"
#include "battle_unit.h"
#include "game.h"

struct InventoryMenuState {
  u8 unk000[0x1c4];
  u16 items[32];
  u8 unk204[0x10];
  u8 itemCount;
};

struct BattleUnit *Unit_Get(s32 owner);
s32 ItemMenu_CollectItems(struct BattleUnit *owner, u16 *items, s32 mode);
s32 BattleEffect_ApplyToTargets(s32 effectId, s32 sourceId, s32 targetId,
                                s32 fixedScale);

s32 Item_Use(s32 slot, s32 ownerId, s32 targetId) {
  struct InventoryMenuState *menu;
  s32 useResult;
  s32 itemId;
  s32 entry;
  struct BattleUnit *unit;
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
