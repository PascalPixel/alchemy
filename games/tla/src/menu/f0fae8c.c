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
