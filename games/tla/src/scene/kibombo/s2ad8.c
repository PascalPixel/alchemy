#include "kibombo.h"

struct KibomboWork90 {
  u8 unk00[0x18];
  struct KibomboObject *leader;
};

void Func_02002ad8(void) {
  struct KibomboWork90 *work = Ram_HeapSlots->unk84[3];

  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the party
   * state's address before it builds the owner's offset (2 differ without it).
   */
  do {
  } while (0);
  work->leader = Actor_GetById(gMainRoutineWork.currentOwner);
}

void Func_02002af8(void) {
  struct KibomboWork90 *work = Ram_HeapSlots->unk84[3];

  work->leader = 0;
}

s32 Func_02002b08(s32 value) {
  if (value > 31)
    value = 31;
  if (value < 0)
    value = 0;
  return value;
}
