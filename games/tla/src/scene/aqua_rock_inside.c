/// @file
/// Aqua Rock's inside, the island south of the Apojii Islands, in the rooms of
/// this overlay: rapids and falls the party runs against or rides, vents that
/// blow until a pillar is Moved over them (Vent_Puff), Moai that lower
/// the water, puddles to Frost, Water Stones to Douse and a tightrope. At its
/// heart Piers reads the tablet and learns Parch
/// (AquaRockInside_ReadParchTablet); the Mercury Djinni Steam waits in one of
/// its rooms (AquaRockInside_FightSteam).

#include "shared/actors.h"
#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "system/iwram_call.h"
#include "system/io_reg.h"
#include "field_sprite.h"
#include "field/event_inline.h"
#include "system/frame.h"

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Animation_ApplyChildArgument(void *animation, s32 value);
u8 *ResourceMetadata_Register(void *animation, s32 id);
void Event_FinishScreenTransition(void);
s32 VramCache_FindFreeEntry(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

#include "map.h"

#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

struct PillarEntry {
  s32 x;
  s32 height;
  s32 z;
  s32 base;
  u16 kind;
  u16 state;
  s32 *position;
  u32 top[3];
  u32 column[3];
};

struct PillarWork {
  u16 vramEntry;
  u16 count;
  struct PillarEntry entries[32];
};

struct BeamEntry {
  s32 x;
  s32 y;
  s32 z;
  u16 unk0c;
  u16 direction;
};

struct Splash {
  s32 x;
  s32 y;
  s32 z;
  u16 active;
  u16 direction;
  u32 sprite[3];
};

struct BeamWork {
  u16 vramEntry;
  u16 count;
  struct BeamEntry entries[2];
  struct Splash splashes[32];
};

struct VentEntry {
  s16 active;
  s16 timer;
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

struct VentWork {
  s32 counter;
  s32 (*hook)(void);
  struct VentEntry entries[16];
  u16 ids[4];
};

extern u8 gInventorySnapshot[];
#define VENT_WORK ((struct VentWork *)(gInventorySnapshot + 0x188))

struct VentRow {
  u8 unk00[8];
  s16 active;
  s16 timer;
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

#define VENT_ROW(index) ((struct VentRow *)((u8 *)VENT_WORK + (index) * 20))

struct SceneEventWork {
  u8 unk000[0x15a];
  s16 value15a;
  u8 unk15c[0xe];
  s16 value16a;
};

struct FieldSpriteFlags {
  u8 unk00[0x11];
  u8 flag0 : 1;
};

s32 Trig_Cos(s32 angle);
void Decompress_TaggedLz(const void *source, void *destination);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_StartScript(struct Actor *object, const void *data);
void Map_LoadCellsAndShapes(void);
void MessageArgument_Push(s32 item, s32 mode);
void Event_End(void);
void Actor_JumpDownLedge(s32 id, s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Field_TryJumpForward(void);
void Party_AddPp(s32 a, s32 b);
void Scene_QueueMusic(s32 a);
void Func_02002bd4(struct Actor *object);
s32 Actor_EmitMist(struct Actor *object);
void Event_NoOp(void);

void Func_020006d0(void);
s32 Func_02000b24(s32 id, s32 value);
void Func_02000b5c(void);
void AquaRockInside_ReadParchTablet(void);
void Func_02001e78(s32 id);
void Func_020030f4(void);
void Func_02003460(s32 value);
void Func_020034fc(void);
void Func_02003628(void);
s32 Func_02003688(s32 x, s32 y, s32 z, s32 kind);
s32 Func_020036b8(s32 a, s32 b);
void Func_0200389c(struct Actor *object);
void Actor_StartScripted(struct Actor *object, s32 priority);
void Vent_Puff(s32 *position, s32 base);
s32 Func_02003bb8(s32 index, s32 value);
s32 Func_02003bcc(void);
s32 Vents_AdvanceCounter(s32 limit);

extern s32 gData_02004d84;
extern struct PillarWork gData_0200536c;
extern struct BeamWork gData_02005970;
extern u32 gData_02003fd8[];
extern u32 gData_020040dc[];
extern u32 gData_0200411c[];
extern u32 gWaterDropScript[];
extern const u8 gData_0200414c[];
extern const u8 gData_02004351[];
extern u32 gData_020044c8[];

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 a, s32 z, s32 b) {
  FieldCamera_MoveWithinMapBounds(x, a, z, b);
}

static __inline__ void Func_02003d14_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellsRect(a, b, c, d, e, f);
}

static __inline__ void Func_02003d1c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static __inline__ void Func_02003d24_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  MapGrid_CopyCellPictureCellsRect(a, b, c, d, e, f);
}

static __inline__ void *MemAlloc_(s32 size) { return Heap_Alloc(size); }
#include "flags.h"
#include "shared/effects.h"
#include "event_work.h"
#include "field/field.h"
#include "module/field/map.h"
#include "types.h"
#include "module/scene/scene.h"
#include "module/sound/sound.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "version.h"
#include "actor.h"
#include "system/vector3.h"
#include "field/event_inline.h"
#include "system/io_write_queue.h"
#include "system/frame.h"
#include "system/field_effect.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "call.h"
#include "system/iwram_call.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "macros.h"
#include "shared/vents.h"
#include "shared/tables.h"
#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

DEFINE_SCENE_CREATE_ACTOR(Scene_CreateActor)

#include "../field/field_effect/move.c"

// @file
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

struct Actor;

struct WaterOutlet {
  u16 state;
  u16 index;
  s32 x;
  s32 y;
  s32 z;
  s32 level;
};

struct WaterWork {
  u32 unk00;
  void (*handler)(void);
  struct WaterOutlet outlets[16];
  u16 levels[4];
};

void Func_0200349c(u32 index, s32 height);

void Func_0200061c(struct Actor *object);

void Leader_RiseFromWater(void);
void Func_0200107c(s32 a, s32 b);
void Func_02003278(void);
void Func_020032d4(u32 first, s32 level, s32 baseLevel);
void Func_020007a8(void);
void Func_02003420(s32 id, s32 kind, s32 height);
void Func_02000ad8(void);

void Func_020004b4(void);

void Func_02000690(void);

void Func_0200071c(void);
void Func_020007d4(void);
void AquaRockInside_CheckColumn(void);

void Func_02000a98(void);

void Func_02000bac(void);

void Func_02000bd4(void);

void Func_02000de4(void);

void Func_02001060(void);
void AquaRockInside_CheckPsynergyStone(void);
void Func_02001170(void);
void Func_02001328(void);
void AquaRockInside_WalkToParchTablet(void);
void Func_02001cd8(void);
void Func_02001d40(void);
void Func_02002ca4(void);

void Actor_SelectAnimation(struct Actor *object, s32 mode);
void FieldEffect_StartBlankOverlay();
void Tightrope_Place();

#include "text/msg_ids.h"

extern u8 Data_0202c001[];

void AquaRockInside_FightSteam(void) { Field_RequestDjinnBattle(10, 32); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gAquaRockInside_Placements[];
extern struct ActorPlacement gAquaRockInside_Room1Placements[];
extern struct ActorPlacement gAquaRockInside_Room2Placements[];
extern struct ActorPlacement gAquaRockInside_Room3Placements[];
extern struct ActorPlacement gAquaRockInside_Room4Placements[];
extern struct ActorPlacement gAquaRockInside_Room5Placements[];
extern struct ActorPlacement gAquaRockInside_Room6Placements[];
extern struct ActorPlacement gAquaRockInside_Room7Placements[];
extern struct ActorPlacement gAquaRockInside_Room8Placements[];
extern struct ActorPlacement gAquaRockInside_Room9Placements[];
extern struct ActorPlacement gAquaRockInside_Room10Placements[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67cRoom1)
    return gAquaRockInside_Room1Placements;
  if (scene == (s32)&SceneId_Ov67cRoom2)
    return gAquaRockInside_Room2Placements;
  if (scene == (s32)&SceneId_Ov67cRoom3)
    return gAquaRockInside_Room3Placements;
  if (scene == (s32)&SceneId_Ov67cRoom4)
    return gAquaRockInside_Room4Placements;
  if (scene == (s32)&SceneId_Ov67cRoom5)
    return gAquaRockInside_Room5Placements;
  if (scene == (s32)&SceneId_Ov67cRoom6)
    return gAquaRockInside_Room6Placements;
  if (scene == (s32)&SceneId_Ov67cRoom7)
    return gAquaRockInside_Room7Placements;
  if (scene == (s32)&SceneId_Ov67cRoom8)
    return gAquaRockInside_Room8Placements;
  if (scene == (s32)&SceneId_Ov67cRoom9)
    return gAquaRockInside_Room9Placements;
  if (scene == (s32)&SceneId_Ov67cRoom10)
    return gAquaRockInside_Room10Placements;
  return gAquaRockInside_Placements;
}

// When object 10 reaches row 12, or object 11 column 51, stops its motion,
// lifts it one pixel and sets its flag; object 10 also redraws two blocks of
// row 12.
void Func_0200042c(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 10) {
    s32 row = object->z >> 20;

    if (row == 12) {
      object->motionFlags = 0;
      {
        s32 step = -0x10000;

        object->y += step;
      }
      Event_SetFlag(0x95a);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 9, row);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 13, row);
    }
  } else if (id == 11) {
    if (object->x >> 20 == 51) {
      object->motionFlags = 0;
      {
        s32 step = -0x10000;

        object->y += step;
      }
      Event_SetFlag(0x95b);
    }
  }
}

void Func_020004b4(void) { Field_TryJumpForward(); }

void Func_020004bc(s32 unused, s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;
  s32 column;
  s32 objectId = id;

  layer = (s32 *)(map + 0x140);
  FieldActor_SetSpriteColor(Actor_Get(objectId), 0);
  Func_02001e78(objectId);
  Actor_SetAnimation(objectId, 3);
  layer[3] -= 0x200000;
  Map_LoadCellsAndShapes();
  Map_CenterOnFollowedPoint();
  column = (Actor_Get(12)->x >> 20) - 42;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_556);
  for (i = 0; i <= 31; i++) {
    Func_02003460(i << 16);
    {
      /* FAKEMATCH(pin): the zero pinned to r1, set before the column is copied
       * to r0. */
      register s32 height asm("r1") = 0;

      Func_0200349c(column, height);
    }
    layer[3] += 0x10000;
    System_Wait(4);
  }
  if (Event_GetFlag(FLAG_SCENE_TEMP_50))
    Func_02003d1c_(41, 42, 1, 1, 43, 44);
  Event_End();
  Event_SetFlag(FLAG_SCENE_TEMP_30);
}

void Func_02000574(s32 unused, s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;

  layer = (s32 *)(map + 0x140);
  FieldActor_SetSpriteColor(Actor_Get(id), 0);
  Func_02001e78(id);
  Actor_SetAnimation(id, 3);
  layer[3] -= 0x200000;
  Map_LoadCellsAndShapes();
  Map_CenterOnFollowedPoint();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_556);
  for (i = 0; i <= 31; i++) {
    Func_02003460(i << 16);
    layer[3] += 0x10000;
    System_Wait(4);
  }
  Event_End();
  Event_SetFlag(FLAG_SCENE_TEMP_31);
}

