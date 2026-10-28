#include "anemos_inner_sanctum_entrance.h"

extern const s8 gData_02000ee0[];

void Func_020004e4(s32 a, s32 b) {
  s32 i;

  const s8 *x = gData_02000ee0;
  const s8 *z = gData_02000ee0 + 1;

  for (i = 9; i <= 14; i++) {
    struct ObjectRuntime *object = Object_GetById(i);

    if (object->x >> 20 != *x)
      return;
    {
      s32 cellZ = *z;

      z += 2;
      x += 2;
      if (object->z >> 20 != cellZ)
        return;
    }
  }
  GameFlag_SetBit(0x200);
  Func_02000d64(201);
  Func_02000be0(1);
  Func_02000494();
}

void Func_02000534(s32 a) {
  struct Adventure *party;
  s32 held = Func_02000d4c();

  party = &gAdventure;
  if (held) {
    if (Func_02000d0c(party->currentOwner) != -1) {
      Func_02000d54();
      Func_020004e4(0, a);
    }
  } else {
    if (Object_GetById(party->currentOwner)->y > -0x1c0000)
      Func_02000d14();
  }
}
