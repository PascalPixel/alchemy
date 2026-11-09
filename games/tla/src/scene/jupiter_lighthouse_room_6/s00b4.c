#include "jupiter_lighthouse_room_6.h"
#include "event_inline.h"

void Func_020000b4(s32 mode) {
  if (mode == 1) {
    Event_PlaySound(133);
    MapGrid_CopyCellsLayered(84, 36, 2, 3, 78, 40);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(84, 39, 2, 3, 78, 40);
  MapGrid_CopyCellAttributes(20, 38, 2, 1, 14, 42);
}

void Func_02000108(void) {
  Event_PlaySound(134);
  MapGrid_CopyCellsLayered(84, 36, 2, 3, 78, 40);
  System_Wait(6);
  MapGrid_CopyCellsLayered(84, 42, 2, 3, 78, 40);
  MapGrid_CopyCellAttributes(20, 44, 2, 1, 14, 42);
}

void Func_02000154(void) {
  struct JupiterLighthouseRoom6Object *object =
      ((struct JupiterLighthouseRoom6Object *)Actor_Get(12));

  if (!Event_GetFlag(0xa19) && object->x >> 20 == 4 && object->z >> 20 == 37) {
    Event_SetFlag(0xa19);
    Func_020000b4(1);
  }
}
