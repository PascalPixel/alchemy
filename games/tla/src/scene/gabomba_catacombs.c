/// @file
/// The scene code of overlay 0x673, in ROM order.

#include "flags.h"
#include "shared/tables.h"
#include "event_work.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "system/frame.h"
#include "field/event_inline.h"
#include "field/field.h"

void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_StartScript(struct Actor *object, const void *script);
struct GabombaCatacombsVec;
s32 MapGrid_CheckCollision(struct Actor *object,
                           struct GabombaCatacombsVec *position);
void Event_End(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void Scene_TalkToActor(s32 a);
void Cyclone_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void FieldItem_JumpToLeader(s32 a, s32 b, s32 c);

extern u32 gData_0200128c[];

extern char SceneId_Ov673Room1;
extern char SceneId_Ov673Room2;
extern char SceneId_Ov673Room3;
extern char SceneId_Ov673Room4;
extern char SceneId_Ov673Room5;

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02001274_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  Cyclone_CopyTouchedCells(a, b, c, d, e, f, g);
}

static __inline__ void Func_0200127c_(s32 a, s32 b, s32 c) {
  FieldItem_JumpToLeader(a, b, c);
}

struct GabombaCatacombsVec {
  s32 x;
  s32 y;
  s32 z;
};

struct GabombaCatacombsFollower {
  u8 unk00[0x10];
  s32 z;
};

struct GabombaCatacombsEventWork {
  u8 unk000[0x1cc];
  struct GabombaCatacombsFollower *follower;
};

struct GabombaCatacombsParty {
  u8 unk000[0x254];
  u16 value254;
};

#include "field/event_inline.h"

/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern u32 gFieldEffect_MediumScript[];
extern u32 gData_02001310[];
extern struct ActorPlacement gData_02001378[];
extern struct ActorPlacement gData_02001390[];
extern struct ActorPlacement gData_020013a8[];
extern struct ActorPlacement gData_020013d8[];
extern struct ActorPlacement gData_02001498[];
extern struct ActorPlacement gData_02001510[];
extern struct Trigger gData_0200154c[];
extern struct Trigger gData_02001594[];
extern struct Trigger gData_020015c4[];
extern struct Trigger gData_020016cc[];
extern struct Trigger gData_02001714[];

void Actor_DepartInRing(s32 id);

#include "module/scene/scene.h"
#include "text/msg_ids.h"

#define LEADER_HOP()                                                           \
  do {                                                                         \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x102);             \
  } while (0)

void Func_02000038(void) { Field_RequestDjinnBattle(8, 10); }

void Func_02000044(void) { Func_02001274_(7, 63, 1, 16, 16, 57, 63); }

void Func_02000060(void) { Func_02001274_(22, 15, 0, 5, 5, 63, 52); }

void Func_02000080(void) { Func_02001274_(7, 22, 0, 3, 3, 56, 58); }

void Func_020000a0(void) { Func_02001274_(44, 25, 0, 4, 3, 66, 57); }

void Func_020000c0(void) { Func_02001274_(16, 13, 0, 4, 8, 61, 52); }

void Func_020000e0(void) { Func_02001274_(21, 9, 0, 2, 2, 57, 52); }

