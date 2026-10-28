#include "scene.h"
#include "map.h"

void Func_020020e4(void) {
  struct FieldActor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 index = Func_020041e0(leader);
  struct MapCell *cell = map->layers[0].cells + index;
  struct SequenceParentWork *party;
  s32 y;

  if (cell->collisionCode == 20) {
    y = leader->y;
    asm volatile(
        ""); /* FAKEMATCH: ⚓️ reads the height before it builds 0x100000 */
    if (y > 0x100000) {
      gSequenceParentWork.unk2c3 = 1;
      return;
    }
  }
  party = &gSequenceParentWork;
  /* FAKEMATCH: ⚓️ loads gSequenceParentWork's address before it builds the field
   * offset */
  asm volatile("");
  party->unk2c3 = 0;
}
