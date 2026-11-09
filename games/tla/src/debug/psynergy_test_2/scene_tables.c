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
#include "actor.h"
#include "event_inline.h"

void Func_020001e0(s32 x, s32 y, s32 z, s32 count);
void Func_0200029c(void);
void Func_02000358(void);
void Func_020005d8(s32 a, s32 b);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(void) { Actor_SetHorizontalPositionWithTerrain(8, 0, 0); }

void Func_02000064(s32 mode) {
  if (mode == 1)
    Actor_SetPosition(8, 0, 0);
}

void Func_02000078(void) {}

void Func_0200007c(void) { Actor_SetModeById(9, 3); }

void Func_02000088(s32 mode) {
  if (mode == 1) {
    MapGrid_CopyCellsLayered(49, 1, 1, 2, 34, mode);
    MapGrid_CopyCellAttributes(49, 1, 1, 2, 34, 3);
  }
}

void Func_020000b8(void) {
  if (!Event_GetFlag(0x220)) {
    Event_SetFlag(0x220);
    Func_020005d8(14, 0);
  }
}

void Func_020000d8(s32 id) {
  struct Actor *object = Actor_Get(id);

  Func_0200029c();
  Func_020001e0(object->x, object->y, object->z, 20);
  System_Wait(20);
  Func_02000358();
}