void Func_02000100(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001274_(22, 15, 0, 5, 5, 63, 52);
  if (GameFlag_Get(0xf53) == 0) {
    Actor_SetPosition(64, PIXELS(392), PIXELS(344));
    if (leader->x >> 20 == 24 && leader->z >> 20 == 21) {
      LEADER_HOP();
      Actor_SetDestinationOffsetById(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = PIXELS(6);
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Func_0200018c(void) {
  MapGrid_CopyCellsLayered(51, 83, 3, 1, 9, 65);
  Import_EventSetExitRequest(3);
}

void Func_020001b0(void) {
  MapGrid_CopyCellsLayered(51, 83, 3, 1, 18, 66);
  Import_EventSetExitRequest(4);
}

void Func_020001d4(void) { Import_EventSetExitRequest(5); }

#define DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(name, n1, n2, n3, n4, n5, \
                                                     n6, n7, n8, n9)           \
  void name(void) {                                                            \
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);       \
                                                                               \
    Func_02001274_(n1, n2, 0, n3, 3, 56, n4);                                  \
    Actor_SetPosition(n5, PIXELS(n6), PIXELS(n7));                             \
    if (leader->x >> 20 == n8 && leader->z >> 20 == n9) {                      \
      LEADER_HOP();                                                            \
      Actor_SetDestinationOffsetById(gMainRoutineWork.currentOwner, 16, 0);    \
      leader->velocityY = PIXELS(6);                                           \
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);                    \
      Event_End();                                                             \
    }                                                                          \
  }

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(Func_020001e0, 7, 22, 3, 58, 8,
                                             136, 376, 8, 23)

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(Func_02000260, 5, 13, 5, 52, 9,
                                             136, 248, 8, 15)

void Func_020002e0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001274_(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(10, PIXELS(152), PIXELS(264));
  if (leader->x >> 20 == 9 && leader->z >> 20 == 16) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x102);
    Actor_SetDestinationOffsetById(gMainRoutineWork.currentOwner, -16, 0);
    leader->velocityY = PIXELS(6);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Event_End();
  }
}

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(Func_02000364, 5, 13, 5, 52, 11,
                                             136, 280, 8, 17)

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(Func_020003e4, 5, 13, 5, 52, 12,
                                             104, 280, 6, 17)

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(Func_02000464, 5, 13, 5, 52, 13,
                                             88, 264, 5, 16)

DEFINE_GABOMBA_CATACOMBS_SHIFT_CELLS_AND_HOP(Func_020004e4, 5, 13, 5, 52, 14,
                                             104, 248, 6, 15)

void Func_02000564(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001274_(16, 13, 0, 4, 8, 61, 52);
  Actor_SetPosition(8, PIXELS(280), PIXELS(296));
  if (leader->x >> 20 == 17 && leader->z >> 20 == 18) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x102);
    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -16);
    leader->velocityY = PIXELS(6);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Event_End();
  }
}

void Func_020005e8(void) { Func_02001274_(7, 23, 0, 4, 5, 60, 56); }

void Func_02000608(void) {
  Func_02001274_(7, 23, 0, 4, 5, 60, 56);
  if (GameFlag_Get(0x9b5) == 0) {
    Func_0200127c_(249, 168, 424);
    ((struct GabombaCatacombsParty *)&gMainRoutineWork)->value254 = 0x19b5;
    gMainRoutineWork.battleRule24b = 2;
    Field_RequestBattleFromEncounterTable(106, 0);
  }
}

void Func_0200066c(void) {
  MapGrid_CopyCellPictureCellsRect(0, 0, 1, 2, 7, 11);
  Map_CopyCellAttributeRect(2, 4, 1, 1, 7, 14);
}

void Func_0200069c(void) {
  MapGrid_CopyCellPictureCellsRect(1, 0, 1, 2, 7, 11);
  Map_CopyCellAttributeRect(2, 5, 1, 1, 7, 14);
}

#define DEFINE_GABOMBA_CATACOMBS_SET_MOVED(name, n1, n2, n3, n4, n5, n6)       \
  void name(void) {                                                            \
    struct Actor *object = Actor_GetById(n1);                                  \
                                                                               \
    GameFlag_Set(n2);                                                          \
    object->motionFlags = 0;                                                   \
    object->priorityFlags = 0;                                                 \
    MapGrid_CopyCellAttributes(n3, n4, 1, 1, n5, n6);                          \
  }

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(Func_020006cc, 8, 0x200, 6, 23, 8, 23)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(Func_02000704, 9, 0x201, 0, 4, 8, 15)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(Func_02000740, 10, 0x202, 0, 4, 9, 16)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(Func_0200077c, 11, 0x203, 0, 4, 8, 17)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(Func_020007b8, 12, 0x204, 0, 4, 6, 17)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(Func_020007f0, 13, 0x205, 0, 4, 5, 16)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(Func_0200082c, 14, 0x206, 0, 4, 6, 15)

