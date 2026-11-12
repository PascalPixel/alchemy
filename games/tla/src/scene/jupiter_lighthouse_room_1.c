/// @file
/// The scene code of overlay 0x69a that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_1/.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "field_event.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "debug.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(void) { Import_EventSetExitRequest(6); }

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

void Func_02000178(void);

void Func_020001f4(void) {
  System_Wait(10);
  Func_02000178();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_02001bcc(s32 owner);
void Func_02000060(s32 animate);

s32 Func_0200020c(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  if (!Event_GetFlag(0x109) && gMainRoutineWork.entrance == 6)
    Func_02001bcc(gMainRoutineWork.currentOwner);
  if (Event_GetFlag(0xa01)) {
    Actor_SetPosition(10, PIXELS(280), PIXELS(536));
    Func_02000060(0);
  }
  if (Event_GetFlag(0x200))
    Func_02000060(0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}