void Data_020005f4(void) {}

void Func_020005f8(void) {
  Func_02003d1c_(41, 42, 1, 1, 43, 44);
  Event_SetFlag(FLAG_SCENE_TEMP_50);
}

#if TLA_PATCH_1
void Func_0200061c(struct Actor *object) {
  object->motionFlags = 3;
  do {
    System_Wait(1);
  } while (object->velocityY != 0);
  Event_PlaySound(SE_THUMP);
  System_Wait(10);
  object->motionFlags = 0;
}
#endif

void Func_02000644(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 x;

  if (id == 9) {
    x = object->x >> 20;
    if (x == 39) {
#if !TLA_PATCH_1
      object->motionFlags = 3;
      System_Wait(15);
      object->motionFlags = 0;
      Event_SetFlag(0x95c);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, x, id);
#else
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Func_0200061c(object);
      Event_SetFlag(0x95c);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, x, id);
      Event_End();
#endif
    }
  }
}

void Func_02000690(void) {
  Actor_SetAnimation(8, 4);
  FieldActor_SetSpriteColor(Actor_Get(8), 6);
  Func_02003d14_(123, 15, 5, 3, 110, 15);
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->value16a = 88;
}

void Func_020006d0(void) {
  s32 count = gData_02004d84 - 1;
  s32 row;

  gData_02004d84 = count;
  row = count / 8;
  MapGrid_CopyCells((count & 1) * 5 + 118, 47 - row, 5, 2, 110, 47 - row);
  if (gData_02004d84 == 0)
    FieldTask_Remove(Func_020006d0);
}

void Func_0200071c(void) {
  s32 priority;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_ArmCallback_(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Event_PlaySound(SE_557);
  priority = 0x480;
  FieldTask_AddFrame(Func_020006d0, priority);
  FieldCamera_SetSpeedAndAcceleration(2.5, 0.25);
  FieldCamera_MoveTo(776, CAMERA_KEEP, 648, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  Event_End();
  Event_SetFlag(FLAG_SCENE_TEMP_40);
  Func_02003d1c_(59, 16, 5, 32, 46, 16);
}

void Func_020007a8(void) {
  struct Actor *object;
  struct FieldSpriteFlags *sprite;

  FieldActor_ShowItemIconById(10, 456);
  object = Actor_Get(20);
  sprite = object->sprite;
  sprite->flag0 = 1;
  object->motionFlags = 0;
  object->y = 0x180000;
}

extern u8 gData_02003fb8[];

void Func_020007d4(void) {
  s32 priority;
  s32 i;
  s32 j;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Func_020007a8();
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(60);
  Event_PlaySound(SE_162);
  MapGrid_SetShake(1, 1, 1);
  FieldEvent_Wait(20);
  MapGrid_SetShake(1, 2, 1);
  FieldEvent_Wait(20);
  MapGrid_SetShake(1, 3, 1);
  FieldEvent_Wait(40);
  priority = 0x480;
  FieldTask_AddFrame(Func_020006d0, priority);
  FieldCamera_SetSpeedAndAcceleration(2.5, 0.25);
  FieldCamera_MoveTo(312, CAMERA_KEEP, 504, 1);
  Event_PlaySound(SE_SCATTER);
  Func_02003d24_(75, 96, 7, 13, 80, 27);
  MapGrid_CopyCellsLayered(91, 96, 7, 10, 16, 31);
  MapGrid_CopyCellsLayered(99, 96, 7, 14, 16, 91);
  MapGrid_CopyCellAttributes(43, 0, 5, 9, 17, 31);
  i = 0;
row:
  j = 0;
cell:
  {
    s32 index = (i + (1 & ~j)) * 2;

    Func_02003d24_(gData_02003fb8[index], gData_02003fb8[index + 1], 7, 13, 80,
                   27);
    System_Wait(1);
  }
  j++;
  if (j <= 6)
    goto cell;
  i++;
  if (i <= 14)
    goto row;
  FieldCamera_WaitForArrival();
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  FieldEvent_Wait(30);
  Event_PlaySound(MUS_080);
  Sound_WaitForBgmRestore();
  Event_End();
  Scene_PlayMusic();
  Event_SetFlag(0x95d);
  Event_SetFlag(0x143);
}

void Func_02000958(s32 value) {
  s32 width = 1;
  s32 height = 2;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_THUMP);
  MapGrid_CopyCellsTo(107, 96, 19, 29, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(108, 96, 19, 29, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(107, 98, 19, 29, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(108, 98, 19, 29, width, height);
  FieldEvent_Wait(15);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_Get(gMainRoutineWork.currentOwner)->speedLimit =
      Actor_Get(gMainRoutineWork.currentOwner)->acceleration = 0x8000;
  Event_PlaySound(SE_MAP_EXIT);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 312, 496);
  FieldEvent_Wait(5);
  FieldEvent_SetExitRequest(value);
}

void AquaRockInside_CheckColumn(void) {
  s32 seen;
  const u8 *message;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  seen = Event_GetFlag(0x95d);
  message = MsgCheckedTheColumn;
  if (!seen) {
    Message_ShowCentered(message, 1);
    Message_ShowCentered(message + 1, 1);
    if (FieldPartyInventory_FindMember(456) != -1)
      ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->value15a = 1;
  } else {
    Message_ShowCentered(message, 1);
    MessageArgument_Push(0x1c8, 2);
    Message_ShowCentered(message + 2, 1);
  }
  Event_End();
}

void Func_02000a98(void) {
  s32 i;

  Func_02003628();
  Func_02003688(16, 4, 16, 2);
  Func_02003688(22, 4, 16, 0);
  for (i = 5; i >= 0; i--) {
    Func_020036b8(33, 55);
    System_Wait(15);
  }
  Event_SetFlag(0xa66);
}

void Func_02000ad8(void) {
  MapGrid_CopyCellsLayered(106, 14, 14, 13, 81, 14);
  MapGrid_CopyCellsLayered(44, 78, 14, 12, 17, 78);
  MapGrid_CopyCellAttributes(81, 14, 14, 13, 17, 14);
  Event_SetFlag(0xa6a);
}

s32 Func_02000b24(s32 id, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_Get(id);

  if (object != 0) {
    struct MapCell *cells = map->layers[0].cells;
    struct MapCell *cell = &cells[(object->x >> 20) + (object->z >> 20) * 128];

    cell->collisionCode = value;
    cell->attributeB |= 0x80;
  }
}

void Func_02000b5c(void) {
  struct Actor *object = Actor_Get(12);

  Func_02003d1c_(64, 43, 1, 1, object->x >> 20, object->z >> 20);
}

void Func_02000b84(void) {
  struct Actor *object = Actor_Get(12);

  Func_02003d1c_(64, 42, 1, 1, object->x >> 20, object->z >> 20);
}

void Func_02000bac(void) {
  struct Actor *object = Actor_Get(12);

  Func_02003d1c_(64, 41, 1, 1, object->x >> 20, object->z >> 20);
}

void Func_02000bd4(void) {
  s32 i;
  s32 result;

  if (Event_GetFlag(FLAG_SCENE_TEMP_49)) {
    Func_02000b24(12, 38);
    Func_02003628();
    Func_02003688(12, 2, 45, 1);
    for (i = 3; i >= 0; i--)
      result = Func_020036b8(33, 55);
    Event_SetFlag(FLAG_SCENE_TEMP_48);
    if (result == 1)
      Event_SetFlag(0xa64);
  }
}

void Func_02000c28(s32 unused, s32 id) {
  s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x140);
  s32 i;

  FieldActor_SetSpriteColor(Actor_Get(9), 5);
  Func_02001e78(id);
  Actor_SetAnimation(id, 3);
  layer[3] -= 0x200000;
  Map_LoadCellsAndShapes();
  Map_CenterOnFollowedPoint();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_557);
  for (i = 31; i >= 0; i--) {
    layer[3] += 0x10000;
    System_Wait(4);
  }
  Event_End();
  Func_02000b5c();
  Event_SetFlag(FLAG_SCENE_TEMP_49);
}

void Func_02000ca8(void) {
  Actor_JumpDownLedge(gMainRoutineWork.currentOwner, 6, 0);
  Event_End();
}

#undef gData_02003fb8
/* VIEW(s0cc8): routine Func_02000ca8 as void Func_02000ca8(); the parts before
 * it has another declaration; tried as the definition's void
 * Func_02000ca8(void), which the object already declares, it does not compile
 * in tla-ja: games/tla/src/scene/aqua_rock_inside.c:775: too many arguments to
 * function `Func_02000ca8'. */
extern void Func_02000ca8__s0cc8() __asm__("Func_02000ca8");

void Func_02000cc8(s32 trigger) {
  s32 clear = 1;
  struct Actor *object;
  struct Vector3 target;
  s32 result;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67cRoom5) {
    s32 column = Actor_Get(8)->x >> 20;

    if (trigger == 77 && column == 17)
      clear = 0;
    if (trigger == 78 && column == 19)
      clear = 0;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67cRoom6) {
    if (trigger == 77 && Actor_Get(10)->x >> 20 == 9)
      clear = 0;
    if (trigger == 78 && Actor_Get(11)->x >> 20 == 13)
      clear = 0;
  }
  if (clear == 0)
    return;
  object = Actor_Get(gMainRoutineWork.currentOwner);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  goto test;
step:
  if (result < 0) {
    object->facing = 0x4000;
    Func_02000ca8__s0cc8(trigger);
    return;
  }
  {
    s32 offset, x;

    object->z += 0x20000;
    x = object->x;
    offset = (x & 0xfffff) - 0x80000;
    {
      s32 limit = 0x10001;

      if (offset >= limit)
        offset = 0x10000;
      limit = -0x10001;
      if (offset <= limit)
        offset = -0x10000;
    }
    object->x = x - offset;
  }
  object->facing += 0x1000;
  System_Wait(1);
test:
  target.x = object->x;
  target.y = object->y;
  target.z = object->z + 0x80000;
  result = MapGrid_CheckCollision(object, &target);
  if (result <= 0)
    goto step;
}

void Func_02000de4(void) {
  s32 i;

  Func_02003628();
  Func_02003688(44, 0, 34, 2);
  for (i = 4; i >= 0; i--) {
    Func_020036b8(33, 55);
    System_Wait(15);
  }
  Event_SetFlag(0xa67);
}

void Func_02000e18(s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 speed;
  s32 i;
  u8 *flags;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  flags = &object->motionFlags;
  speed = 0;
  *flags = 0;
  object->priorityFlags |= 0x20;
  Actor_SelectAnimation(object, 18);
  Event_PlaySound(SE_SPLASH_SMALL);
  System_Wait(13);
  Actor_SetSpriteColorMode(object, 9);
  Actor_SelectAnimation(object, 28);
  Event_PlaySound(SE_566);
  for (i = 0; i <= 59; i++) {
    if (i == 30)
      Event_FinishScreenTransition();
    speed += 0x4000;
    object->y += speed;
    if ((i & 7) == 0)
      Func_02002bd4(Actor_Get(15));
    System_Wait(1);
  }
  Event_End();
  FieldEvent_SetExitRequest(value);
}