DEFINE_GABOMBA_CATACOMBS_SET_MOVED(Func_02000868, 8, 0x200, 15, 18, 17, 18)

void Func_020008a0(struct Actor *object, s32 dz) {
  struct GabombaCatacombsFollower *follower;
  struct GabombaCatacombsVec position;

  follower =
      ((struct GabombaCatacombsEventWork *)Ram_HeapSlots->eventWork)->follower;

  position.x = object->x;
  position.y = object->y;
  position.z = object->z + dz;
  MapGrid_CheckCollision(object, &position);
  position.x = object->x;
  position.y = object->y;
  position.z = object->z + 0x80000;
  if (MapGrid_CheckCollision(object, &position) <= 0) {
    position.x = object->x + 0x5b333;
    position.y = object->y;
    position.z = object->z + 0x5b333;
    if (MapGrid_CheckCollision(object, &position) <= 0) {
      position.x = object->x - 0x5b333;
      position.y = object->y;
      position.z = object->z + 0x5b333;
      if (MapGrid_CheckCollision(object, &position) <= 0) {
        follower->z += dz;
        object->z += dz;
      }
    }
  }
}

void Func_0200094c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *steam;

  if ((gFrameCount & 7) == 0) {
    steam = ((struct Actor *)Import_ActorCreate(
        14, leader->x, leader->y + 0x60000, leader->z));
    if (steam) {
      Actor_SetSpriteFlags(steam, 0);
      Actor_SelectAnimation(steam, 1);
      Actor_StartScript(steam, gData_0200128c);
    }
    Sound_Play(0x26f);
  }
  Func_020008a0(leader, 0xd999);
}

s32 Func_020009bc(struct Actor *object) {
  object->scaleX -= 3276;
  object->scaleY -= 3276;
  return 0;
}

#include "types.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Actor_MoveToObject(s32 actor, s32 target);

#define TALK()                                                                 \
  EventMessage_ShowCentered((s16)((union EventLine *)work)->line++, 0, -3)

