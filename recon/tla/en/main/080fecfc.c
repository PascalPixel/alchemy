// Draft of Func_080fecfc: moves the party member at selectedIndex one place
// later (direction 1) or earlier (0) in the party order and returns 1, or 0
// when it cannot move; the twin of The Broken Seal's
// CharacterSelector_MoveEntry (games/tbs/src/menu/menu/status.c), with ⚓️'s
// menu work (owners 0x204, count 0x215).
// Near miss, differ 2 of 129 (2026-10-19): in the first copy loop the native
// steps the owner pointer (adds r2, #2) before it reads the count again;
// here the volatile read comes first. A while loop, a walking pointer for
// the owners or an escaped count pointer is no nearer (2 to 20).
#include "types.h"
#include "ram_buffer.h"

struct CharacterMenuState {
  u8 unk000[0x204];
  u16 ownerIds[8];
  u8 unk214;
  u8 partyCount;
};

s32 Party_AddActiveOwnerFar(s32 ownerId);
s32 Party_ListActiveOwnersFar(u16 *ownerIds);
s32 Party_RemoveActiveOwnerFar(s32 ownerId);

s32 Func_080fecfc(s32 selectedIndex, s32 direction) {
  struct CharacterMenuState *menu = Ram_HeapSlots->menuRuntime;
  u32 reordered[14];
  s32 index;

  if (menu->partyCount <= 1)
    return 0;
  if (direction == 1) {
    if (selectedIndex == menu->partyCount - 1)
      return 0;
  } else {
    if (selectedIndex == 0)
      return 0;
  }

  {
    u32 zero = 0;
    u32 *clear = &reordered[13];

    do {
      *clear = zero;
      clear--;
    } while ((s32)clear >= (s32)reordered);
  }
  /* FAKEMATCH(volatile): the native reads the count again after each copy;
   * as the plain menu->partyCount, loop.c counts the copy down from a hoisted
   * count and 59 of 129 instructions differ in tla-en. */
  for (index = 0; index < *(volatile u8 *)&menu->partyCount; index++)
    reordered[index] = menu->ownerIds[index];

  if (direction == 1) {
    index = reordered[selectedIndex];
    reordered[selectedIndex] = reordered[selectedIndex + 1];
    reordered[selectedIndex + 1] = index;
  } else {
    index = reordered[selectedIndex];
    reordered[selectedIndex] = reordered[selectedIndex - 1];
    reordered[selectedIndex - 1] = index;
  }

  for (index = 0; index < menu->partyCount; index++)
    Party_RemoveActiveOwnerFar(menu->ownerIds[index]);
  for (index = 0; index < menu->partyCount; index++)
    Party_AddActiveOwnerFar(reordered[index]);
  menu->partyCount = Party_ListActiveOwnersFar(menu->ownerIds);
  return 1;
}