void Func_02000ecc(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object->x >> 20 == 49) {
    s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x140);
    s32 i;

    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Event_PlaySound(SE_557);
    Actor_SetAnimation(9, 2);
    Func_02003d24_(114, 39, 1, 2, 113, 39);
    for (i = 31; i >= 0; i--) {
      layer[3] -= 0x10000;
      System_Wait(4);
    }
    i = 13;
  place:
    object = Actor_GetById(i);
    object->x = object->timer << 16;
    object->z = *(s16 *)((u8 *)object + 102) << 16;
    object->motionFlags = 0;
    object->terrainHeight = -0x200000;
    object->y = -0x200000;
    FieldActor_SetSpritePriority(i, 3);
    i++;
    if (i <= 15)
      goto place;
    BlendScript_Suspend();
    i = 12;
  blend:
    IO_WRITE_QUEUE_PUSH(0x04000052, 0x1000 | i, 0x20000);
    System_Wait(4);
    i--;
    if (i >= 0)
      goto blend;
    MapGrid_CopyCellAttributes(105, 41, 12, 13, 41, 41);
    MapGrid_CopyCellsLayered(105, 105, 12, 13, 105, 41);
    BlendScript_Resume();
    Event_End();
    Event_SetFlag(FLAG_SCENE_TEMP_38);
  }
}

void Func_0200101c(s32 unused, s32 id) {
  if ((u32)(id - 13) <= 2) {
    struct Actor *object = Actor_Get(id);

    Func_02003d1c_(47, 45, 1, 1, object->x >> 20, object->z >> 20);
    Event_SetFlag(id + 0x244);
    object->priorityFlags = 0;
  }
}

void Func_02001060(void) { Func_02003d1c_(8, 63, 7, 1, 8, 34); }

// When object 10 or 11 arrives, stops both and redraws row 33 at each one's
// column.
void Func_0200107c(s32 unused, s32 id) {
  if ((u32)(id - 10) <= 1) {
    struct Actor *object = Actor_Get(10);

    object->motionFlags = 0;
    {
      s32 row = 33;

      Func_02003d1c_(8, row, 1, 1, object->x >> 20, 34);
    }
    object = Actor_Get(11);
    object->motionFlags = 0;
    {
      s32 row = 33;

      MapGrid_CopyCellAttributes(8, row, 1, 1, object->x >> 20, 34);
    }
  }
}

void Func_020010d0(struct Actor *object) {
  s32 frames = 60;

  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
}

void AquaRockInside_CheckPsynergyStone(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgCheckedThePsynergy, 1);
  Event_PlaySound(SE_RECOVERY);
  Party_AddPp(999, 0);
  FieldEvent_Wait(10);
  Event_ClearFlag(FLAG_NOTHING_HAPPENS);
  Message_ShowCentered(MsgCheckedThePsynergy + 1, 1);
  Event_End();
}

void Func_02001138(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  MapGrid_CopyCellAttributes(17, 39, 2, 1, 20, 39);
  MapGrid_CopyCellAttributes(23, 39, 1, 1, object->x >> 20, 39);
}

void Func_02001170(void) {
  s32 i;
  s32 result;

  if (Event_GetFlag(FLAG_SCENE_TEMP_4B)) {
    /* FAKEMATCH(signature): these two calls go through a void function type. */
    ((void (*)(s32, s32))Func_02000b24)(12, 38);
    ((void (*)(s32, s32))Func_02000b24)(13, 39);
    Func_02000b24(15, 37);
    Func_02003628();
    Func_02003688(38, 2, 19, 1);
    for (i = 8; i >= 0; i--)
      result = Func_020036b8(33, 55);
    Event_SetFlag(FLAG_SCENE_TEMP_4A);
    if (result == 1)
      Event_SetFlag(0xa65);
  }
}

void Func_020011d4(s32 unused, s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;

  layer = (s32 *)(map + 0x140);
  FieldActor_SetSpriteColor(Actor_Get(id), 5);
  Func_02001e78(id);
  Actor_SetAnimation(id, 3);
  layer[3] -= 0x200000;
  Map_LoadCellsAndShapes();
  Map_CenterOnFollowedPoint();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_556);
  for (i = 31; i >= 0; i--) {
    layer[3] += 0x10000;
    System_Wait(4);
  }
  Event_End();
  Event_SetFlag(FLAG_SCENE_TEMP_4B);
}

void Func_0200124c(void) {
  s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x140);
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  layer[3] -= 0x200000;
  Map_CenterOnFollowedPoint();
  MapGrid_SetShake(1, 1, 1);
  i = 14;
shake:
  {
    s32 column = 77;
    s32 row = 8;

    MapGrid_CopyCellsLayered(70, 8, 7, 6, column, row);
    System_Wait(1);
    MapGrid_CopyCellsLayered(50, 8, 7, 6, column, row);
    System_Wait(1);
  }
  i--;
  if (i >= 0)
    goto shake;
  System_Wait(10);
  for (i = 127; i >= 0; i--) {
    layer[3] += 0x4000;
    System_Wait(1);
  }
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_End();
  Func_02003d1c_(38, 9, 7, 6, 13, 9);
  Event_SetFlag(FLAG_AREA_TEMP_49);
}

void Func_02001328(void) {
  Func_02003d24_(70, 8, 7, 6, 77, 8);
  Func_02003d1c_(38, 17, 7, 6, 13, 9);
  Event_ClearFlag(FLAG_AREA_TEMP_49);
}

void Func_02001364(struct Actor *object) {
  struct Actor *child =
      Import_ActorCreate_(0x2a0, object->x, object->y, object->z);

  child->scaleY = 0x6666;
  child->scaleX = 0x6666;
  child->motionFlags = 0;
  FieldActor_SetSpriteColor(child, 2);
  Scene_SetActorSpritePriority(child, 1);
  Actor_SelectAnimation(child, 1);
  Actor_SetSpriteFlags(child, 0);
  Actor_SetBlendMode(child, 1);
  Actor_StartScript(child, gData_02003fd8);
}

// Steps an orbiting object around its anchor: turns it by half its speed, puts
// it on the anchor (raised 8 pixels) and pushes it out by its radius; when its
// countdown runs out returns 0, else, unless its radius is down to 2 pixels,
// takes a second, shrinking step. Returns 1 while it runs. The same code as
// overlay 681's Func_0200136c. The first push goes through Call3 (directly, its
// offset pointer is set after the angle's load).

DEFINE_ORBITER_SPIRAL(Orbiter_Spiral, Func_02001364)

// Moves around in a circle.
DEFINE_ORBIT_UPDATE(Orbit_Update)

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

DEFINE_ACTOR_AIM_AT_WATCHED(Actor_AimAtWatched)

DEFINE_ACTOR_STEP_TO_WATCHED(Actor_StepToWatched)

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

DEFINE_ACTOR_EMIT_BUBBLES(Actor_EmitBubbles, FieldEffect_Spawn)
#undef BYTE_AT

union ActorView {
  struct Actor actor;
  struct {
    u8 unk00[0x62];
    u8 unk62;
    u8 unk63;
  } bytes;
};
extern u32 gData_02004034[];
extern u32 gData_02003fe8[];
extern s32 gData_02004004[];
extern u32 gData_02004058[];
extern s32 gData_02004014[];
extern s32 gData_0200407c[];
extern s32 gData_020040ac[];

void Func_02001620(s32 firstId, s32 secondId) {
  struct Actor *first = Actor_GetById(firstId);
  struct Actor *second = Actor_GetById(secondId);
  struct Actor *object;
  struct Actor *spark;
  struct Actor *riser;
  s32 i;
  u16 noMotion;
  s32 probe;
  const u8 *script;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  script = gData_02004058;
  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_Wait(30);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveWithinMapBounds((first->x >> 16) << 16,
                                  (first->y >> 16) << 16,
                                  (first->z >> 16) << 16, 1);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  PaletteFade_StartInterpolation(60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Actor_SetMovementTypeById(firstId, gData_02004034);
  Sound_Play(SE_194);
  Event_Wait(45);
  Actor_ConfigureSpriteColor(firstId, 256);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
burst:
  object = ((void *)Import_ActorCreate(672, first->x, first->y, first->z));
  Sound_Play(SE_GLITTER);
  object->x += ((Random16() & 31) - 16) << 16;
  object->y += ((Random16() & 15) - 8) << 16;
  object->speedLimit = 0x80000;
  object->acceleration = 0x3333;
  object->motionFlags = 0;
  Actor_SetSpriteColor(object, 2);
  Scene_SetActorSpritePriority(object, 1);
  Actor_SelectAnimation(object, 7);
  Actor_SetSpriteFlags(object, 0);
  Actor_SetBlendMode(object, 1);
  Actor_StartScript(object, gData_02003fe8);
  REG_BLDALPHA = 0x1004;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  REG_BLDALPHA = 0x1010;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  i++;
  if (i != 16)
    goto burst;
  REG_BLDALPHA = 0x1008;
  wait(30);
  i = 0;
  /* FAKEMATCH(dead): the never-taken jump into the spiral keeps its refs at
   * weight 1. */
  probe = second->unk57;
  if (probe & 0x100) {
    wait(1);
    goto tail;
  }
spiral:
  spark = FieldActor_Create(672, second->x, second->y, second->z);
  Sound_Play(SE_195);
  spark->timer = (Random16() & 0x7fff) + 0x8000;
  spark->unk66 = (((i & 1) << 1) - 1) << ((Random16() & 3) + 9);
  noMotion = 0;
  spark->motionFlags = noMotion;
  spark->unk4c = 0x7a0000;
  spark->watchedObject = second;
  ((union ActorView *)spark)->bytes.unk62 = ((16 - i) << 3) + 15;
  Actor_SetSpriteColor(spark, 2);
  Scene_SetActorSpritePriority(spark, 1);
  Actor_SelectAnimation(spark, 7);
  Actor_SetSpriteFlags(spark, 0);
  Actor_SetBlendMode(spark, 1);
  FieldVector_AddPolarOffset(spark->unk4c, spark->timer, &spark->x);
  Actor_StartScript(spark, gData_02004004);
  if (i == 3) {
    Actor_SetSpriteColorById(secondId, 256);
    second->motionFlags = noMotion;
    second->acceleration = 0x3333;
    second->speedLimit = 0x3333;
    Actor_SetMovementTypeById(secondId, script);
  }
tail:
  wait(8);
  i++;
  if (i != 16)
    goto spiral;
  Sound_Play(SE_SHIMMER);
  wait(16);
  i = 2;
column:
  riser = FieldActor_Create(672, second->x, second->y + i * 0x10000 - 0x80000,
                            second->z);
  riser->timer = Random16();
  riser->unk66 = 0x800;
  ((union ActorView *)riser)->bytes.unk62 = i;
  ((union ActorView *)riser)->bytes.unk63 = 1;
  riser->bounce = riser->x;
  riser->unk4c = 0;
  riser->gravity = riser->z;
  riser->watchedObject = second;
  riser->motionFlags = (u16)0;
  riser->priorityFlags = 1;
  Actor_SetSpriteColor(riser, 2);
  Actor_SelectAnimation(riser, 7);
  Actor_SetSpriteFlags(riser, 0);
  Actor_SetBlendMode(riser, 1);
  Actor_StartScript(riser, gData_02004014);
  i += 2;
  if (i != 32)
    goto column;
  Sound_Play(SE_SHIMMER);
  wait(50);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  PaletteFade_StartInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_ConfigureSpriteColor(firstId, 0);
  Actor_ConfigureSpriteColor(secondId, 0);
  Sound_Play(SE_574);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(80);
  Actor_SetMovementTypeById(firstId, gData_0200407c);
  Actor_SetMovementTypeById(secondId, gData_020040ac);
  second->update = Actor_EmitBubbles;
  wait(120);
  second->update = 0;
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Scene_PlayMusic();
  wait(1);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Actor_MoveToObject();

void AquaRockInside_ReadParchTablet(void) {
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)&MsgCheckedTheTabletIt, 1);
  if (Event_GetFlag(0x9ae)) {
    Event_End();
    goto L_02009c56;
  }
  FieldEvent_SetMessageId((s32)&MsgCheckedTheTabletIt);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), s16, 0) =
      (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  FieldEvent_Wait(1);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 256, 184);
  Actor_WalkTo(7, 274, 184);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  FieldActor_WaitForMoveById(7);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(5);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(30);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 184);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(7, 3);
  FieldEvent_Wait(30);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(7, 1);
  Actor_WalkToAndWait(7, 264, 168);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Message_ShowCentered(((s32)&MsgCheckedTheTabletIt + 2), 1);
  Func_02001620(14, 7);
  Party_LearnAbility(PIERS, ABILITY_PARCH);
  Event_SetFlag(0x9ae);
  Event_PlaySound(SE_TREMOR);
  FieldEvent_Wait(30);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Func_0200124c();
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  FieldEvent_Wait(30);
  FieldEvent_SetMessageId((s32)&MsgWhatWentWrong);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Actor_FaceEachOther(gMainRoutineWork.currentOwner, 7, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(7, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(7, 1.2);
  Actor_SetAnimation(7, 2);
  object = Actor_Get(gMainRoutineWork.currentOwner);
  if (object != 0) {
    Actor_SetDestination(7, FIELD_AT((u8 *)object, s16, 10),
                         FIELD_AT((u8 *)object, s16, 18));
  }
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Event_End();
L_02009c56:
}

void AquaRockInside_WalkToParchTablet(void) {
  if (Event_GetFlag(0x9ae)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgCheckedTheTabletIt, 1);
    Event_End();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 152);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 184);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 184);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    AquaRockInside_ReadParchTablet();
  }
}