void Func_020009d4(void) {
  struct Actor *follower = Actor_GetById(9);
  struct Actor *statue = Actor_GetById(0x40);
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  struct Actor *glow, *leader;
  s32 speed;

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x403108, 1);
  PaletteFade_StartInterpolation(60);
  FieldCamera_MoveTo(0xa80000, -1, 0x9e0000, 1);
  Sound_Play(26);
  Event_Wait(60);
  Actor_SetModeById(8, 2);
  Sound_Play(0x240);
  Event_Wait(60);
  Event_SetMessageId((s32)MsgByReachingThis);
  work += 0x1c4;
  TALK();
  Actor_SetPosition(0x40, 0xa80000, 0x980000);
  statue->motionFlags = 6;
  statue->terrainHeight += 0x100000;
  Sound_Play(220);
  glow = ((struct Actor *)Import_ActorCreate(412, 0, 0, 0));
  Actor_StartScript(glow, gFieldEffect_MediumScript);
  Actor_SetSpriteColorMode(glow, 1);
  glow->motionFlags = 0;
  glow->x = statue->x;
  glow->y = statue->y + 0x80000;
  glow->z = statue->z;
  glow->terrainHeight = statue->terrainHeight;
  glow->update = Func_020009bc;
  Event_Wait(15);
  glow->x = 0;
  glow->y = 0;
  glow->z = 0;
  glow->update = 0;
  TALK();
  Actor_SetDestinationById(gMainRoutineWork.currentOwner, 166, 188);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  speed = 0x10000;
  follower->speedLimit = speed;
  Actor_SetDestinationById(9, 184, 188);
  Actor_WaitForMoveById(9);
  Actor_FaceDirection(9, 0x8000, 0);
  Event_ShowMessageAndWait(9, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  TALK();
  Actor_ShowEmoteById(9, 258, 0);
  Actor_FaceDirection(9, 0xb000, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(9, 0);
  TALK();
  Actor_ShowEmote(9, 261, 0);
  Event_Wait(30);
  TALK();
  Actor_ShowEmoteById(9, 265, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(9, 0);
  TALK();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0x8000, 0);
  Event_Wait(10);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 257, 0);
  Actor_ShowEmoteById(9, 257, 0);
  Event_Wait(60);
  TALK();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_FaceDirection(9, 0xb000, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 261, 0);
  Actor_ShowEmoteById(9, 261, 0);
  Event_Wait(60);
  TALK();
  Event_Wait(40);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Event_Wait(40);
  Actor_SetModeById(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(8, 2);
  TALK();
  Event_Wait(30);
  Actor_SetModeById(8, 4);
  Actor_SetModeById(9, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestinationById(9, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(9);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_WaitForMoveById(9);
  GameFlag_Set(0x9ce);
  Scene_PlayMusic();
  PaletteFade_ApplyTarget(speed, 0);
  PaletteFade_StartInterpolation(60);
  FieldCamera_MoveToActorById(gMainRoutineWork.currentOwner, 1);
  Event_Wait(60);
  Event_End();
}

void Func_02000d90(void) {
  struct Actor *object = Actor_GetById(64);

  if (object != NULL && object->x != 0)
    Scene_TalkToActor(0x1040);
}

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room1)
    return (const struct SceneRegion *)gData_02001310;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct Trigger gData_02001540[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room1)
    return (const struct ActorPlacement *)gData_02001390;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room2)
    return (const struct ActorPlacement *)gData_020013a8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room3)
    return (const struct ActorPlacement *)gData_020013d8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room4)
    return (const struct ActorPlacement *)gData_02001498;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room5)
    return (const struct ActorPlacement *)gData_02001510;
  return (const struct ActorPlacement *)gData_02001378;
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room1)
    return (const struct Trigger *)gData_0200154c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room2)
    return (const struct Trigger *)gData_02001594;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room3)
    return (const struct Trigger *)gData_020015c4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room4)
    return (const struct Trigger *)gData_020016cc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room5)
    return (const struct Trigger *)gData_02001714;
  return (const struct Trigger *)gData_02001540;
}

#include "version.h"

#if !TLA_PATCH_1
#define CATACOMBS_SCREEN_EFFECT 513
#else
#define CATACOMBS_SCREEN_EFFECT 516
#endif

extern char SceneId_Ov671Room7, SceneId_Ov672Room1;
void FieldEffect_StartBlankOverlay(s32 a);

s32 Scene_Initialize(void) {
  s32 entrance;
  struct Actor *statue;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      CATACOMBS_SCREEN_EFFECT;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room1 &&
      (entrance = gMainRoutineWork.entrance) == 1) {
    if (GameFlag_Get(0xa27)) {
      gMainRoutineWork.retreatScene = (s32)&SceneId_Ov671Room7;
      gMainRoutineWork.retreatEntrance = entrance;
    } else if (GameFlag_Get(0xa28)) {
      gMainRoutineWork.retreatScene = (s32)&SceneId_Ov672Room1;
      gMainRoutineWork.retreatEntrance = 3;
    }
  }
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineWork.scene == (s32)&SceneId_Ov673Room2 &&
      gMainRoutineWork.entrance > 2)
    Actor_DepartInRing(gMainRoutineWork.currentOwner);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room3) {
    if (GameFlag_Get(0x200))
      MapGrid_CopyCellAttributes(6, 23, 1, 1, 8, 23);
    if (GameFlag_Get(513))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 15);
    if (GameFlag_Get(514))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 9, 0x10);
    if (GameFlag_Get(515))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 17);
    if (GameFlag_Get(516))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 17);
    if (GameFlag_Get(517))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 5, 0x10);
    if (GameFlag_Get(518))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 15);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room5 && GameFlag_Get(0x200))
    MapGrid_CopyCellAttributes(15, 18, 1, 1, 17, 18);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room4 &&
      GameFlag_Get(0x9ce) && !GameFlag_Get(0xf93) &&
      !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    statue = Actor_GetById(0x40);
    Actor_SetPosition(0x40, 0xa80000, 0x980000);
    statue->motionFlags = 6;
    statue->terrainHeight += 0x100000;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "objcmd.h"

