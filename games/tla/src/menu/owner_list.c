#include "types.h"
#include "ram_buffer.h"

struct InventoryMenuState {
  u8 unk000[0x204];
  u16 ownerIds[8];
  u8 unk214;
  u8 partyCount;
};

struct OwnerEntry {
  u8 unk00[20];
};

struct OwnerList {
  struct OwnerEntry entries[8];
  u8 results[8];
};

s32 Func_08103064(struct OwnerEntry *entry, s32 owner, s32 mode);

/// For each active party member of the menu work, fills a 20-byte entry of the
/// list with Func_08103064 (owner, -1) and keeps its result byte at offset 160
/// of the list. With the owner read as the argument the two walking pointers
/// swap r5 and r6 (differ 16).
void Func_081019a4(struct OwnerList *list) {
  struct InventoryMenuState *menu =
      (struct InventoryMenuState *)Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = 0; i < menu->partyCount; i++) {
    s32 owner = menu->ownerIds[i];

    list->results[i] = Func_08103064(&list->entries[i], owner, -1);
  }
}
