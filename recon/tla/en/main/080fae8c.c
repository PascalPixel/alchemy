// Near miss, differ 2 of 217 (tla-en, 2026-10-30): scheduler order only. After
// the first Unit_Get the native stores the unit spill (`str r0, [sp]`) before
// `lsls r3, r2, #1` (row 23); here the shift comes first (row 21): sched2 ranks
// the shift 8 and the store 7, since the store leads only to the ldrh.
// First draft of Func_080fae8c: the twin of ☀️'s ItemMenu_DrawEquipPreview
// (games/tbs/src/menu/menu/item.c). Shows in the status panel what the item in
// owner's slot would mean to member target, previewing it in a copy of
// target's unit when target is someone else. This game adds item type 10
// (style 9) and draws types 0 and 11 plainly.
//
// One size variable per case, as ☀️ has it, makes 332 an unallocated
// constant that reload rebuilds at each use, so the copy-back issues the
// address and argument loads between the halves of 332 as the native does
// (with a fresh 332 per call the halves stayed together: differ 6 with the
// former Heap_Alloc wrapper in two cases). It also shortens buffer's life so
// that buffer outranks slot and they swap r9 and r11 (differ 34); the dead
// test of menu in cases 0 and 11 gives slot the reference that ranks it first
// again (permuter, 2026-10-30). Earlier: no single empty asm or one-pass block
// reached 0 (2026-10-26).
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

struct PreviewMenu {
  u8 unk000[0x28];
  void *statusWindow;
};

struct PreviewUnit {
  u8 unk000[0xd8];
  u16 inventory[15];
};

struct PreviewItem {
  u8 unk00[2];
  u8 type;
};

void *Unit_Get(s32 owner);
struct PreviewItem *Item_Get(s32 item);
void *Heap_Alloc(s32 size);
void Heap_Release(void *buffer);
s32 Inventory_RemoveFirstUnflagged(s32 owner);
s32 Inventory_AddItemFar(s32 owner, s32 item);
void Func_080f8170(void *window, s32 target, s32 slot, s32 style);

void Func_080fae8c(s32 owner, s32 slot, s32 mode, s32 target) {
  struct PreviewMenu *menu = Ram_HeapSlots->menuRuntime;
  s32 style = 0;
  struct PreviewUnit *unit = Unit_Get(owner);
  s32 entry = unit->inventory[slot];
  s32 previewSlot;
  void *buffer;
  struct PreviewItem *item = Item_Get(entry & 0x1ff);

  if (mode == 1)
    style = 0x100;
  switch (item->type) {
  case 0:
  case 11:
    /* FAKEMATCH(dead): the test of menu, whose two branches make the same call,
     * kept; as the one call, 34 of 217 instructions differ in tla-en. */
    if (menu)
      Func_080f8170(menu->statusWindow, target, slot, style);
    else
      Func_080f8170(menu->statusWindow, target, slot, style);
    break;
  case 10:
    if (owner == target) {
      style |= 9;
      Func_080f8170(menu->statusWindow, target, slot, style);
    } else {
      s32 size;

      unit = Unit_Get(target);
      size = 332;
      buffer = Heap_Alloc(size);
      Iwram_CopyWords(buffer, unit, size);
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        entry &= ~0x200;
        previewSlot = Inventory_AddItemFar(target, entry);
        if (previewSlot != -1) {
          style |= 9;
          Func_080f8170(menu->statusWindow, target, previewSlot, style);
        } else {
          Func_080f8170(menu->statusWindow, target, slot, style);
        }
      } else {
        Func_080f8170(menu->statusWindow, target, slot, style);
      }
      Iwram_CopyWords(unit, buffer, size);
      Heap_Release(buffer);
    }
    break;
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 7:
  case 8:
  case 9:
    if (owner == target) {
      style |= 2;
      Func_080f8170(menu->statusWindow, target, slot, style);
    } else {
      s32 size;

      unit = Unit_Get(target);
      size = 332;
      buffer = Heap_Alloc(size);
      Iwram_CopyWords(buffer, unit, size);
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        entry &= ~0x200;
        previewSlot = Inventory_AddItemFar(target, entry);
        if (previewSlot != -1) {
          style |= 2;
          Func_080f8170(menu->statusWindow, target, previewSlot, style);
        } else {
          Func_080f8170(menu->statusWindow, target, slot, style);
        }
      } else {
        Func_080f8170(menu->statusWindow, target, slot, style);
      }
      Iwram_CopyWords(unit, buffer, size);
      Heap_Release(buffer);
    }
    break;
  case 6:
    if (target == owner) {
      style |= 4;
      Func_080f8170(menu->statusWindow, target, slot, style);
    } else {
      s32 size;

      unit = Unit_Get(target);
      size = 332;
      buffer = Heap_Alloc(size);
      Iwram_CopyWords(buffer, unit, size);
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        previewSlot = Inventory_AddItemFar(target, entry);
        if (previewSlot != -1) {
          style |= 4;
          Func_080f8170(menu->statusWindow, target, previewSlot, style);
        } else {
          Func_080f8170(menu->statusWindow, target, slot, style);
        }
      } else {
        Func_080f8170(menu->statusWindow, target, slot, style);
      }
      Iwram_CopyWords(unit, buffer, size);
      Heap_Release(buffer);
    }
    break;
  }
}
