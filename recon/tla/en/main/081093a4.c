// Near miss, differ 28 of 238 (2026-10-12): The Broken Seal's
// Shop_DrawEquipComparison with this game's Unit_Get, Item_Get and menu runtime
// slot. Every instruction and register is the native's; seven rows are in
// another order: (1) movs r3, #192 is started before sub sp and the unitId
// store; (2) and (3) the first inventory entry and the replaced entry are
// loaded after their masks are started (native: load first); (4) item_id |
// 0x200 is copied to r1 before the slot offset and the saved load; (5) in the
// with[] block mov r0, r8 comes one row late and adds r3, #66 before the pool
// branch; (6) in the without[] block each load comes one row before the loop's
// movs r5, mov r8 and movs r7; (7) both icon offsets are started before shop is
// reloaded from the stack. Tried: an empty asm barrier first in the function
// (the argument copies move up, worse), an entry temporary behind a barrier for
// (3) (the entry is spilled), a barrier after saved is read (differ 44), a
// barrier after without[3] (fixes none of (6), moves the loop setup below it),
// shop copied to a local behind an empty barrier for (7) (no change) or a
// register-use barrier (a new spill, differ 178). The three message names are
// not in the catalogs yet: 04667 (Can't equip), 04677 (ATK, the first stat
// label) and 00607 (the first item name).
#include "types.h"
#include "item.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ShopUnit {
  u8 unk00[60];
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 unk43[149];
  u16 inventory[15];
};

struct ShopWork {
  u8 unk000[0x4f4];
  u16 statUpIcon;
  u16 statDownIcon;
};

extern u8 MsgShopCannotEquip[];
extern u8 MsgShopStatLabel[];
extern u8 MsgItemName[];

struct ShopUnit *Unit_Get(s32 unitId);
void Unit_CalculateFar(s32 unitId);
s32 Item_CanOwnerEquip(s32 owner, s32 item);
s32 Inventory_FindEquippedFar(s32 unitId, s32 type);
void RenderOutput_PrepareForRedrawFar(s32 window);
void UiText_DrawResourceFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x,
                                  s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width,
                                 s32 style);
struct RenderOutput *RenderOutput_CreateFar(s32 code, s32 layers,
                                            struct RenderInput *input, s32 x,
                                            s32 y);

void Shop_DrawEquipComparison(s32 window, s32 unitId, s32 itemId) {
  struct ShopWork *shop = Ram_HeapSlots->menuRuntime;
  struct ShopUnit *unit = Unit_Get(unitId);
  struct ItemDefinition *item = Item_Get(itemId);
  s32 replaced = -1;
  s32 slot;
  s32 saved;
  s32 i;
  s32 without[4];
  s32 with[4];

  if (window == 0)
    return;
  RenderOutput_PrepareForRedrawFar(window);
  if (!Item_CanOwnerEquip(unitId, itemId)) {
    UiText_DrawResourceFar((s32)MsgShopCannotEquip, window, 8, 24);
    return;
  }
  slot = Inventory_FindEquippedFar(unitId, item->type);
  if (slot == -1) {
    for (i = 0; i <= 14; i++) {
      if (!(unit->inventory[i] & 0x200))
        break;
    }
    if (i == 15) {
      for (i = 0; i <= 14; i++) {
        if (Item_Get(unit->inventory[i])->type == 6)
          break;
      }
      if (i == 15)
        i = 0;
    }
    slot = i;
  } else {
    replaced = unit->inventory[slot] & 0x1ff;
  }
  saved = unit->inventory[slot];
  unit->inventory[slot] = itemId | 0x200;
  Unit_CalculateFar(unitId);
  with[0] = unit->atk;
  with[1] = unit->def;
  with[2] = unit->agi;
  with[3] = unit->luck;
  /* FAKEMATCH: the native stores luck before it copies the saved entry back
   * (differ 32 without the barrier, 28 with). */
  __asm__("");
  unit->inventory[slot] = saved;
  Unit_CalculateFar(unitId);
  without[0] = unit->atk;
  without[1] = unit->def;
  without[2] = unit->agi;
  without[3] = unit->luck;
  for (i = 0; i <= 2; i++) {
    if (without[i] > with[i])
      ((u8 *)RenderOutput_CreateFar(shop->statDownIcon, 0x40000000,
                                    (struct RenderInput *)window, 56,
                                    i * 16 - 4))[4] = 0;
    else if (without[i] < with[i])
      ((u8 *)RenderOutput_CreateFar(shop->statUpIcon, 0x40000000,
                                    (struct RenderInput *)window, 56,
                                    i * 16 - 4))[4] = 0;
    UiText_DrawNumberInWindowFar(without[i], 3, window, 32, i * 16);
    if (without[i] != with[i])
      UiText_DrawNumberInWindowFar(with[i], 3, window, 72, i * 16);
    UiText_DrawCharacterAtOffsetFar((s32)MsgShopStatLabel + i, window, 0,
                                    i * 16);
    UiWindow_DrawDividerLineFar(window, 0, i * 2 + 2, 13, i * 2 + 2);
  }
  {
    s32 old = replaced;

    /* FAKEMATCH: the native reloads the replaced item before it builds -1
     * (differ 34 without the temporary and barrier, 32 with). */
    __asm__("");
    if (old != -1)
      UiText_DrawCharacterAtOffsetFar(old + (s32)MsgItemName, window, 0, 48);
  }
}
