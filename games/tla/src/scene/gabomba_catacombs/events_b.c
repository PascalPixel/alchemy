#include "local.h"

void Func_020008a0(struct Actor *object, s32 dz);

void Func_0200094c(void) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  struct Actor *steam;

  if ((gFrameCount & 7) == 0) {
    steam = Func_0200115c(14, leader->x, leader->y + 0x60000, leader->z);
    if (steam) {
      Actor_SetSingleChildFlags(steam, 0);
      Func_0200114c(steam, 1);
      Func_02001154(steam, gData_0200128c);
    }
    Func_02001284(0x26f);
  }
  Func_020008a0(leader, 0xd999);
}

s32 Func_020009bc(struct Actor *object) {
  object->scaleX -= 3276;
  object->scaleY -= 3276;
  return 0;
}
