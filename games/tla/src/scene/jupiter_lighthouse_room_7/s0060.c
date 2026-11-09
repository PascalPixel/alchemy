#include "jupiter_lighthouse_room_7.h"
#include "event_inline.h"

void Func_02000060(s32 mode) {
  if (mode == 1) {
    Event_PlaySound(133);
    MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(81, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 41, 4, 1, 12, 35);
}

void Func_020000b4(void) {
  Event_PlaySound(134);
  MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
  System_Wait(6);
  MapGrid_CopyCellsLayered(71, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 36, 4, 1, 12, 35);
}

void Func_02000100(void) {
  struct Actor *object = Actor_Get(16);

  if (!Event_GetFlag(0xa1b) && object->x >> 20 == 8 && object->z >> 20 == 37) {
    Event_SetFlag(0xa1b);
    Func_02000060(1);
  }
}