#define DEFINE_AQUA_ROCK_INSIDE_READ_TABLET(name, n1)                          \
  void name(void) {                                                            \
    if (Event_GetFlag(0x9ae)) {                                                \
      Event_Start();                                                           \
      FieldEvent_ResolvePendingActions(0);                                     \
      Message_ShowCentered(MsgCheckedTheTabletIt, 1);                          \
      Event_End();                                                             \
    } else {                                                                   \
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, n1, 184);             \
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 184);            \
      Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);     \
      FieldEvent_Wait(10);                                                     \
      AquaRockInside_ReadParchTablet();                                        \
    }                                                                          \
  }

DEFINE_AQUA_ROCK_INSIDE_READ_TABLET(Func_02001cd8, 280)

DEFINE_AQUA_ROCK_INSIDE_READ_TABLET(Func_02001d40, 248)

extern struct Trigger gAquaRockInside_Triggers[];
extern struct Trigger gAquaRockInside_Room1Triggers[];
extern struct Trigger gAquaRockInside_Room2Triggers[];
extern struct Trigger gAquaRockInside_Room3Triggers[];
extern struct Trigger gAquaRockInside_Room4Triggers[];
extern struct Trigger gAquaRockInside_Room5Triggers[];
extern struct Trigger gAquaRockInside_Room6Triggers[];
extern struct Trigger gAquaRockInside_Room7Triggers[];
extern s16 gAquaRockInside_Room8Triggers[];
extern struct Trigger gAquaRockInside_Room9Triggers[];
extern struct Trigger gAquaRockInside_Room10Triggers[];

const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67cRoom1)
    return gAquaRockInside_Room1Triggers;
  if (scene == (s32)&SceneId_Ov67cRoom2)
    return gAquaRockInside_Room2Triggers;
  if (scene == (s32)&SceneId_Ov67cRoom3)
    return gAquaRockInside_Room3Triggers;
  if (scene == (s32)&SceneId_Ov67cRoom4)
    return gAquaRockInside_Room4Triggers;
  if (scene == (s32)&SceneId_Ov67cRoom5)
    return gAquaRockInside_Room5Triggers;
  if (scene == (s32)&SceneId_Ov67cRoom6)
    return gAquaRockInside_Room6Triggers;
  if (scene == (s32)&SceneId_Ov67cRoom7)
    return gAquaRockInside_Room7Triggers;
  if (scene == (s32)&SceneId_Ov67cRoom8)
    return gAquaRockInside_Room8Triggers;
  if (scene == (s32)&SceneId_Ov67cRoom9)
    return gAquaRockInside_Room9Triggers;
  if (scene == (s32)&SceneId_Ov67cRoom10)
    return gAquaRockInside_Room10Triggers;
  return gAquaRockInside_Triggers;
}

void Func_02001e78(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->sprite, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Actor_SelectAnimation(object, 2);
  }
}

void Func_02001ea0(struct Actor *object) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child =
        Import_ActorCreate_(0x10d, object->x, object->y, object->z);

    if (child != 0) {
      struct FieldSprite *sprite = child->sprite;

      Actor_StartScript(child, gData_020040dc);
      child->motionFlags = 0;
      child->terrainId = 1;
      if (sprite != 0) {
        Animation_ApplyChildArgument(sprite, 2);
        sprite->nextFrame = 0;
        sprite->rotation = 0x4000;
        sprite->priority = 2;
      }
    }
  }
}

void Func_02001f04(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *flags = &object->motionFlags;
  s32 saved = *flags;
  s32 speed;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Actor_SetSpriteColorMode(object, 9);
  *flags = 0;
  object->y = 0x500000;
  speed = 0x30000;
  i = 31;
fall:
  object->y -= speed;
  object->facing += 0x2000;
  speed += -0xccc;
  Actor_SetSpriteFlags(object, 0);
  System_Wait(1);
  i--;
  if (i >= 0)
    goto fall;
  Func_02002bd4(object);
  Actor_SetSpriteColorMode(object, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 2);
  *flags = saved;
  object->velocityY = 0x80000;
  Event_PlaySound(SE_SPLASH_SMALL);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 168, 200);
  Event_End();
}

void Func_02001fcc(void) {
  s32 counter = Func_02003bcc();

  switch (counter) {
  case 0:
    Func_02003bb8(0, 1);
    Func_02003bb8(1, 1);
    Scene_QueueMusic(SE_170);
    break;
  case 0x34bbf:
    Func_02003bb8(0, 0);
    Func_02003bb8(1, 0);
    Scene_QueueMusic(-1);
    break;
  }
  Vents_AdvanceCounter(0x34bc0);
}

extern u16 gData_020040f4[];
extern u16 gData_02004102[];
extern u16 gData_02004106[];
extern u32 gData_02004114[];

s32 Func_02003ab0(const u8 *a, const u8 *b, void (*handler)(void));

// Aqua Rock's entry setup: in each room it shows the statues already filled by
// their flags (or dims those still empty), opens what the story has opened and
// arms the room's water and cell handlers by entrance.
s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  if (scene == (s32)&SceneId_Ov67cRoom1) {
    if (GameFlag_Get(FLAG_SCENE_TEMP_30)) {
      Call1((void (*)())Func_02001e78, 8);
      Actor_SetAnimation(8, 3);
    } else {
      FieldActor_SetSpriteColor(Actor_GetById(8), 6);
    }
    if (GameFlag_Get(FLAG_SCENE_TEMP_31)) {
      Call1((void (*)())Func_02001e78, 9);
      Actor_SetAnimation(9, 3);
    } else {
      FieldActor_SetSpriteColor(Actor_GetById(9), 6);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom2) {
    if (GameFlag_Get(FLAG_SCENE_TEMP_40)) {
      MapGrid_CopyCellsLayered(118, 15, 5, 32, 110, 15);
      MapGrid_CopyCellAttributes(59, 16, 5, 32, 46, 16);
      FieldActor_SetSpriteColor(Actor_GetById(8), 6);
    } else {
      Call1((void (*)())Func_02001e78, 8);
      Actor_SetAnimation(8, 3);
    }
    object = Actor_GetById(9);
    object->motionFlags = 0;
    if (GameFlag_Get(0x95c)) {
      Actor_SetPosition(9, 632, 152);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, 39, 9);
      object->y = 0;
      object->terrainHeight = 0;
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom3) {
    if (entrance == 7)
      Leader_RiseFromWater();
  } else if (scene == (s32)&SceneId_Ov67cRoom4) {
    Call1((void (*)())Func_02001e78, 10);
    Actor_SetAnimation(10, 3);
    Call1((void (*)())Func_02001e78, 11);
    Actor_SetAnimation(11, 3);
    if ((u32)(entrance - 6) <= 1) {
      if (GameFlag_Get(FLAG_SCENE_TEMP_49)) {
        FieldActor_SetSpriteColor(Actor_GetById(9), 5);
        Call1((void (*)())Func_02001e78, 9);
        Actor_SetAnimation(9, 3);
        Func_02000b5c();
      } else {
        FieldActor_SetSpriteColor(Actor_GetById(9), 6);
      }
    }
    Actor_GetById(13)->riseCounter = 1;
    Actor_GetById(14)->riseCounter = 1;
    object = Actor_GetById(64);
    if (object != 0) {
      object->motionFlags = 0;
      object->terrainHeight = -0x200000;
      object->y = -0x200000;
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom5) {
    if ((u32)(entrance - 5) <= 1) {
      Func_02003ab0(gData_020040f4, gData_02004102, Func_02001fcc);
      Scene_QueueMusic(SE_170);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom6) {
    if (entrance == 5 || entrance == 7) {
      Func_02003ab0(gData_02004106, gData_02004114, Func_02001fcc);
      Scene_QueueMusic(SE_170);
      Func_0200107c(0, 10);
    }
    if (!GameFlag_Get(FLAG_SCENE_TEMP_38)) {
      Func_02001e78(9);
      Actor_SetAnimation(9, 3);
    }
    object = Actor_GetById(12);
    Actor_SetSpriteFlags(object, 0);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    object->update = (void (*)(void))Func_02001ea0;
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) && entrance == 13)
      Func_02001f04();
  } else if (scene == (s32)&SceneId_Ov67cRoom9) {
    if (GameFlag_Get(FLAG_SCENE_TEMP_4B)) {
      FieldActor_SetSpriteColor(Actor_GetById(9), 5);
      Func_02001e78(9);
      Actor_SetAnimation(9, 3);
    } else {
      FieldActor_SetSpriteColor(Actor_GetById(9), 6);
    }
    Actor_GetById(10)->riseCounter = 1;
    Actor_GetById(11)->riseCounter = 1;
    Actor_GetById(14)->riseCounter = 1;
    Actor_GetById(16)->riseCounter = 1;
    Actor_GetById(17)->riseCounter = 1;
    Actor_GetById(18)->riseCounter = 1;
  }
  return 0;
}

