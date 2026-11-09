#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_02000190(s32, s32);
void Func_02000200(s32, s32);

void Func_02000394(s32 unused, s32 id) {
  Func_02000190(unused, id);
  MapGrid_SetCellBehavior(0x1400000, 0x1d00000, 2, 255);
  MapGrid_SetCellBehavior(0x1400000, 0x1e00000, 2, 255);
  MapGrid_SetCellBehavior(0x1400000, 0x1f00000, 2, 255);
  MapGrid_SetCellBehavior(0x1400000, 0x2000000, 2, 255);
  MapGrid_SetCellBehavior(0x1600000, 0x1d00000, 2, 255);
  MapGrid_SetCellBehavior(0x1600000, 0x1e00000, 2, 255);
  MapGrid_SetCellBehavior(0x1600000, 0x1f00000, 2, 255);
  MapGrid_SetCellBehavior(0x1600000, 0x2000000, 2, 255);
  MapGrid_SetCellBehavior(0x1500000, 0x2100000, 2, 0);
}

void Func_0200042c(s32 mode, s32 id) {
  struct Actor *object;
  s32 x;
  s32 z;
  s32 open;

  object = Actor_Get(id);
  x = object->x >> 20;
  z = object->z >> 20;
  open = 0;
  if (x == 21 && z == 33)
    open = 1;
  MapGrid_SetCellBehavior(0x1400000, 0x1d00000, 2, 0);
  MapGrid_SetCellBehavior(0x1400000, 0x1e00000, 2, 0);
  MapGrid_SetCellBehavior(0x1400000, 0x1f00000, 2, 0);
  MapGrid_SetCellBehavior(0x1400000, 0x2000000, 2, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x1d00000, 2, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x1e00000, 2, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x1f00000, 2, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x2000000, 2, 0);
  MapGrid_SetCellBehavior(0x1500000, 0x2100000, 2, 255);
  if (open)
    FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(560), 1);
  Func_02000200(mode, id);
  if (open)
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
}
