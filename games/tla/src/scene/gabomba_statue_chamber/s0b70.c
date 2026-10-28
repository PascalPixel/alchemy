#include "gabomba_statue_chamber.h"
#include "input.h"

void Func_02000b70(void) {
  struct GabombaStatueChamberObject *leader =
      Object_GetById(gAdventure.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 26) {
    if (z == 5 && (gInput.held & 128) == 0)
      return;
    if (z == 7 && (gInput.held & 64) == 0)
      return;
  } else if (z == 6) {
    if (x == 25 && (gInput.held & 16) == 0)
      return;
    if (x == 27 && (gInput.held & 32) == 0)
      return;
  }
  Func_02000a98(424, 100);
}