#include "text/msg_ids.h"

#define BG1_SET_PRIORITY_1()                                                   \
  {                                                                            \
    u16 value;                                                                 \
                                                                               \
    value = (control = REG_BG1CNT & 0xfffc);                                   \
    do {                                                                       \
      REG_BG1CNT = value | 1;                                                  \
    } while (0);                                                               \
  }

void Tightrope_Initialize();

// Aqua Rock's map preparation: for each room and entrance it sets the water
// level, places the statues and boulders the flags have moved, opens or
// closes the cells around them, and in the rooms whose water drains it shows
// the drained state once its flag is set. Room 6 clears its actors' x and z
// where the other rooms clear terrainHeight and y.
s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  volatile u16 control;
  // i1, i3, i4, i6, i7, i12, i14, i17 and b each hold one constant for their
  // whole life, i2 holds a flag test and the others each count one loop.
  s32 i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16,
      i17;
  struct Actor *actor;
  s32 a, b;
  u8 *map;

  ((struct MapBits *)Ram_HeapSlots->mapWork)->mode = 1;
  Func_02003278();
  if (scene == (s32)&SceneId_Ov67cRoom1) {
    if ((u32)(entrance - 3) <= 1 || entrance == 6 || entrance == 8) {
      Func_020032d4(33, 0, -2);
    } else if (entrance == 5 || entrance == 7) {
      actor = Actor_GetById(12);
      i1 = 0;
      actor->motionFlags = i1;
      actor->terrainHeight = i1;
      actor->y = i1;
      Call2((void (*)())Actor_SelectAnimation, (s32)actor, 0);
      actor = Actor_GetById(13);
      actor->motionFlags = i1;
      actor->terrainHeight = i1;
      actor->y = i1;
      i2 = GameFlag_Get(FLAG_SCENE_TEMP_30);
      if (i2 == 0) {
        MapGrid_CopyCellsLayered(104, 80, 16, 10, 104, 40);
        MapGrid_CopyCellsLayered(110, 116, 18, 12, 39, 105);
        // i2 is the flag test's 0.
        MapGrid_CopyCellAttributes(64, 32, 64, 32, i2, 32);
        Func_020032d4(33, 0, 0);
      } else {
        Call3((void (*)())Func_020032d4, 33, 4, 0);
        actor = Actor_GetById(12);
        Call2((void (*)())Func_0200349c, (actor->x >> 20) - 42, 0);
      }
      if (GameFlag_Get(FLAG_SCENE_TEMP_50)) {
        MapGrid_CopyCellAttributes(41, 42, 1, 1, 43, 44);
      }
    } else if ((u32)(entrance - 9) <= 1) {
      a = GameFlag_Get(FLAG_SCENE_TEMP_31);
      if (a == 0) {
        i3 = 32;
        MapGrid_CopyCellsLayered(77, 80, 23, 20, 77, i3);
        MapGrid_CopyCellsLayered(112, 96, 16, 20, 12, 98);
        // a is the flag test's 0.
        MapGrid_CopyCellAttributes(64, 32, 64, 32, a, i3);
        Func_020032d4(33, 0, 0);
      } else {
        Func_020032d4(33, 4, 0);
      }
    }
    if (GameFlag_Get(0x95a)) {
      Actor_SetPosition(10, 184, 200);
      i4 = 12;
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 9, i4);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 13, i4);
    }
    if (GameFlag_Get(0x95b)) {
      Actor_SetPosition(11, 824, 216);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom2) {
    if ((u32)(entrance - 1) <= 1) {
      Func_020032d4(33, 0, -2);
    }
    actor = Actor_GetById(8);
    actor->motionFlags = 0;
    actor->terrainHeight = 0;
    actor->y = 0;
  } else if (scene == (s32)&SceneId_Ov67cRoom3) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    Call2((void (*)())Actor_SetSpriteFlags, (s32)Actor_GetById(9), 0);
    Call2((void (*)())Actor_SetSpriteFlags, (s32)Actor_GetById(10), 0);
    Func_020032d4(33, 8, -2);
    Call3((void (*)())Func_020032d4, 40, 8, 8);
    if (GameFlag_Get(0xa66)) {
      Func_02003628();
      Value4((s32 (*)())Func_02003688, 16, 4, 16, 2);
      Value4((s32 (*)())Func_02003688, 22, 4, 16, 0);
      for (i5 = 0; i5 < 6; i5++)
        Func_020036b8(33, 55);
    }
    if (GameFlag_Get(0x95d)) {
      MapGrid_CopyCellsLayered(75, 96, 7, 13, 80, 27);
      i6 = 16;
      b = 31;
      MapGrid_CopyCellsLayered(91, 96, 7, 10, i6, b);
      MapGrid_CopyCellsLayered(99, 96, 7, 14, i6, 91);
      MapGrid_CopyCellAttributes(43, 0, 5, 9, 17, b);
      Func_020007a8();
    } else {
      MapGrid_CopyCellAttributes(17, 32, 5, 1, 17, 31);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom4) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    if ((u32)(entrance - 6) <= 1)
      goto statues;
    goto no_statues;
  statues:
    {
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Value3((s32 (*)())Func_02003420, 12, 5, 4);
      Value3((s32 (*)())Func_02003420, 13, 3, 4);
      Value3((s32 (*)())Func_02003420, 14, 4, 4);
      Func_020032d4(40, 4, 4);
      if (!GameFlag_Get(FLAG_SCENE_TEMP_49) && !Event_GetFlag(0xa64)) {
        map = (u8 *)Ram_HeapSlots->mapWork;
        i7 = 39;
        MapGrid_CopyCellsLayered(64, 64, 16, 13, 71, i7);
        MapGrid_CopyCellAttributes(71, 39, 16, 15, 7, i7);
        Func_02000b84();
        map[262] = 1;
        BG1_SET_PRIORITY_1();
      } else if (Event_GetFlag(0xa64) || GameFlag_Get(FLAG_SCENE_TEMP_48)) {
        if (Event_GetFlag(0xa64)) {
          Actor_SetPosition(12, 280, 712);
        }
        Func_02000b24(12, 38);
        Func_02003628();
        Value4((s32 (*)())Func_02003688, 12, 2, 45, 1);
        for (i8 = 0; i8 < 8; i8++)
          Func_020036b8(33, 55);
        Event_SetFlag(FLAG_SCENE_TEMP_49);
      }
      i9 = 12;
      do {
        actor = Actor_GetById(i9);
        actor->motionFlags = 0;
        actor->terrainHeight = 0;
        actor->y = 0;
        i9++;
      } while (i9 <= 14);
      goto statues_cleared;
    }
  no_statues:
    Func_020032d4(33, 0, -4);
  statues_cleared:
    if (GameFlag_Get(0xa6a)) {
      Func_02000ad8();
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom5) {
    Func_020032d4(33, 0, -2);
    Call3((void (*)())Func_020032d4, 40, 0, 0);
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    if (((u32)(entrance - 1) <= 3 || entrance == 7) && GameFlag_Get(0xa67)) {
      Func_02003628();
      Value4((s32 (*)())Func_02003688, 44, 0, 34, 2);
      for (i10 = 0; i10 <= 4; i10++)
        Func_020036b8(33, 55);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom6) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    if (entrance == 1 || entrance == 13) {
      Func_020032d4(33, 0, -2);
    } else if (entrance == 9 || entrance == 11 || entrance == 12) {
      Func_020032d4(33, 0, -2);
    }
    if (!GameFlag_Get(FLAG_SCENE_TEMP_38)) {
      i11 = 13;
      do {
        actor = Actor_GetById(i11);
        actor->motionFlags = 0;
        actor->x = 0;
        actor->z = 0;
        i11++;
      } while (i11 <= 15);
      goto done;
    } else {
      MapGrid_CopyCellsLayered(114, 39, 1, 2, 113, 39);
      i12 = 41;
      MapGrid_CopyCellAttributes(105, 41, 12, 13, i12, i12);
      MapGrid_CopyCellsLayered(105, 105, 12, 13, 105, i12);
      for (i13 = 13; i13 <= 15; i13++) {
        actor = Actor_GetById(i13);
        if (GameFlag_Get(i13 + 580)) {
          MapGrid_CopyCellAttributes(47, 45, 1, 1, actor->x >> 20,
                                     actor->z >> 20);
        }
      }
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom7) {
    Func_020032d4(33, 0, -2);
  } else if (scene == (s32)&SceneId_Ov67cRoom8) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    Func_020032d4(33, 0, -2);
    Tightrope_Initialize();
    Call3((void (*)())Tightrope_Place, 0, 8, 9);
  } else if (scene == (s32)&SceneId_Ov67cRoom9) {
    if ((u32)(entrance - 3) <= 1) {
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Value3((s32 (*)())Func_02003420, 10, 6, 4);
      Value3((s32 (*)())Func_02003420, 11, 3, 4);
      Value3((s32 (*)())Func_02003420, 12, 5, 4);
      Value3((s32 (*)())Func_02003420, 13, 6, 4);
      Value3((s32 (*)())Func_02003420, 14, 5, 4);
      Value3((s32 (*)())Func_02003420, 15, 4, 4);
      Value3((s32 (*)())Func_02003420, 16, 4, 4);
      Value3((s32 (*)())Func_02003420, 17, 3, 4);
      Value3((s32 (*)())Func_02003420, 18, 6, 4);
      Func_020032d4(40, 4, 4);
      if (!GameFlag_Get(FLAG_SCENE_TEMP_4B) && !Event_GetFlag(0xa65)) {
        map = (u8 *)Ram_HeapSlots->mapWork;
        i14 = 13;
        MapGrid_CopyCellsLayered(64, 64, 19, 17, 100, i14);
        MapGrid_CopyCellsLayered(35, 105, 23, 24, 35, 73);
        MapGrid_CopyCellAttributes(100, 13, 19, 19, 36, i14);
        map[262] = 1;
        BG1_SET_PRIORITY_1();
      } else if (Event_GetFlag(0xa65) || GameFlag_Get(FLAG_SCENE_TEMP_4A)) {
        if (Event_GetFlag(0xa65)) {
          Actor_SetPosition(12, 808, 296);
          Actor_SetPosition(13, 680, 360);
          Actor_SetPosition(15, 744, 424);
        }
        Call2((void (*)())Func_02000b24, 12, 38);
        Call2((void (*)())Func_02000b24, 13, 39);
        Func_02000b24(15, 37);
        Func_02003628();
        Value4((s32 (*)())Func_02003688, 38, 2, 19, 1);
        for (i15 = 0; i15 < 8; i15++)
          Func_020036b8(33, 55);
        Event_SetFlag(FLAG_SCENE_TEMP_4B);
      }
    }
    i16 = 10;
  clear_pillars:
    actor = Actor_GetById(i16);
    actor->motionFlags = 0;
    actor->terrainHeight = 0;
    actor->y = 0;
    i16++;
    if (i16 <= 20)
      goto clear_pillars;
    actor = Actor_GetById(19);
    MapGrid_CopyCellAttributes(23, 39, 1, 1, actor->x >> 20, 39);
    if ((u32)(entrance - 1) <= 1) {
      Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom10) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    Func_020032d4(33, 0, -2);
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
    if (GameFlag_Get(FLAG_AREA_TEMP_49)) {
      MapGrid_CopyCellAttributes(38, 9, 7, 6, 13, 9);
    } else {
      i17 = 8;
      MapGrid_CopyCellsLayered(77, 8, 7, 6, 50, i17);
      MapGrid_CopyCellsLayered(70, 8, 7, 6, 77, i17);
      MapGrid_CopyCellAttributes(38, 17, 7, 6, 13, 9);
    }
  }
