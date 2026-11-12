/// @file
/// The scene code of overlay 0x6a2 that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_9/.

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
#include "main_routine_work.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_work.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

extern s32 gData_02002618;

s32 Func_02001978();
struct Actor *Func_02000910();

void Func_02000054(void) {
  if (gMainRoutineWork.renderMode == 10)
    MapGrid_SetCellBehavior(0x2200000, 0x1100000, 0, 255);
}

void Func_0200007c(void) { Func_080dbdd4(0x2200000, 0x1100000, 0, 6); }

void Func_02000090(s32 id) {
  struct Actor *object;

  if (gData_02002618 == 0) {
    object = Actor_Get(8);
    if (id == 30) {
      Actor_SetPosition(8, PIXELS(296), PIXELS(600));
      object->y = 0;
      object->angle = FACING_WEST;
    } else if (id == 31) {
      Actor_SetPosition(8, PIXELS(136), PIXELS(680));
      object->y = 0;
      object->angle = 0;
    }
    Func_02000910(object);
    gData_02002618 = 1;
  }
}

void Func_020000e8(s32 id) {
  struct Actor *object;
  struct Actor *motion;

  if (gData_02002618 == 0 &&
      Actor_Get(gMainRoutineWork.currentOwner)->y > 0) {
    object = Actor_Get(8);
    if (id == 50) {
      Actor_SetPosition(8, PIXELS(488), PIXELS(280));
      object->y = PIXELS(32);
      object->angle = FACING_WEST;
    } else if (id == 51) {
      Actor_SetPosition(8, PIXELS(472), PIXELS(184));
      object->y = PIXELS(32);
      object->angle = FACING_WEST;
    } else if (id == 52) {
      Actor_SetPosition(8, PIXELS(616), PIXELS(280));
      object->y = PIXELS(32);
      object->angle = 0;
    } else if (id == 53) {
      Actor_SetPosition(8, PIXELS(632), PIXELS(184));
      object->y = PIXELS(32);
      object->angle = 0;
    }
    motion = Func_02000910(object);
    if (motion != NULL)
      motion->speedLimit = 0x10000;
    gData_02002618 = 1;
  }
}

void Func_0200018c(s32 arg) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 &&
      Func_02001978(arg) != 0)
    FieldEvent_SetExitRequest(13);
}

void Func_020001bc(s32 arg) {
  struct Actor *leader;
  s32 x;
  s32 z;

  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0) {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    x = leader->x >> 20;
    z = leader->z >> 20;
    if (x == 18 && z == 34)
      FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
    if (x == 8 && z == 39)
      FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
    if (Func_02001978(arg) != 0)
      FieldEvent_SetExitRequest(14);
  }
}

void Func_02000228(s32 arg) {
  struct Actor *leader;
  s32 x;
  s32 z;

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader->y <= 0) {
    Func_0200018c(arg);
  } else {
    x = leader->x >> 20;
    z = leader->z >> 20;
    if (x == 25) {
      if (z == 17)
        Func_020000e8(50);
      if (z == 11)
        Func_020000e8(51);
    }
    if (x == 43) {
      if (z == 17)
        Func_020000e8(52);
      if (z == 11)
        Func_020000e8(53);
    }
  }
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

extern s32 gData_02002618;

void Func_0200130c(s32 kind, s32 x, s32 z, s32 a, s32 b);
void Func_02001a28(s32 dx, s32 dz, s32 effect);
void Func_02000460(s32 mode);

/// The scene's set-up: screen effect 0x204, actor 64's action variant 3 and
/// actor 8 held still, then Func_0200130c and Func_02001a28 by the entrance.
s32 Func_02000290(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x204;
  FieldActor_SetSpritePriority(64, 3);
  gData_02002618 = 0;
  Actor_Get(8)->motionFlags = 0;
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    Func_0200130c(33, 0x280, 0x380, 48, 1);
    Func_02001a28(0, 0, 0x204);
  } else if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5 ||
             gMainRoutineWork.entrance == 13) {
    Func_0200130c(43, 0x280, 0x380, 49, 1);
    Func_02001a28(-21, 32, 0x204);
  } else {
    Func_0200130c(43, 0x280, 0x380, 0, 0);
    Func_02001a28(-2, -27, 0x204);
  }
  return 0;
}

s32 Func_02000358(void) {
  Scene_SetArrivalFlags(0);
  Func_02000460(0);
  return 0;
}
