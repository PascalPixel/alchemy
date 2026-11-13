/// @file
/// The scene code of overlay 0x6a2, in ROM order.

#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
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
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_room_9.h"
#include "module/scene/scene.h"
#include "module/tla.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

s32 Teleporters_Step();
struct Actor *Whirlwind_SpawnUpper();

void Func_02000054(void) {
  if (gMainRoutineWork.renderMode == 10)
    FieldMapGrid_SetCellBehavior(0x2200000, 0x1100000, 0, 255);
}

void Func_0200007c(void) {
  MapGrid_SetCellBehavior(0x2200000, 0x1100000, 0, 6);
}

void Func_02000090(s32 id) {
  struct Actor *object;

  if (gData_02002618 == 0) {
    object = Actor_Get(8);
    if (id == 30) {
      Actor_SetPosition(8, PIXELS(296), PIXELS(600));
      object->y = 0;
      object->facing = FACING_WEST;
    } else if (id == 31) {
      Actor_SetPosition(8, PIXELS(136), PIXELS(680));
      object->y = 0;
      object->facing = 0;
    }
    Whirlwind_SpawnUpper(object);
    gData_02002618 = 1;
  }
}

void Func_020000e8(s32 id) {
  struct Actor *object;
  struct Actor *motion;

  if (gData_02002618 == 0 && Actor_Get(gMainRoutineWork.currentOwner)->y > 0) {
    object = Actor_Get(8);
    if (id == 50) {
      Actor_SetPosition(8, PIXELS(488), PIXELS(280));
      object->y = PIXELS(32);
      object->facing = FACING_WEST;
    } else if (id == 51) {
      Actor_SetPosition(8, PIXELS(472), PIXELS(184));
      object->y = PIXELS(32);
      object->facing = FACING_WEST;
    } else if (id == 52) {
      Actor_SetPosition(8, PIXELS(616), PIXELS(280));
      object->y = PIXELS(32);
      object->facing = 0;
    } else if (id == 53) {
      Actor_SetPosition(8, PIXELS(632), PIXELS(184));
      object->y = PIXELS(32);
      object->facing = 0;
    }
    motion = Whirlwind_SpawnUpper(object);
    if (motion != NULL)
      motion->speedLimit = 0x10000;
    gData_02002618 = 1;
  }
}

void Func_0200018c(s32 arg) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 &&
      Teleporters_Step(arg) != 0)
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
    if (Teleporters_Step(arg) != 0)
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

void Teleporters_Find(s32 kind, s32 x, s32 z, s32 a, s32 b);

/// The scene's set-up: screen effect 0x204, actor 64's action variant 3 and
/// actor 8 held still, then Teleporters_Find and Teleporter_PlaceArrival by the
/// entrance.
s32 Scene_Initialize(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x204;
  FieldActor_SetSpritePriority(64, 3);
  gData_02002618 = 0;
  Actor_Get(8)->motionFlags = 0;
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    Teleporters_Find(33, 0x280, 0x380, 48, 1);
    Teleporter_PlaceArrival(0, 0, 0x204);
  } else if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5 ||
             gMainRoutineWork.entrance == 13) {
    Teleporters_Find(43, 0x280, 0x380, 49, 1);
    Teleporter_PlaceArrival(-21, 32, 0x204);
  } else {
    Teleporters_Find(43, 0x280, 0x380, 0, 0);
    Teleporter_PlaceArrival(-2, -27, 0x204);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "shared/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "field_event.h"

extern s32 gData_020025f4;

void Func_02000398(s32 value) { gData_020025f4 = value; }

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

DEFINE_PALETTE_CYCLE_STEP(PaletteCycle_Step, gPaletteCycleFrame, gData_020025f4,
                          gData_0200261c)

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gData_020025f4, gData_0200261c,
                                        gData_0200263c, PaletteCycle_Step,
                                        gData_02002640)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, Func_02001bd8, Func_02001c98)

#include "frame.h"
#include "field_sprite.h"

DEFINE_WHIRLWIND_CARRY_LEADER(Whirlwind_CarryLeader)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern const s32 gWhirlwindEndScript[];

s32 Func_02001c88(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE(Whirlwind_Update, Func_02001c88, Position_FlatDistance,
                        gWhirlwindEndScript, gData_02002618)
