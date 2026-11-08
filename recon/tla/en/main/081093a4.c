// Matches: differ 0 (tla-en, 2026-11-08). The Broken Seal's
// Shop_DrawEquipComparison with this game's Unit_Get, Item_Get and menu
// runtime slot, plain C with no device. What closed the last 12: the unit is
// the shared struct BattleUnit, whose word fields (djinnAvailable onwards) put
// int in its alias set, so sched2 keeps each stat load behind the s32 store
// to with[] or without[] before it and the restore store behind with[3].
// The private ShopUnit (bytes and halfwords only) let them pass; the two
// once-only blocks earlier rounds added made up for part of that and are gone
// (kept with this struct, differ 14). Editions: tla-en matches; ja, fr and it
// keep it at .rom.001093a4, de at .rom.001113a4 and es at .rom.0010d3a4,
// scaffold sections diff cannot read. This draft compiled for each of them
// differs from its ROM only at the same 18 relocation sites (calls and the
// three message words), and the six ROMs differ from each other only there, so
// it can be adopted in all six. The three message names are not in the catalogs
// yet: 04667 (Can't equip), 04677 (ATK, the first stat label) and 00607 (the
// first item name).
#include "types.h"
#include "battle_unit.h"
#include "item.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ShopWork {
  u8 unk000[0x4f4];
  u16 statUpIcon;
  u16 statDownIcon;
};

extern u8 MsgShopCannotEquip[];
extern u8 MsgShopStatLabel[];
extern u8 MsgItemName[];

struct BattleUnit *Unit_Get(s32 unitId);
void Unit_CalculateFar(s32 unitId);
s32 Item_CanOwnerEquip(s32 owner, s32 item);
s32 Inventory_FindEquippedFar(s32 unitId, s32 type);
void WindowSprite_PrepareForRedrawFar(s32 window);
void Text_DrawMessageFar(s32 message, s32 window, s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x,
                                s32 y);
void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Window_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width, s32 style);
struct WindowSprite *WindowSprite_CreateFar(s32 code, s32 layers,
                                            struct RenderInput *input, s32 x,
                                            s32 y);

void Func_081093a4(s32 window, s32 unitId, s32 itemId) {
  struct ShopWork *shop = Ram_HeapSlots->menuRuntime;
  struct BattleUnit *unit = Unit_Get(unitId);
  struct ItemDefinition *item = Item_Get(itemId);
  s32 replaced = -1;
  s32 slot;
  s32 saved;
  s32 i;
  s32 without[4];
  s32 with[4];

  if (window == 0)
    return;
  WindowSprite_PrepareForRedrawFar(window);
  if (!Item_CanOwnerEquip(unitId, itemId)) {
    Text_DrawMessageFar((s32)MsgShopCannotEquip, window, 8, 24);
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
  unit->inventory[slot] = saved;
  Unit_CalculateFar(unitId);
  without[0] = unit->atk;
  without[1] = unit->def;
  without[2] = unit->agi;
  without[3] = unit->luck;
  for (i = 0; i <= 2; i++) {
    if (without[i] > with[i])
      WindowSprite_CreateFar(shop->statDownIcon, 0x40000000,
                             (struct RenderInput *)window, 56, i * 16 - 4)
          ->kind = 0;
    else if (without[i] < with[i])
      WindowSprite_CreateFar(shop->statUpIcon, 0x40000000,
                             (struct RenderInput *)window, 56, i * 16 - 4)
          ->kind = 0;
    Text_DrawNumberInWindowFar(without[i], 3, window, 32, i * 16);
    if (without[i] != with[i])
      Text_DrawNumberInWindowFar(with[i], 3, window, 72, i * 16);
    Text_DrawCharacterAtOffsetFar((s32)MsgShopStatLabel + i, window, 0, i * 16);
    Window_DrawDividerLineFar(window, 0, i * 2 + 2, 13, i * 2 + 2);
  }
  if (replaced != -1)
    Text_DrawCharacterAtOffsetFar(replaced + (s32)MsgItemName, window, 0, 48);
}