done:
  return 0;
}

void Func_02002bd4(struct Actor *object) {
  struct Actor *child = FieldActor_Create(14, object->x, object->y + 0x40000,
                                          object->z + 0x40000);
  struct FieldSprite *source = object->sprite;

  if (child != 0) {
    struct FieldSprite *sprite = child->sprite;

    child->terrainHeight = object->terrainHeight;
    Actor_StartScript(child, gData_0200411c);
    child->motionFlags = 0;
    if (sprite != 0) {
      Animation_ApplyChildArgument(sprite, 1);
      sprite->nextFrame = 0;
      sprite->priority = source->priority;
    }
  }
}

DEFINE_ACTOR_EMIT_WATER_DROP(Actor_EmitWaterDrop, gWaterDropScript)

void Func_02002ca4(void) {
  struct Actor *center;
  s32 id;
  struct Actor *actor;
  s32 position[3];
  s32 hit;

  {
    struct SlideWork *work = Ram_HeapSlots->eventWork;

    id = gMainRoutineWork.currentOwner;
    center = work->camera;
  }
  actor = Actor_Get(id);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67bRoom1 &&
      actor->terrainHeight == 0)
    return;
  position[0] = actor->x;
  position[1] = actor->y;
  position[2] = actor->z + 0x20000;
  hit = MapGrid_CheckCollision(actor, position);
  if ((gFrameCount & 4) == 0)
    Func_02002bd4(actor);
  if (hit < 0) {
    Actor_SetAttachedEffect(id,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldActor_SetMoveTargetFixed(actor, actor->x, actor->y,
                                  actor->z + 0x80000);
    Actor_SelectAnimation(actor, 49);
    FieldActor_WaitForMove(actor);
    do {
      System_Wait(1);
    } while (actor->y != actor->terrainHeight);
    Func_02002bd4(actor);
    Actor_SelectAnimation(actor, 49);
    System_Wait(3);
    return;
  }
  position[0] = actor->x;
  position[1] = actor->y;
  position[2] = actor->z + 0x80000;
  hit = MapGrid_CheckCollision(actor, position);
  if (hit > 0)
    return;
  position[0] = actor->x + 0x5b333;
  position[1] = actor->y;
  position[2] = actor->z + 0x5b333;
  hit = MapGrid_CheckCollision(actor, position);
  if (hit > 0)
    return;
  position[0] = actor->x - 0x5b333;
  position[1] = actor->y;
  position[2] = actor->z + 0x5b333;
  hit = MapGrid_CheckCollision(actor, position);
  if (hit > 0)
    return;
  center->z += 0x20000;
  actor->z += 0x20000;
  actor->facing = 0x4000;
}

DEFINE_ACTOR_SHOW_PARTS(Actor_ShowParts, Actor_SelectAnimation)

DEFINE_LEADER_SPIN_AWAY(Leader_SpinAway, Actor_SelectAnimation, Func_02002bd4)

// On odd frames, spawns a drifting effect above the object, swaying with the
// frame count; Func_02002030 of Aqua Rock is the same function.
DEFINE_ACTOR_EMIT_MIST(Actor_EmitMist)

DEFINE_LEADER_SINK_AWAY(Leader_SinkAway, Actor_EmitMist)

DEFINE_LEADER_RISE_FROM_WATER(Leader_RiseFromWater, Actor_EmitMist)

// Draws the scene's pillars: a top sprite for each active one and, when it
// stands above its base, a column sprite as tall as its height.
void Func_020030f4(void) {
  s32 *view = &Ram_HeapSlots->mapWork->viewX;
  struct PillarWork *work = &gData_0200536c;
  s32 viewX = view[0];
  struct PillarEntry *entry = work->entries;
  s32 viewZ;
  s32 originY;
  s32 depth;
  s32 i;
  u32 tile;

  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = Ram_HeapSlots->mapWork->origin[1];
  tile = gResourceTableEntries[work->vramEntry].offset >> 5;

  for (i = 0; i < 32; i++) {
    if (entry->state != 0) {
      s32 *position = entry->position;
      s32 x;
      s32 y;
      s32 z;
      s32 screenY;
      s32 attributes;

      if (position != 0) {
        x = position[0] - viewX;
        y = position[1] + entry->height - originY;
        z = position[2] - viewZ - originY;
        screenY = z - y;
      } else {
        x = entry->x - viewX;
        y = entry->height - originY;
        z = entry->z - viewZ - originY;
        screenY = z - y;
      }
      if (entry->height != entry->base || entry->state == 2)
        attributes = 0x800;
      else
        attributes = 0xc00;
      x = (x >> 16) - 8;
      screenY = (screenY >> 16) - 8;
      depth = ((y + z) >> 16) + 50;
      if ((u32)(x + 15) <= 270) {
        s32 rise;

        if ((u32)(screenY + 47) <= 238) {
          x &= 511;
          screenY &= 255;
          rise = entry->kind * 8;
          entry->top[0] = 0;
          screenY |= x << 16;
          screenY |= 0x40002000;
          entry->top[1] = screenY;
          entry->top[2] = (tile + rise) | attributes;
          Oam_AddSpriteAtDepthClamped(entry->top, depth);
        }
        screenY = z - y;
        screenY = (screenY >> 16) + 8;
        if ((u32)(screenY + 47) <= 238) {
          rise = (entry->height - entry->base) >> 16;
          if (rise != 0) {
            u32 *column;
            rise--;
            screenY &= 255;
            column = entry->column;
            if ((u32)rise > 7)
              rise = 7;
            rise = rise * 4 + 64;
            column[0] = 0;
            screenY |= x << 16;
            screenY |= 0x6000;
            entry->column[1] = screenY;
            entry->column[2] = attributes | (tile + rise);
            Oam_AddSpriteAtDepthClamped(column, depth);
          }
        }
      }
    }
    entry++;
  }
}

void Func_02003278(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWordsEntry(&gData_0200536c, 0x604);
  buffer = MemAlloc_(0xc80);
  Decompress_TaggedLz(gData_0200414c, buffer);
  gData_0200536c.vramEntry = VramCache_FindFreeEntry();
  Vram_Load(gData_0200536c.vramEntry, 0xc80, buffer);
  Heap_Release(buffer);
  priority = 0x41c;
  FieldTask_AddFrame(Func_020030f4, priority);
}

#define LAYER_S32(layer, off) (*(s32 *)((layer)->unk08 + (off) - 8))

// Adds a water spout on every map cell of the given layer whose spout kind lies
// in [first, first + 8), at the given heights, and marks those cells as spout
// cells (120).
void Func_020032d4(u32 first, s32 level, s32 baseLevel) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *layer = map->layers;
  struct MapCell *cells = layer->cells;
  s32 originX = LAYER_S32(layer, 8) + map->minX;
  s32 originZ = LAYER_S32(layer, 12) + map->minZ;
  struct PillarWork *work = &gData_0200536c;
  s32 width = (map->maxX - map->minX) >> 20;
  s32 depth = (map->maxZ - map->minZ) >> 20;
  struct PillarEntry *spout = &work->entries[work->count];
  s32 corner = ((map->minZ >> 20) << 7) + (map->minX >> 20);
  s16 row;

  *(s32 *)(Data_0202c001 + 0x1df) = level << 8;
  cells += corner;
  row = 0;
  if (row < depth) {
    struct MapCell *cell;
    s16 column;
  rowTop:
    cell = cells + ((u16)row << 7);
    column = 0;
    if (column < width) {
    columnTop:
      {
        u32 kind = cell->collisionCode;

        if (kind != 0 && kind >= first && kind < first + 8) {
          spout->x = ((u16)column << 20) + originX + 0x80000;
          spout->z = ((u16)row << 20) + originZ + 0x80000;
          spout->base = baseLevel << 19;
          spout->height = level << 19;
          spout->kind = kind - first;
          spout->position = 0;
          spout->state = 1;
          spout++;
          work->count++;
          {
            u8 *terrain = map->layers[0].terrainIds + corner;

            terrain[(u16)column + ((u16)row << 7)] = 120;
          }
        }
      }
      {
        s32 next = (column << 16) + 0x10000;
        column = next >> 16;
        cell++;
        if ((s32)((u32)next >> 16) < width)
          goto columnTop;
      }
    }
    {
      s32 next = (row << 16) + 0x10000;
      row = next >> 16;
      if ((s32)((u32)next >> 16) < depth)
        goto rowTop;
    }
  }
}

void Func_02003420(s32 id, s32 kind, s32 height) {
  struct Actor *object = Actor_Get(id);
  struct PillarWork *work = &gData_0200536c;
  struct PillarEntry *entry = &work->entries[work->count];

  if (object != 0) {
    entry->x = 0;
    entry->z = 0;
    entry->base = height << 19;
    entry->height = height << 19;
    entry->state = 2;
    entry->kind = kind;
    entry->position = &object->x;
    work->count++;
  }
}

void Func_02003460(s32 height) {
  struct PillarWork *work = &gData_0200536c;
  struct PillarEntry *entry;
  s32 i;

  *(s32 *)(Ram_MapCollision + 0x1e0) = ((height / 0x80000) & 0xff) << 8;
  entry = work->entries;
  for (i = 31; i >= 0; i--) {
    if (entry->x != 0)
      entry->height = height;
    entry++;
  }
}

void Func_0200349c(u32 index, s32 height) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct PillarWork *work = &gData_0200536c;

  if (index <= 31) {
    struct PillarEntry *entry = &work->entries[index];

    if (entry->x != 0) {
      *(s32 *)(Ram_MapCollision + 0x1e4) = ((height / 0x80000) & 0xff) << 8;
      {
        u8 *ids = map->layers[0].terrainIds;
        s32 at = (entry->x >> 20) + (entry->z >> 20) * 128;
        s32 v = 121;
        ids[at] = v;
      }
      entry->height = height;
    }
  }
}

