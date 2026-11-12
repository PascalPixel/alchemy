/// @file
/// Scene code of overlay 0x69f, in ROM order, from s00b4's offset up to the next object the overlay links.

#include "jupiter_lighthouse_room_6.h"
#include "event_inline.h"
#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"

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

extern s32 gData_020023c0;
extern s16 gData_0200242a;
extern struct Actor *gData_0200242c[];
extern u16 gData_0200252e;
s32 Func_020018bc(s32 value);
void Scene_SetArrivalFlags(s32 value);
void Func_020003a4(s32 value);

void Func_02000190(void) {
  if (!Event_GetFlag(512)) {
    Event_SetFlag(512);
    Func_020000b4(1);
  }
}

void Func_020001b0(void) {
  if (Event_GetFlag(512) == 1) {
    Event_ClearFlag(512);
    Func_02000108();
  }
  Func_02000154();
}

void Func_020001d0(void) {
  System_Wait(10);
  Func_02000154();
}
