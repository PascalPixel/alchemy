// Near miss, differ 12 of 110 (23 when the objects were stored through a
// pointer stepped beside the index), instruction order only: the native
// spills first (str r0, [sp, #4]) before it builds the work address, builds
// the objects pointer's spill after the loop's first index copies, and loads
// the flags byte before building the -13 mask; agscc issues each a step
// later, the load and store pattern AGENTS.md records for ⚓️.
#include "types.h"
#include "ram_buffer.h"

struct MenuEntryWork {
  u8 unk000[30];
  u8 count;
  u8 unk01f[0xf8 - 0x1f];
  void *objects[8];
  u8 unk118[0x158 - 0x118];
  s16 ids[8];
};

void ResourceObject_ReleaseFar(void *object);
void *ResourceObject_CreateFar(s32 resource);
void Animation_ApplyChildArgumentFar(void *object, s32 argument);
s32 Party_ListActiveOwnersFar(u16 *owners);
s32 Func_080c82b8(s32 owner);

void Menu_CreateOwnerEntryObjects(s32 first) {
  struct MenuEntryWork *menu = Ram_HeapSlots->menuRuntime;
  u16 owners[14];
  u8 *object;
  s32 i;

  for (i = 0; i < 8; i++) {
    object = menu->objects[i];
    if (object != NULL) {
      ResourceObject_ReleaseFar(object);
      menu->objects[i] = NULL;
      menu->ids[i] = -1;
    }
  }
  menu->count = Party_ListActiveOwnersFar(owners);
  for (i = 0; i < 4 && owners[first + i] != 255; i++) {
    menu->ids[i] = owners[first + i];
    object = ResourceObject_CreateFar(Func_080c82b8(owners[first + i]));
    if (object != NULL) {
      Animation_ApplyChildArgumentFar(object, 1);
      ((s8 *)object)[9] &= -13;
      object[26] = 0;
    }
    menu->objects[i] = object;
  }
}