// Draws thirty-two sprites that each shake by a random few pixels a frame.
void Func_020034fc(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  struct BeamWork *splashes = &gData_02005970;
  s32 viewX = view[0];
  s32 viewZ;
  s32 originY;
  struct Splash *splash;
  s32 i;
  u32 tile;
  // The sprite word's priority bits (priority 2): a local that holds this one
  // value.
  s32 priority = 0x800;

  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = map->origin[1];
  tile = gResourceTableEntries[splashes->vramEntry].offset >> 5;
  splash = splashes->splashes;
  i = 31;

  do {
    if (splash->active != 0) {
      s32 x = splash->x - viewX;
      s32 y = splash->y - originY;
      s32 z = splash->z;
      s32 screenY, rawX, rawY;
      z -= viewZ;
      z -= originY;
      screenY = z - y;
      rawX = (x >> 16) + ((Random16() * 10) >> 16);

      x = rawX - 10;
      rawY = (screenY >> 16) + ((Random16() * 10) >> 16);
      screenY = rawY - 10;
      z = ((y + z) >> 16) + 50;

      if ((u32)(x + 15) <= 270 && (u32)(screenY + 47) <= 238) {
        u32 *frame = &splash->sprite[2];
        s32 column = x & 511;

        screenY &= 255;
        splash->sprite[0] = 0;
        splash->sprite[1] = screenY | (column << 16) | 0x40002000;
        *frame = (tile + (((Random16() * 3) >> 16) << 3)) | priority;
        Oam_AddSpriteAtDepthClamped(splash->sprite, z);
      }
    }
    i--;
    splash++;
  } while (i >= 0);
}

void Func_02003628(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWordsEntry(&gData_02005970, 0x3a4);
  buffer = MemAlloc_(0x400);
  Decompress_TaggedLz(gData_02004351, buffer);
  gData_02005970.vramEntry = VramCache_FindFreeEntry();
  Vram_Load(gData_02005970.vramEntry, 0x400, buffer);
  Heap_Release(buffer);
  priority = 0x41c;
  FieldTask_AddFrame(Func_020034fc, priority);
  Event_PlaySound(SE_SHIMMER);
}

s32 Func_02003688(s32 x, s32 y, s32 z, s32 kind) {
  struct BeamWork *work = &gData_02005970;
  struct BeamEntry *entry = work->entries;

  if (entry->x != 0)
    entry++;
  SET_POSITION(entry, (x << 20) + 0x80000, y << 20, (z << 20) + 0x80000);
  entry->direction = kind;
}

extern s8 gData_0200534c[];

// Advances Aqua Rock's two water streams one cell along the map: each stream
// follows the turn its cell's kind gives, leaves a splash on every open cell
// (255) it crosses, stops on a wall kind and counts the cells of the given kind
// it reaches; then it clears the splashes past the new count and opens the
// cells under those left.
s32 Func_020036b8(s32 first, s32 target) {
  s32 reached = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct BeamWork *state = &gData_02005970;
  s32 count = state->count;
  s32 turn;
  s32 again = 0;
  s32 i;
  struct BeamEntry *stream = state->entries;
  struct Splash *splash;

  for (i = 0; i < 2; i++) {
    if (stream->x != 0) {
      splash = &state->splashes[state->count];
      do {
        struct MapCell *cells = map->layers[0].cells;
        struct MapCell *cell =
            &cells[(stream->x >> 20) + (stream->z >> 20) * 128];
        s32 kind = cell->collisionCode;

        if (kind == 255) {
          splash->x = stream->x;
          splash->z = stream->z;
          splash->y = stream->y;
          splash->active = 1;
          splash++;
          state->count++;
          turn = 0;
          again = 1;
        } else if (kind >= first && kind <= first + 7) {
          turn = gData_0200534c[(stream->direction << 3) + kind - first];
          if (turn != 8)
            count = state->count;
          again = 0;
        } else if (kind == target) {
          turn = 8;
          count = state->count;
          reached++;
          again = 0;
        } else if (kind == 0) {
          turn = 8;
          again = 0;
        }
        if (turn != 8) {
          // The unsigned % 4: as & 3 the mask narrows to a halfword and pools.
          stream->direction = (u32)(stream->direction + turn) % 4;
          FieldVector_AddPolarOffset(0x100000, stream->direction << 14,
                                     &stream->x);
        }
      } while (again != 0);
      state->count = count;
    }
    stream++;
  }
  splash = &state->splashes[count];
  if (count < 32) {
    i = 32 - count;
  clear:
    if (splash->active != 0)
      splash->active = 0;
    splash++;
    if (--i != 0)
      goto clear;
  }
  splash = state->splashes;
  if (count != 0) {
    i = count;
  open:
    if (splash->active != 0) {
      struct MapCell *cells = map->layers[0].cells;
      struct MapCell *cell =
          &cells[(splash->x >> 20) + (splash->z >> 20) * 128];

      cell->collisionCode = 0;
      cell->attributeB |= 0x80;
    }
    splash++;
    if (--i != 0)
      goto open;
  }
  return reached;
}
#undef FIELD_AT
#undef BG1_SET_PRIORITY_1
#undef LAYER_S32

#define VENT_TOUCHING Func_02003844
#define VENT_BOUNCE Func_0200389c
#include "../field/object/vent_bounce.c"

DEFINE_ACTOR_START_SCRIPTED(Actor_StartScripted, Actor_SelectAnimation,
                            gData_020044c8)

void Vent_Puff(s32 *position, s32 base) {
  struct Actor *object = Import_ActorCreate_(0x11d, position[0], position[1],
                                             position[2] - 0x20000);
  s32 angle;

  if (object != 0) {
    object->bounce = Trig_Cos(((u32)(Random16() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16();
    object->unk4c = (u16)(Random16() << 1) + base;
    /* FAKEMATCH(temporary): the s32 local keeps the zero extension and the pool
     * word 0xffff8000. */
    angle = (u16)Random16() - 0x8000;
    object->timer = angle;
    Actor_StartScripted(object, 3);
    object->update = (void (*)(void))Func_0200389c;
    Actor_SetBlendMode(object, 1);
  }
}

DEFINE_VENTS_UPDATE(Vents_Update, Vent_Puff)

/* VIEW(s3844): routine Func_02003ab0 as void Func_02003ab0(const u16 *spots,
 * const u16 *levels, void (*handler)(void));
 * games/tla/src/scene/aqua_rock_inside/s1620 has s32 Func_02003ab0(const u8 *a,
 * const u8 *b, void (*handler)(void)); tried as the definition's void
 * Func_02003ab0(const u16 *spots, const u16 *levels, void(*handler) (void)),
 * which the object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/aqua_rock_inside.c:2404: conflicting types for
 * `Func_02003ab0'. */
void Func_02003ab0__s3844(const u16 *spots, const u16 *levels,
                          void (*handler)(void)) __asm__("Func_02003ab0");
#define Func_02003ab0 Func_02003ab0__s3844
void Func_02003ab0(const u16 *spots, const u16 *levels, void (*handler)(void)) {
  struct WaterWork *work = (struct WaterWork *)(gInventorySnapshot + 0x188);
  struct WaterOutlet *outlet = work->outlets;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 x;
    s32 i;

    Iwram_ClearWordsEntry(work, 0x150);
    x = *spots++;
    i = 0;
    while (x > 0) {
      s32 z = spots[0];
      s32 level = spots[1];

      outlet->state = 0;
      outlet->x = x << 16;
      outlet->z = z << 16;
      outlet->y = Map_GetTerrainHeight(0, x << 16, (z << 16) + 0x100000);
      outlet->level = level << 16;
      outlet->index = i++;
      spots += 2;
      outlet++;
      if (i > 15)
        break;
      x = *spots++;
    }
    if (levels != 0) {
      x = *levels++;
      i = 0;
      if (x > 0) {
      loop2:
        work->levels[i++] = x;
        if (i > 3)
          goto done2;
        x = *levels++;
        if (x > 0)
          goto loop2;
      }
    }
  done2:;
  }
  work->handler = handler;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = 0xc08;
      REG_BLDALPHA = alpha;
    }
    {
      s32 cnt = 0x3f10;
      REG_BLDCNT = cnt;
    }
  }
  Task_AddFrame(Vents_Update, 0x480);
}

s32 Func_02003ba4(s32 index) {
  u8 *base = (u8 *)VENT_WORK;

  return ((struct VentRow *)(base + index * 20))->active;
}

s32 Func_02003bb8(s32 index, s32 value) {
  u8 *base = (u8 *)VENT_WORK;

  ((struct VentRow *)(base + index * 20))->active = value;
}

s32 Func_02003bcc(void) { return VENT_WORK->counter; }

DEFINE_VENTS_ADVANCE_COUNTER(Vents_AdvanceCounter)

void Func_02003c00(s32 index, s32 value) {
  u8 *base = (u8 *)VENT_WORK;

  ((struct VentRow *)(base + index * 20))->value = value;
}

DEFINE_VENTS_SET_ID(Vents_SetId)

