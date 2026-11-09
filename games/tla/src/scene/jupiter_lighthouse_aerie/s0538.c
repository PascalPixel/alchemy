#include "jupiter_lighthouse_aerie.h"
#include "event_inline.h"

s32 Func_02000538(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 513;
  if ((u16)(gMainRoutineWork.entrance - 3) <= 1)
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_02000858(0);
  Func_02002b28(15, 1);
  Func_02002b28(16, 1);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  Actor_Get(17)->priorityFlags |= 128;
  Actor_Get(18)->priorityFlags |= 128;
  Actor_Get(19)->priorityFlags |= 128;
  Actor_Get(20)->priorityFlags |= 128;
  Actor_Get(21)->priorityFlags |= 128;
  Actor_Get(22)->priorityFlags |= 128;
  Func_02000e80();
  return 0;
}

void Scene_SetArrivalFlags(s32 flags);
void Func_02001cb4(void);
void Func_02001dbc(s32 actor, s32 mode);
void Func_02002a78(s32 x, s32 z, s32 a, s32 b);

/// The room's setup: actors 13 and 14 by flags 0xa0a and 0xa0b (with the map
/// cells copied when set), then the terrain under each.
s32 Func_02000604(void) {
  struct Actor *object;

  Scene_SetArrivalFlags(0);
  Func_02001cb4();
  if (Event_GetFlag(0xa0a)) {
    Actor_SetPosition(13, 0, 0);
    MapGrid_CopyCellsLayered(66, 39, 1, 1, 70, 10);
    MapGrid_CopyCellsLayered(5, 75, 1, 1, 6, 75);
    MapGrid_CopyCellAttributes(5, 12, 1, 1, 6, 12);
    MapGrid_CopyCellsLayered(70, 39, 1, 1, 71, 10);
  } else {
    Func_02001dbc(13, 3);
    Actor_Get(13)->priorityFlags |= 32;
  }
  object = Actor_Get(13);
  object->motionFlags = 0;
  {
    s32 last = 6;

    Func_02002a78(object->x, object->z, 0, last);
  }
  if (Event_GetFlag(0xa0b)) {
    Actor_SetPosition(14, 0, 0);
    MapGrid_CopyCellsLayered(68, 39, 1, 1, 121, 10);
    MapGrid_CopyCellsLayered(58, 75, 1, 1, 57, 75);
    MapGrid_CopyCellAttributes(58, 12, 1, 1, 57, 12);
    MapGrid_CopyCellsLayered(72, 39, 1, 1, 120, 10);
  } else {
    Func_02001dbc(14, 5);
    Actor_Get(14)->priorityFlags |= 32;
  }
  object = Actor_Get(14);
  object->motionFlags = 0;
  {
    s32 last = 6;

    Func_02002a78(object->x, object->z, 0, last);
  }
  BYTE_AT(Actor_Get(11), 0x62) = 1;
  BYTE_AT(Actor_Get(12), 0x62) = 1;
  return 0;
}
