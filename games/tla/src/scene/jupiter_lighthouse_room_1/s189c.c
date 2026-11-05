#include "../jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"

extern u8 gData_020023a0[];
void Func_020014fc();
void Func_0200156c();

s32 Func_0200189c(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct JupiterLighthouseAerieWork *work =
      (struct JupiterLighthouseAerieWork *)gData_020023a0;
  struct JupiterLighthouseAerieEntry *entry =
      (struct JupiterLighthouseAerieEntry *)(work + 1);
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (GameFlag_Test(entry->flag) == 0) {
        Func_020014fc(work, entry);
        GameFlag_SetBit(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_0200156c(work, entry);
      GameFlag_SetByte(work->flag, x);
      GameFlag_SetByte(work->flag + 8, z);
      return 1;
    }
  }
  return 0;
}
