/// @file
/// The scene code of overlay 0x69f, in ROM order.

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
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_room_6.h"
#include "module/scene/scene.h"
#include "module/tla.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

extern s32 gData_020023c0;
s32 Teleporters_Step(s32 value);
void JupiterLighthouse_CloseDoor(void);
void JupiterLighthouse_LoadPalettes(s32 value);

void Func_02000054(s32 value) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 && Teleporters_Step(value))
    FieldEvent_SetExitRequest(13);
}

void Func_02000084(s32 value) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 && Teleporters_Step(value))
    FieldEvent_SetExitRequest(14);
}

#include "jupiter_lighthouse_room_6.h"

DEFINE_JUPITER_LIGHTHOUSE_OPEN_DOOR(JupiterLighthouse_OpenDoor, 84, 36, 2, 78, 40, 84, 39, 20, 38, 14, 42)

DEFINE_JUPITER_LIGHTHOUSE_CLOSE_DOOR(JupiterLighthouse_CloseDoor, 84, 36, 2, 78, 40, 84, 20, 44, 14, 42)

void Func_02000154(void) {
  struct JupiterLighthouseRoom6Object *object =
      ((struct JupiterLighthouseRoom6Object *)Actor_Get(12));

  if (!Event_GetFlag(0xa19) && object->x >> 20 == 4 && object->z >> 20 == 37) {
    Event_SetFlag(0xa19);
    JupiterLighthouse_OpenDoor(1);
  }
}

DEFINE_SCENE_RUN_ONCE(Scene_RunOnce, 512, JupiterLighthouse_OpenDoor)

DEFINE_SCENE_RUN_IF_FLAG_CLEARED(Scene_RunIfFlagCleared, 512, JupiterLighthouse_CloseDoor, Func_02000154)

void Func_020001d0(void) {
  System_Wait(10);
  Func_02000154();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_020023bc;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gData_020023bc = value; }

s32 __modsi3();
void Teleporters_Find(s32 a, s32 b, s32 c, s32 d, s32 e);

s32 Scene_Initialize(void) {
  struct JupiterLighthouseRoom6EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x204;
  if (Event_GetFlag(0xa19)) {
    Actor_SetPosition(12, PIXELS(72), PIXELS(600));
    JupiterLighthouse_OpenDoor(0);
  }
  if (Event_GetFlag(0x200))
    JupiterLighthouse_OpenDoor(0);
  if ((u16)(gMainRoutineWork.entrance - 5) <= 1) {
    Teleporters_Find(33, 0x280, 0x380, 20, 31);
  } else {
    Teleporters_Find(43, 0x280, 0x380, 21, 31);
    Teleporter_PlaceArrival(21, -3, 0x204);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  if (!Event_GetFlag(265))
    Event_ClearFlag(0xa19);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "shared/jupiter_lighthouse_aerie.h"
#include "ram_buffer.h"
#include "field_event.h"

void Func_020002dc(s32 value) { gData_020023c0 = value; }

DEFINE_PALETTE_CYCLE_STEP(PaletteCycle_Step, gData_020023bc, gData_020023c0,
    gData_020023e8)

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes, gData_020023c0, gData_020023e8,
    gData_02002408, PaletteCycle_Step, gData_0200240c)

void Func_02000428(s32 id) {
  struct JupiterLighthouseRoom6Object *object;
  s32 height;

  object = ((struct JupiterLighthouseRoom6Object *)Actor_Get(id));
  object->motionFlags = 4;
  height = object->height;
  WORD_AT(object, 68) = 0;
  object->y = height + 0x200000;
  object->priorityFlags |= 128;
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_02001b34(object->x, object->z, 0, (height >> 19) + 6);
  Func_02001be4(object->x, object->z, 0, 128);
}

#include "frame.h"
#include "field_sprite.h"
#include "event_work.h"

DEFINE_WHIRLWIND_CARRY_LEADER(Whirlwind_CarryLeader)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern const s32 gData_0200a3c4[];

void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE(Whirlwind_Update, Func_02001bdc, Position_FlatDistance,
    gData_0200a3c4, Data_020023e4)
