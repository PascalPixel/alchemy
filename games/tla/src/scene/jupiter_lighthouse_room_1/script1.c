#include "types.h"
#include "field_event.h"
#include "event_inline.h"

void Func_02000060(s32 animate) {
  if (animate == 1) {
    Event_PlaySound(133);
    MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 29);
    MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 37);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(84, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(84, 20, 2, 3, 79, 37);
  MapGrid_CopyCellAttributes(20, 22, 2, 1, 15, 31);
  MapGrid_CopyCellAttributes(20, 22, 2, 1, 15, 39);
}

void Func_020000ec(void) {
  Event_PlaySound(134);
  MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 37);
  System_Wait(6);
  MapGrid_CopyCellsLayered(87, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(87, 20, 2, 3, 79, 37);
  MapGrid_CopyCellAttributes(23, 22, 2, 1, 15, 31);
  MapGrid_CopyCellAttributes(23, 22, 2, 1, 15, 39);
}

void Func_02000178(void) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(10));

  if (!Event_GetFlag(0xa01) && object->x >> 20 == 17 && object->z >> 20 == 33) {
    Event_SetFlag(0xa01);
    Func_02000060(1);
  }
}

void Func_020001b4(void) {
  if (!Event_GetFlag(0x200)) {
    Event_SetFlag(0x200);
    Func_02000060(1);
  }
}

void Func_020001d4(void) {
  if (Event_GetFlag(0x200) == 1) {
    Event_ClearFlag(0x200);
    Func_020000ec();
  }
  Func_02000178();
}
