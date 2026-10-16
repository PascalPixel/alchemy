// Draft: differ 76 (listing 76 instructions, draft 74). The native keeps the
// work in r6 and the index in r7 (the draft has them the other way round),
// and reads the count as a halfword before the loop, extending it at each
// test; the draft keeps the extended count in ip.
#include "../../../../games/tla/src/scene/jupiter_aerie/jupiter_aerie.h"

s32 Func_020027d8(void) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct JupiterAerieWork *work = (struct JupiterAerieWork *)gData_02003488;
  struct JupiterAerieEntry *entry = (struct JupiterAerieEntry *)(work + 1);
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (GameFlag_Test(entry->flag) == 0) {
        Func_02002438(work, entry);
        GameFlag_SetBit(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_020024a8(work, entry);
      GameFlag_SetByte(work->flag, x);
      GameFlag_SetByte(work->flag + 8, z);
      return 1;
    }
  }
  return 0;
}