// clang-format off
u32 gData_02003f04[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003f40[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003f7c[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u8 gData_02003fb8[] = {67, 66, 75, 66, 83, 66, 91, 66, 99, 66, 107, 66, 115, 66, 67, 81, 75, 81, 83, 81, 91, 81, 99, 81, 107, 81, 115, 81, 67, 96, 75, 96};

u32 gData_02003fd8[] = {
    0, 6, 38, 17,
};

u32 gData_02003fe8[] = {
    5, 0, 0xa00000, 0, 1, 38,
    17,
};

DEFINE_G_DATA_02003584(gData_02004004, Orbiter_Spiral, 38, 17)

DEFINE_G_DATA_02004014(gData_02004014, Orbit_Update, Actor_AimAtWatched,
    Actor_StepToWatched)

u32 gData_02004034[] = {
    5, 0, 0x180000, 0, 1, 22,
    26, 4, 17,
};

u32 gData_02004058[] = {
    5, 0, 0x100000, 0, 1, 22,
    26, 4, 17,
};

s32 gData_0200407c[] = {
    22, 26, 0, 5, 0, -0x180000,
    0, 1, 22, 26, 0, 17,
};

s32 gData_020040ac[] = {
    22, 26, 0, 5, 0, -0x100000,
    0, 1, 22, 26, 3, 17,
};

u32 gData_020040dc[] = {
    0, 1, 13, 30, 0, 38,
};

u16 gData_020040f4[] = {
    280, 344, 2, 312, 344, 2, 0,
};

u16 gData_02004102[] = {
    8, 0,
};

u16 gData_02004106[] = {
    152, 536, 2, 216, 536, 2, 0,
};

u32 gData_02004114[] = {
    0xb000a, 0,
};

u32 gData_0200411c[] = {
    0, 20, 38,
};

u32 gWaterDropScript[] = {
    23, 6, 0x4000, 0, 1, 13,
    30, 0, 38,
};
// clang-format on

#undef Func_02003ab0
#undef VENT_TOUCHING
#undef VENT_BOUNCE
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
#undef FIELD_AT
#undef DEFINE_AQUA_ROCK_INSIDE_READ_TABLET
#undef BG1_SET_PRIORITY_1
#undef LAYER_S32
#undef BYTE_AT
#undef gData_02003fb8
#undef Func_02003ab0

void AquaRockInside_FightSteam(void);
void Func_020005f8(void);
void Func_02000b84(void);
void AquaRockInside_ReadParchTablet(void);

// clang-format off
LATE_DATA u32 gData_020044c8[] = {
    0, 44, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gData_02003f04,
    gData_02003f40,
    gData_02003f7c,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 320, 0, 320, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(160),
    SCENE_EXIT(1, 167, 1),
    SCENE_EXIT(2, 160, 3),
    SCENE_EXIT(3, 160, 2),
    SCENE_EXIT(4, 160, 5),
    SCENE_EXIT(5, 160, 4),
    SCENE_EXIT(6, 160, 7),
    SCENE_EXIT(7, 160, 6),
    SCENE_EXIT(8, 160, 9),
    SCENE_EXIT(9, 160, 8),
    SCENE_EXIT(10, 163, 2),
    SCENE_EXIT(11, 162, 1),
    SCENE_EXITS(161),
    SCENE_EXIT(1, 166, 1),
    SCENE_EXIT(2, 161, 3),
    SCENE_EXIT(3, 161, 2),
    SCENE_EXIT(4, 167, 5),
    SCENE_EXIT(5, 162, 2),
    SCENE_EXIT(6, 162, 8),
    SCENE_EXIT(7, 161, 8),
    SCENE_EXIT(8, 161, 7),
    SCENE_EXIT(9, 162, 6),
    SCENE_EXITS(162),
    SCENE_EXIT(1, 160, 11),
    SCENE_EXIT(2, 161, 5),
    SCENE_EXIT(3, 169, 1),
    SCENE_EXIT(4, 163, 4),
    SCENE_EXIT(5, 163, 5),
    SCENE_EXIT(6, 161, 9),
    SCENE_EXIT(7, 166, 4),
    SCENE_EXIT(8, 161, 6),
    SCENE_EXITS(163),
    SCENE_EXIT(1, 158, 4),
    SCENE_EXIT(2, 160, 10),
    SCENE_EXIT(3, 168, 2),
    SCENE_EXIT(4, 162, 4),
    SCENE_EXIT(5, 162, 5),
    SCENE_EXIT(6, 164, 6),
    SCENE_EXIT(7, 163, 8),
    SCENE_EXIT(8, 163, 7),
    SCENE_EXIT(9, 167, 4),
    SCENE_EXITS(164),
    SCENE_EXIT(1, 165, 1),
    SCENE_EXIT(2, 165, 2),
    SCENE_EXIT(3, 165, 3),
    SCENE_EXIT(4, 164, 5),
    SCENE_EXIT(5, 164, 4),
    SCENE_EXIT(6, 163, 6),
    SCENE_EXIT(7, 165, 12),
    SCENE_EXITS(165),
    SCENE_EXIT(1, 164, 1),
    SCENE_EXIT(2, 164, 2),
    SCENE_EXIT(3, 164, 3),
    SCENE_EXIT(4, 165, 5),
    SCENE_EXIT(5, 165, 4),
    SCENE_EXIT(6, 165, 7),
    SCENE_EXIT(7, 165, 6),
    SCENE_EXIT(8, 165, 9),
    SCENE_EXIT(9, 165, 8),
    SCENE_EXIT(10, 165, 11),
    SCENE_EXIT(11, 165, 10),
    SCENE_EXIT(12, 164, 7),
    SCENE_EXIT(13, 159, 1),
    SCENE_EXITS(166),
    SCENE_EXIT(1, 161, 1),
    SCENE_EXIT(2, 167, 3),
    SCENE_EXIT(3, 167, 2),
    SCENE_EXIT(4, 162, 7),
    SCENE_EXITS(167),
    SCENE_EXIT(1, 160, 1),
    SCENE_EXIT(2, 166, 3),
    SCENE_EXIT(3, 166, 2),
    SCENE_EXIT(4, 163, 9),
    SCENE_EXIT(5, 161, 4),
    SCENE_EXIT(6, 168, 4),
    SCENE_EXIT(7, 168, 3),
    SCENE_EXIT(8, 168, 1),
    SCENE_EXITS(168),
    SCENE_EXIT(1, 167, 8),
    SCENE_EXIT(2, 163, 3),
    SCENE_EXIT(3, 167, 7),
    SCENE_EXIT(4, 167, 6),
    SCENE_EXITS(169),
    SCENE_EXIT(1, 162, 3),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gAquaRockInside_Placements[] = {
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_Room1Placements[] = {
    {380, CONDITION_ALWAYS, 1, PIXELS(808), 0, PIXELS(679), FACING_SOUTH, 2, 0},
    {380, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(551), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(792), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {292, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(744), FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(712), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_Room2Placements[] = {
    {380, CONDITION_ALWAYS, 1, PIXELS(776), 0, PIXELS(247), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(616), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {244, 0x50, 1, PIXELS(776), 0, PIXELS(264), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_Room3Placements[] = {
    {383, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(264), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(672), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(672), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_Room4Placements[] = {
    {383, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {380, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(647), FACING_SOUTH, 2, 0},
    {380, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(87), FACING_SOUTH, 2, 0},
    {380, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(103), FACING_SOUTH, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(696), FACING_SOUTH, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(776), FACING_SOUTH, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(776), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_Room5Placements[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(360), FACING_SOUTH, 2, 0},
    {383, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(552), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_Room6Placements[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(776), 0, PIXELS(648), FACING_SOUTH, 2, 0},
    {380, CONDITION_ALWAYS, 1, PIXELS(792), 0, PIXELS(631), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(552), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(552), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(696), FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(808), 0, PIXELS(744), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_Room7Placements[] = {
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_Room8Placements[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_Room9Placements[] = {
    {383, CONDITION_ALWAYS, 1, PIXELS(616), 0, PIXELS(264), FACING_SOUTH, 2, 0},
    {380, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(231), FACING_SOUTH, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(296), FACING_SOUTH, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(616), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(680), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(712), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(776), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(808), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(680), 0, PIXELS(424), FACING_SOUTH, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(808), 0, PIXELS(424), FACING_SOUTH, 2, 1},
    {272, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(632), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(632), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gAquaRockInside_Room10Placements[] = {
    {312, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(248), FACING_SOUTH, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(312), FACING_SOUTH, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(312), FACING_SOUTH, 2, 1},
    {323, CONDITION_ALWAYS, 1, 0x64, 0, 0, FACING_SOUTH, 2, 2},
    {7, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_Triggers[] = {
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_Room1Triggers[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x51, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x51, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x51, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0xc602, 50, CONDITION_ALWAYS, EVENT(Func_020004b4)},
    {0x8602, 50, CONDITION_ALWAYS, EVENT(Func_020004b4)},
    {0x4602, 51, CONDITION_ALWAYS, EVENT(Func_020004b4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x95a, EVENT(Func_0200042c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x95b, EVENT(Func_0200042c)},
    {0x9, 0, CONDITION_ALWAYS, EVENT(Func_0200042c)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 8, 0x230, EVENT(Func_020004bc)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 9, 0x231, EVENT(Func_02000574)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, (s32)((u8 *)Data_020005f4 + 1)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, 0x250, EVENT(Func_020005f8)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 gData_02004d84 = 256;

LATE_DATA struct Trigger gAquaRockInside_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x21, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0x51, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {0x51, 8, CONDITION_ALWAYS, 8},
    {0x51, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(Func_02002ca4)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(AquaRockInside_FightSteam)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x95c, EVENT(Func_02000644)},
    {0x9, 0, CONDITION_ALWAYS, EVENT(Func_02000644)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 88, 0x240, EVENT(Func_02000690)},
    {TRIGGER_RAISED, 88, 0x240, EVENT(Func_0200071c)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_Room3Triggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {0x51, 2, CONDITION_ALWAYS, 2},
    {0xa02, 3, CONDITION_ALWAYS, EVENT(Func_02000958)},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0xc402, 7, CONDITION_ALWAYS, EVENT(Leader_SinkAway)},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(AquaRockInside_CheckColumn)},
    {0x1c814, 9, 0x95d, EVENT(Func_020007d4)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 8, 0xa66, EVENT(Func_02000a98)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x51, 2, CONDITION_ALWAYS, 2},
    {0x21, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {0x51, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {0x202, 55, CONDITION_ALWAYS, EVENT(Func_020004b4)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 12, CONDITION_ALWAYS, EVENT(Func_02000bac)},
    {0x8, 0, CONDITION_ALWAYS, EVENT(Func_02000bac)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT(Func_02000b84)},
    {0x9, 0, CONDITION_ALWAYS, EVENT(Func_02000b84)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 50, 0xa6a, EVENT(Func_02000ad8)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 8, 0xa64, EVENT(Func_02000bd4)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 9, 0x249, EVENT(Func_02000c28)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_Room5Triggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {0x51, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 77, CONDITION_ALWAYS, EVENT(Func_02000cc8)},
    {TRIGGER_STEP, 78, CONDITION_ALWAYS, EVENT(Func_02000cc8)},
    {0x4602, 70, CONDITION_ALWAYS, EVENT(Func_020004b4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 9, 0xa67, EVENT(Func_02000de4)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_Room6Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0x51, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x51, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0x51, 11, CONDITION_ALWAYS, 11},
    {0x51, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(Func_02000e18)},
    {TRIGGER_STEP, 77, CONDITION_ALWAYS, EVENT(Func_02000cc8)},
    {TRIGGER_STEP, 78, CONDITION_ALWAYS, EVENT(Func_02000cc8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x238, EVENT(Func_02000ecc)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, 0x251, EVENT(Func_0200101c)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 14, 0x252, EVENT(Func_0200101c)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 15, 0x253, EVENT(Func_0200101c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 10, CONDITION_ALWAYS, EVENT(Func_02001060)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 11, CONDITION_ALWAYS, EVENT(Func_02001060)},
    {0x8, 0, CONDITION_ALWAYS, EVENT(Func_02001060)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(Func_0200107c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT(Func_0200107c)},
    {0x9, 0, CONDITION_ALWAYS, EVENT(Func_0200107c)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_Room7Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {0x21, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXAMINE, 30, CONDITION_ALWAYS, EVENT(AquaRockInside_CheckPsynergyStone)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s16 gAquaRockInside_Room8Triggers[] = {
    49, 0, 1, -1, 1, 0, 49, 0,
    2, -1, 2, 0, 49, 0, 3, -1,
    3, 0, 33, 0, 4, -1, 4, 0,
    49, 0, 5, -1, 5, 0, 1, 0,
    6, -1, 6, 0, 1, 0, 7, -1,
    7, 0, 1, 0, 8, -1, 8, 0,
    -1, -1, 0, 0, 0, 0,
};

LATE_DATA struct Trigger gAquaRockInside_Room9Triggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(Func_02002ca4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(Func_02001138)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 8, 0xa65, EVENT(Func_02001170)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 9, 0x24b, EVENT(Func_020011d4)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gAquaRockInside_Room10Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xc403, 30, CONDITION_ALWAYS, EVENT(AquaRockInside_ReadParchTablet)},
    {0x4403, 30, CONDITION_ALWAYS, EVENT(AquaRockInside_WalkToParchTablet)},
    {0x8403, 30, CONDITION_ALWAYS, EVENT(Func_02001cd8)},
    {0x403, 30, CONDITION_ALWAYS, EVENT(Func_02001d40)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 40, IF_FLAG_SET(0x349), EVENT(Func_02001328)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s8 gData_0200534c[] = {
    8, 8, 0, 8, -1, 8, 1, 8,
    8, 0, 8, -1, 1, 8, 8, 8,
    8, 8, 0, 1, 8, -1, 8, 8,
    8, 0, 8, 8, 8, 1, -1, 8,
};
// clang-format on
