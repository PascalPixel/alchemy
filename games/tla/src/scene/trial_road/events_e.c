#include "trial_road.h"

void Func_02000768(void) {
  OBJECT_BYTE(Object_GetById(17), 98) = 0;
  GameFlag_SetBit(0x307);
}

void Func_02000784(void) { Func_020059c0(); }

void Func_0200078c(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  if (object != NULL)
    Map_Stamp(14, 27, 1, 1, object->x >> 20, object->z >> 20);
}

void Func_020007b4(void) {
  s32 y = Object_GetById(gAdventure.currentOwner)->y;

  /* FAKEMATCH: loads the leader height before it builds the threshold (differ 2
   * without it; an object pointer, a limit temporary, a const height, a height
   * inline and a subtraction measured the same). */
  do {
  } while (0);
  if (y == 0x200000)
    GameFlag_SetBit(0x208);
  else
    GameFlag_ClearBit(0x208);
}
