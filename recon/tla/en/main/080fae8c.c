// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 6 of 217 in tla-en. First difference: this issues `lsls r3, r2, #1` earlier
// (row 21) than the native does (row 23). No single empty asm or one-pass block
// reaches 0.
// First draft of Func_080fae8c: the twin of ☀️'s ItemMenu_DrawEquipPreview
// (games/tbs/src/menu/menu/item.c). Shows in the status panel what the item in
// owner's slot would mean to member target, previewing it in a copy of
// target's unit when target is someone else. This game adds item type 10
// (style 9) and draws types 0 and 11 plainly.
//
// Near miss, differ 6 of 217, instruction order only: three times the native
// issues a load between the halves of a constant (the unit spill after the
// first Unit_Get, and 332 around the copy-back loads); here the halves stay
// together. As written plainly, slot and the buffer swap r9 and r11 (differ
// 38); the allocation helper in two of the three cases fixes that (permuter,
// 2026-10-19; in one or all three cases 38 or 42), and one 332 variable per
// case as ☀️ has it measures 34.
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

// FAKEMATCH(wrapper): the helper in the equipment and Psynergy cases only;
// as buffer = Heap_Alloc(332) in all three, 38 of 217 instructions differ.
static inline void *AllocUnitCopy(void) { return Heap_Alloc(332); }

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
    Func_080f8170(menu->statusWindow, target, slot, style);
    break;
  case 10:
    if (owner == target) {
      style |= 9;
      Func_080f8170(menu->statusWindow, target, slot, style);
    } else {
      unit = Unit_Get(target);
      buffer = Heap_Alloc(332);
      Iwram_CopyWords(buffer, unit, 332);
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
      Iwram_CopyWords(unit, buffer, 332);
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
      unit = Unit_Get(target);
      buffer = AllocUnitCopy();
      Iwram_CopyWords(buffer, unit, 332);
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
      Iwram_CopyWords(unit, buffer, 332);
      Heap_Release(buffer);
    }
    break;
  case 6:
    if (target == owner) {
      style |= 4;
      Func_080f8170(menu->statusWindow, target, slot, style);
    } else {
      unit = Unit_Get(target);
      buffer = AllocUnitCopy();
      Iwram_CopyWords(buffer, unit, 332);
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
      Iwram_CopyWords(unit, buffer, 332);
      Heap_Release(buffer);
    }
    break;
  }
}