// clang-format off
u32 gData_0200128c[] = {
    0, 15, 3, 0, 1, 38,
};

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_02001310[] = {
    0x2e0184, 0x18c0114, 0x11c0036, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(126),
    SCENE_EXIT(1, 124, 2),
    SCENE_EXIT(2, 127, 1),
    SCENE_EXIT(3, 127, 3),
    SCENE_EXIT(4, 127, 4),
    SCENE_EXIT(5, 127, 5),
    SCENE_EXITS(127),
    SCENE_EXIT(1, 126, 2),
    SCENE_EXIT(2, 128, 1),
    SCENE_EXITS(128),
    SCENE_EXIT(1, 127, 2),
    SCENE_EXIT(2, 130, 1),
    SCENE_EXITS(129),
    SCENE_EXIT(1, 130, 2),
    SCENE_EXITS(130),
    SCENE_EXIT(1, 128, 2),
    SCENE_EXIT(2, 129, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02001378[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001390[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020013a8[] = {
    {243, 0x3a, 1, PIXELS(728), 0, PIXELS(312), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_020013D8(gData_020013d8, 290, 0, 0, 0, 290, 0, 0, 0, 0)

struct ActorPlacement gData_02001498[] = {
    {375, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001510[] = {
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gData_02001540[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gData_0200154c[] = {
    {0x31, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41,
     CONDITION_ALWAYS, EVENT(Func_02000044)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43,
     CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 44,
     CONDITION_ALWAYS, EVENT(Func_020001b0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 45,
     CONDITION_ALWAYS, EVENT(Func_020001d4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02001594[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0xc400, 8, CONDITION_ALWAYS, EVENT(Func_02000038)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020015c4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Func_0200066c)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Func_0200069c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40,
     CONDITION_ALWAYS, EVENT(Func_02000060)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41,
     CONDITION_ALWAYS, EVENT(Func_02000080)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 42,
     CONDITION_ALWAYS, EVENT(Func_020001e0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43,
     CONDITION_ALWAYS, EVENT(Func_02000260)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 44,
     CONDITION_ALWAYS, EVENT(Func_020002e0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 45,
     CONDITION_ALWAYS, EVENT(Func_02000364)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 46,
     CONDITION_ALWAYS, EVENT(Func_020003e4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 47,
     CONDITION_ALWAYS, EVENT(Func_02000464)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 48,
     CONDITION_ALWAYS, EVENT(Func_020004e4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 49,
     CONDITION_ALWAYS, EVENT(Func_02000100)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x200, EVENT(Func_020006cc)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x201, EVENT(Func_02000704)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, 0x202, EVENT(Func_02000740)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x203, EVENT(Func_0200077c)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, 0x204, EVENT(Func_020007b8)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, 0x205, EVENT(Func_020007f0)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 14, 0x206, EVENT(Func_0200082c)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020016cc[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 40, 0x9ce, EVENT(Func_020009d4)},
    {TRIGGER_TALK, 8, 0xf93, EVENT(Func_02000d90)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(Func_020005e8)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(Func_02000608)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02001714[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40,
     CONDITION_ALWAYS, EVENT(Func_020000a0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41,
     CONDITION_ALWAYS, EVENT(Func_020000c0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43,
     CONDITION_ALWAYS, EVENT(Func_020000e0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 42,
     CONDITION_ALWAYS, EVENT(Func_02000564)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(Func_0200094c)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x200, EVENT(Func_02000868)},
    {TRIGGER_TABLE_END},
};
