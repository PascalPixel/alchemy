
#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"
/// @file
/// The Kandorean Temple of Master Poi, south-west of Daila, in nine rooms that
/// share this overlay (SceneId_Ov653Room1 to SceneId_Ov653Room9).
///
/// Its gate is shut for the meditation (KandoreanTemple_StartGate): Kraden
/// spots a suspicious wall and the party finds a hidden way in through a cave
/// (KandoreanTemple_FindTheHiddenPassage). Inside, Poi tests three students
/// (KandoreanTemple_TestTheStudents), and the party follows the one who can
/// levitate through the temple's trials: a hot floor, steam that pushes the
/// leader along, two lifts, two air vents whose platforms a pillar holds down
/// (KandoreanTemple_RunLeftVent, KandoreanTemple_RunRightVent), and three
/// Chestbeaters (KandoreanTemple_FightChestbeaters). Poi then gives the party
/// the Lash Pebble (KandoreanTemple_ReceiveLash), and Lash reaches Fog, the
/// Mercury Djinni (KandoreanTemple_FightFog).
///
/// This file is the overlay's own copy of the effect spawner
/// (FieldEffect_Spawn); the rest of the temple is in kandorean_temple/.

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect/spawn.c"

/// The Kandorean Temple's rooms: their tables' getters, the steam, vents and
/// lifts.

#include "flags.h"
#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "system/iwram_call.h"
#include "field_sprite.h"
#include "system/input.h"
#include "field/event_inline.h"
#include "system/frame.h"

void Actor_SetSpriteFlags(struct Actor *object, u32 flags);

void Event_End(void);
void KandoreanTemple_TrailSteam(struct Actor *object, s32 a);

void Actor_StartScript(struct Actor *object, const void *data);
void Actor_SelectAnimation(struct Actor *object, s32 a);
s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);
void Animation_ApplyChildArgument(void *sprite, s32 a);
void KandoreanTemple_PuffSteam(struct Actor *object);
void KandoreanTemple_PushNorth(struct Actor *object, s32 dz);
void Actor_DisableById(s32 id);
void BattleEffect_CleanupSceneObjects(void);
void KandoreanTemple_BlowSteam(s32 *position, s32 value);
void KandoreanTemple_BlowStreamSteam(void);
extern s32 gKandoreanTemple_LeftVentBlowing;
extern s32 gKandoreanTemple_RightVentBlowing;
extern s32 gKandoreanTemple_SteamCounter;
extern s32 gKandoreanTemple_SteamScript[];
extern u32 gKandoreanTemple_PuffScript[];
extern u32 gKandoreanTemple_StreamPuffScript[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Actor_GetCellIndex(void);
void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void FieldEffect_StartBlankOverlay(s32 a);
void KandoreanTemple_StartLiftRoom(void);
void KandoreanTemple_StartRoom2(void);
void KandoreanTemple_StartChestbeaterRoom(void);
void KandoreanTemple_StartVentRoom(void);
void KandoreanTemple_StartRoom5(void);
void KandoreanTemple_StartRoom6(void);
void KandoreanTemple_StartFogRoom(void);
void KandoreanTemple_StartGate(void);
void KandoreanTemple_StartPoisHall(void);
extern char SceneId_Ov653Room1;
extern char SceneId_Ov653Room2;
extern char SceneId_Ov653Room3;
extern char SceneId_Ov653Room4;
extern char SceneId_Ov653Room5;
extern char SceneId_Ov653Room6;
extern char SceneId_Ov653Room7;
extern char SceneId_Ov653Room8;
extern char SceneId_Ov653Room9;
void KandoreanTemple_ShrinkColumn(s32 id, s32 dy, s32 scale);
void Scene_QueueMusic(s32 a);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void Actor_MoveToObject(s32 id, s32 target);
void *HeapSlot_Alloc(s32 slot, s32 size);
extern struct Actor *gKandoreanTemple_LashPebble;
extern s32 gKandoreanTemple_MeditateScript[];
extern s32 gKandoreanTemple_LevitateScript[];

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

void KandoreanTemple_AskAboutPoi(void);
void KandoreanTemple_TellOfTheMeditation(void);
void KandoreanTemple_OpenTheTrapdoor(void);
void KandoreanTemple_LeaveBySteam(void);
extern u32 gKandoreanTemple_LookAroundScript[];
extern s16 gKandoreanTemple_BlockActors[];
extern s16 gKandoreanTemple_PillarActors[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_SetSingleChildFlags_(struct Actor *object,
                                                  s32 flags) {
  Actor_SetSpriteFlags(object, flags);
}

static __inline__ struct Actor *Func_02005140_(s32 type, s32 x, s32 y, s32 z) {
  return ((struct Actor *)Import_ActorCreate(type, x, y, z));
}

static __inline__ void Actor_SetPositionAndResetMotion_(struct Actor *object,
                                                        s32 x, s32 y, s32 z) {
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

static __inline__ void Func_020053d0_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH(wrapper): the call through this inline, whose parameters give
   * each stack argument its own register, as in The Broken Seal's scene layer;
   * as a direct call to TileCopies_CopyTouchedCells, which builds them all in
   * r3, 8 of 15 instructions differ in tla-en
   * (KandoreanTemple_CycloneFirstPatch). */
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

static __inline__ void Func_020053a8_(s32 a, s32 b, s32 c, s32 d) {
  Rope_Place(a, b, c, d);
}
#include "field/field.h"
#include "types.h"
#include "scene_tables.h"
#include "module/scene/scene.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/event_inline.h"
#include "macros.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/io_reg.h"
#include "version.h"
#include "system/task.h"
#include "shared/tables.h"
#include "system/field_effect.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "objcmd.h"

struct MapWork17 {
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  u16 flags;
  u8 wideTiles;
  u8 bits0 : 2;
  u8 mode : 2;
};

extern u32 gKandoreanTemple_Room5Regions[];
extern struct ActorPlacement gKandoreanTemple_GatePlacements[];
extern struct ActorPlacement gKandoreanTemple_PoisHallPlacements[];
extern s16 gKandoreanTemple_Room2Events[];
void KandoreanTemple_ArriveOnEastLift(void);
void KandoreanTemple_LowerTheLadder(void);

void KandoreanTemple_StartLeftVent(void);

void KandoreanTemple_TestTheStudents(void);
void KandoreanTemple_PraiseTheLevitator(void);
void KandoreanTemple_FindTheRopeOnTheFloor(void);
void KandoreanTemple_MeetPoi(void);
void KandoreanTemple_CreateLashPebble(void);
extern u32 gKandoreanTemple_PoiWalksOutScript[];
extern u32 gKandoreanTemple_StudentWalksAsideScript[];
extern u32 gKandoreanTemple_StudentWalksAwayScript[];

void KandoreanTemple_CastLash(void);
void KandoreanTemple_KradenSuspectsTheWall(void);

void KandoreanTemple_PushThroughTheSteam(void);

void KandoreanTemple_ReadPoisSign(void);

void KandoreanTemple_TryTheLockedDoor(void);

void KandoreanTemple_AskWhoPassedTheTest(void);

void KandoreanTemple_ReadTheGateStudent(void);

void KandoreanTemple_ReadTheGuard(void);
void KandoreanTemple_ClimbDownTheWell(void);

void KandoreanTemple_DropThroughTheTrapdoor(void);

void KandoreanTemple_SlipIntoTheCrack(void);
void KandoreanTemple_StartMovingBlock(void);
void KandoreanTemple_FinishMovingBlock(void);
void KandoreanTemple_FinishMovingPillar(void);
void KandoreanTemple_StartMovingPot(void);
void KandoreanTemple_FinishMovingPot(void);

void KandoreanTemple_TouchTheHotFloor(void);

void KandoreanTemple_OpenTheWellPassage(void);
void KandoreanTemple_FindTheHiddenPassage(void);
void KandoreanTemple_StepBehindThePole(void);
void KandoreanTemple_StepOutFromThePole(void);
void KandoreanTemple_FightChestbeaters(void);
void KandoreanTemple_CycloneFirstPatch(void);
void KandoreanTemple_CycloneSecondPatch(void);
void KandoreanTemple_CycloneThirdPatch(void);
void KandoreanTemple_CycloneFourthPatch(void);

void KandoreanTemple_CycloneFifthPatch(void);

extern u32 gFieldEffect_ShortScript[];

extern u32 gFieldEffect_MediumScript[];

extern s32 gKandoreanTemple_LevitateHighScript[];
extern s32 gKandoreanTemple_LevitateLowScript[];
extern s32 gKandoreanTemple_StandStillScript[];

void KandoreanTemple_ArriveOnWestLift(void);

extern s32 gKandoreanTemple_LeftTimer;
extern u16 gKandoreanTemple_LeftBobPhase;
extern u16 gKandoreanTemple_RightBobPhase;
void Tightrope_Initialize(void);
void Actor_PlaceBlocks(const void *data);

#include "text/msg_ids.h"

extern s32 gKandoreanTemple_RightTimer;

void KandoreanTemple_FightFog(void) { Field_RequestDjinnBattle(11, 27); }

u32 KandoreanTemple_CountDownSteam(struct Actor *object) {
  s32 left;

  left = --*(s16 *)&object->unk66;
  return (u32)(-left | left) >> 31;
}

s32 KandoreanTemple_MeditateRestlessly(struct Actor *object) {
  if (object->timer != 0) {
    object->timer--;
  } else {
    Actor_ShowEmoteById((s16)object->unk66, 0x105, 0);
    object->timer = ((Random16() * 240) >> 16) + 240;
  }
  return 1;
}

u32 __umodsi3(u32 a, u32 b);

s32 KandoreanTemple_LevitateStudentBob(struct Actor *object) {
  switch (object->timer) {
  case 6:
    object->scaleX -= 0x4000;
    {
      s32 scale = object->scaleY;

      object->scaleY = scale + 0x2000;
    }
    break;
  case 4:
    object->scaleX += 0x2000;
    object->scaleY -= 0x1000;
    break;
  case 2:
    object->scaleX += 0x1000;
    object->scaleY -= 0x800;
    break;
  case 0:
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    object->timer = Random16() % 90 + 60;
    break;
  }
  object->timer--;
  return 1;
}

s32 KandoreanTemple_PlaceLevitatingStudent(struct Actor *object) {
  object->motionFlags = 0;
  object->scaleX = 0xcccc;
  object->scaleY = 0xcccc;
  object->collisionFlags |= 8;
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern u32 gKandoreanTemple_ChestbeaterRoomRegions[];

u32 *Scene_GetRegions(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov653Room3)
    return gKandoreanTemple_ChestbeaterRoomRegions;
  if (scene == (s32)&SceneId_Ov653Room5)
    return gKandoreanTemple_Room5Regions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gKandoreanTemple_Placements[];
extern struct ActorPlacement gKandoreanTemple_LiftRoomPlacements[];
extern struct ActorPlacement gKandoreanTemple_ChestbeaterRoomPlacements[];
extern struct ActorPlacement gKandoreanTemple_VentRoomPlacements[];
extern struct ActorPlacement gKandoreanTemple_FogRoomPlacements[];

void *Scene_GetPlacements(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov653Room1)
    return gKandoreanTemple_LiftRoomPlacements;
  if (scene == (s32)&SceneId_Ov653Room3)
    return gKandoreanTemple_ChestbeaterRoomPlacements;
  if (scene == (s32)&SceneId_Ov653Room4)
    return gKandoreanTemple_VentRoomPlacements;
  if (scene == (s32)&SceneId_Ov653Room7)
    return gKandoreanTemple_FogRoomPlacements;
  if (scene == (s32)&SceneId_Ov653Room8) {
    if (GameFlag_Get(0x849)) {
      gKandoreanTemple_GatePlacements[3].talkFacing = 0;
      gKandoreanTemple_GatePlacements[4].talkFacing = 0;
      gKandoreanTemple_GatePlacements[5].talkFacing = 0;
    }
    return gKandoreanTemple_GatePlacements;
  }
  if (scene == (s32)&SceneId_Ov653Room9) {
    if (GameFlag_Get(0x849)) {
      gKandoreanTemple_PoisHallPlacements[5].talkFacing = 3;
      gKandoreanTemple_PoisHallPlacements[6].x = 0x1700000;
      gKandoreanTemple_PoisHallPlacements[6].z = 0x2b60000;
      gKandoreanTemple_PoisHallPlacements[7].talkFacing = 1;
    }
    return gKandoreanTemple_PoisHallPlacements;
  }
  return gKandoreanTemple_Placements;
}

void KandoreanTemple_KradenSuspectsTheWall(void) {
  struct Actor *object;

  if (GameFlag_Get(0x842) == 0) {
    GameFlag_Set(0x230);
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_WalkToAndWaitById(4, 104, 252);
    Actor_FaceDirection(4, FACING_NORTHWEST, 0);
    FieldActor_LaunchFromLeader(14, -16, -16, FACING_NORTH);
    Actor_WaitForMoveById(14);
    Event_Wait(20);
    Event_SetMessageId((s32)&MsgKandoreanTemple11);
    Event_ShowMessageAndWait(14, 0);
    Actor_FaceDirectionById(14, FACING_SOUTHEAST);
    Event_Wait(30);
    Actor_SetModeById(14, 3);
    Motion_SetModeAndWaitAnimation(4, 3);
    Event_Wait(20);
    Actor_SetSpeed(14, 0x13333, 0x9999);
    Actor_SetModeById(14, 2);
    object = Actor_GetById(4);
    if (object)
      Actor_SetDestinationById(14, ((s16 *)&object->x)[1],
                               ((s16 *)&object->z)[1]);
    Actor_WaitForMoveById(14);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
    Event_End();
  }
}

extern struct Trigger gKandoreanTemple_Events[];
extern struct Trigger gKandoreanTemple_LiftRoomEvents[];
extern struct Trigger gKandoreanTemple_ChestbeaterRoomEvents[];
extern struct Trigger gKandoreanTemple_VentRoomEvents[];
extern struct Trigger gKandoreanTemple_Room5Events[];
extern struct Trigger gKandoreanTemple_Room6Events[];
extern struct Trigger gKandoreanTemple_FogRoomEvents[];
extern struct Trigger gKandoreanTemple_GateEvents[];
extern struct Trigger gKandoreanTemple_PoisHallEvents[];

void *Scene_GetEvents(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov653Room1)
    return gKandoreanTemple_LiftRoomEvents;
  if (scene == (s32)&SceneId_Ov653Room2)
    return gKandoreanTemple_Room2Events;
  if (scene == (s32)&SceneId_Ov653Room3)
    return gKandoreanTemple_ChestbeaterRoomEvents;
  if (scene == (s32)&SceneId_Ov653Room4)
    return gKandoreanTemple_VentRoomEvents;
  if (scene == (s32)&SceneId_Ov653Room5)
    return gKandoreanTemple_Room5Events;
  if (scene == (s32)&SceneId_Ov653Room6)
    return gKandoreanTemple_Room6Events;
  if (scene == (s32)&SceneId_Ov653Room7)
    return gKandoreanTemple_FogRoomEvents;
  if (scene == (s32)&SceneId_Ov653Room8)
    return gKandoreanTemple_GateEvents;
  if (scene == (s32)&SceneId_Ov653Room9)
    return gKandoreanTemple_PoisHallEvents;
  return gKandoreanTemple_Events;
}

void KandoreanTemple_StartSteamTrail(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  KandoreanTemple_TrailSteam(leader, 0);
}

void KandoreanTemple_TrailSteam(struct Actor *object, s32 step) {
  s32 speed = (object->velocityX + object->velocityZ) >> 1;
  s32 *counter = &gKandoreanTemple_SteamCounter;

  if (speed < 0)
    speed = -speed;
  if ((u32)speed > 0xbb80)
    *counter += 4;
  else if (speed != 0)
    *counter += 2;
  gKandoreanTemple_SteamCounter += step;
  if (gKandoreanTemple_SteamCounter > 40) {
    KandoreanTemple_PuffSteam(object);
    gKandoreanTemple_SteamCounter = 0;
  }
}

void KandoreanTemple_PuffSteam(struct Actor *object) {
  struct Actor *steam = Func_02005140_(0x10d, object->x, object->y, object->z);
  struct FieldSprite *sprite;

  if (steam) {
    sprite = steam->sprite;
    Actor_StartScript(steam, gKandoreanTemple_PuffScript);
    steam->motionFlags = 0;
    steam->terrainId = 1;
    steam->priorityFlags = 2;
    if (sprite) {
      Animation_ApplyChildArgument(sprite, 2);
      sprite->blendMode = 1;
      sprite->nextFrame = 0;
      sprite->priority = 3;
    }
  }
}

void KandoreanTemple_PushThroughTheSteam(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *steam;

  if ((gFrameCount & 7) == 0) {
    steam = ((struct Actor *)Import_ActorCreate(
        14, leader->x, leader->y + 0x60000, leader->z));
    if (steam) {
      Actor_SetSpriteFlags(steam, 0);
      Actor_SelectAnimation(steam, 1);
      Actor_StartScript(steam, gKandoreanTemple_StreamPuffScript);
    }
  }
  KandoreanTemple_PushNorth(leader, 0xd999);
}

/* VIEW(kandorean_temple): tag struct EventWork1cc as struct
 * EventWork1cc__kandorean_temple; module/tla.h has another definition; tried
 * with the definition the rest of the object has, its members read focus as
 * object, it does not compile in tla-ja:
 * games/tla/src/scene/kandorean_temple.c:513: dereferencing pointer to
 * incomplete type. */
struct EventWork1cc__kandorean_temple {
  u8 unk000[0x1cc];
  struct Actor *focus;
};

void KandoreanTemple_PushNorth(struct Actor *object, s32 dz) {
  struct Actor *focus =
      ((struct EventWork1cc__kandorean_temple *)Ram_HeapSlots->eventWork)
          ->focus;
  s32 position[3];

  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z + dz;
  MapGrid_CheckCollision(object, position);
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z + 0x80000;
  if (MapGrid_CheckCollision(object, position) > 0)
    return;
  position[0] = object->x + 0x5b333;
  position[1] = object->y;
  position[2] = object->z + 0x5b333;
  if (MapGrid_CheckCollision(object, position) > 0)
    return;
  position[0] = object->x - 0x5b333;
  position[1] = object->y;
  position[2] = object->z + 0x5b333;
  if (MapGrid_CheckCollision(object, position) > 0)
    return;
  focus->z += dz;
  object->z += dz;
}

s32 KandoreanTemple_IsOverActor(struct Actor *a, struct Actor *b) {
  s32 left = b->x - 0xc0000;
  s32 right = b->x + 0xc0000;

  if (a->x > left && a->x < right && a->z > b->z - 0x80000 &&
      a->z < b->z + 0x80000)
    return 1;
  return 0;
}

void KandoreanTemple_UpdateVentSteam(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *other = Actor_GetById(8);
  s32 vx = object->bounce;
  s32 vz;
  struct FieldSprite *sprite;
  s32 rate;
  s32 scale;

  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  rate = object->speedLimit;
  scale = object->scaleX;
  sprite = object->sprite;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  sprite->rotation += object->timer;
  if (leader->y == 0) {
    if (KandoreanTemple_IsOverActor(object, leader)) {
      object->z -= object->unk4c;
      object->gravity += object->unk4c >> 2;
      object->unk4c = 0;
    }
    if (leader->y == 0 && KandoreanTemple_IsOverActor(object, other)) {
      object->z -= object->unk4c;
      object->gravity += object->unk4c >> 2;
      object->unk4c = 0;
    }
  }
}

void KandoreanTemple_InitializeSteam(struct Actor *object, s32 priority) {
  struct FieldSprite *sprite;

  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->priorityFlags = 2;
  sprite = object->sprite;
  sprite->priority = priority;
  Actor_SetSpriteFlags(object, 0);
  Actor_SelectAnimation(object, 2);
  Actor_StartScript(object, gKandoreanTemple_SteamScript);
  Actor_SetSpriteColor(object, 9);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void KandoreanTemple_BlowSteam(s32 *position, s32 value) {
  struct Actor *object =
      Func_02005140_(0x11d, position[0], position[1], position[2] - 0x20000);

  if (object) {
    s32 timer;

    object->bounce = Trig_Cos(((Random16() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16();
    object->unk4c = (u16)(Random16() * 2) + value;
    timer = (u16)Random16() - 0x8000;
    object->timer = timer;
    KandoreanTemple_InitializeSteam(object, 3);
    object->update = (void (*)(void))KandoreanTemple_UpdateVentSteam;
    Actor_SetBlendMode(object, 1);
  }
}

void KandoreanTemple_UpdateStreamSteam(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *other = Actor_GetById(12);
  s32 vx = object->bounce;
  s32 vz;
  struct FieldSprite *sprite;
  s32 rate;
  s32 scale;

  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  /* FAKEMATCH(dead): an empty once-only block, so the Z speed is stored before
   * the scale rate loads, as in KandoreanTemple_UpdateVentSteam; without it, 2
   * of 97 instructions differ in tla-en. */
  do {
  } while (0);
  rate = object->speedLimit;
  scale = object->scaleX;
  sprite = object->sprite;
  object->scaleX = scale + rate;
  {
    /* FAKEMATCH(pin): the call's first argument pinned to r0, so it is copied
     * before the Y scale store; as a plain local, copied after the action's
     * halfword load, 4 of 97 instructions differ in tla-en. */
    register struct Actor *first asm("r0") = object;

    object->scaleY += object->acceleration;
    /* FAKEMATCH(dead): an empty once-only block, so the Y scale is stored
     * before the action is addressed, as in KandoreanTemple_UpdateVentSteam;
     * without it, 2 of 97 instructions differ in tla-en. */
    do {
    } while (0);
    sprite->rotation += object->timer;
    if (KandoreanTemple_IsOverActor(first, leader)) {
      ((struct FieldSprite *)object->sprite)->priority = 3;
      object->z -= object->unk4c;
      object->gravity += object->unk4c >> 2;
      object->unk4c = 0;
    }
  }
  if (KandoreanTemple_IsOverActor(object, other)) {
    ((struct FieldSprite *)object->sprite)->priority = 3;
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 3;
    object->unk4c = 0;
  }
}

void KandoreanTemple_BlowStreamSteam(void) {
  struct Actor *object = ((struct Actor *)Import_ActorCreate(
      0x11d, 0x2b80000, 0x1c0000, 0x9c0000));

  if (object) {
    s32 timer;

    object->bounce = Trig_Cos(((Random16() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16();
    object->unk4c = (u16)(Random16() * 2) + 0x58000;
    timer = (u16)Random16() - 0x8000;
    object->timer = timer;
    KandoreanTemple_InitializeSteam(object, 1);
    object->update = (void (*)(void))KandoreanTemple_UpdateStreamSteam;
    Actor_SetBlendMode(object, 1);
  }
}

void KandoreanTemple_ShowLeader(void) {
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
}

void KandoreanTemple_PrepareLift(s32 id) {
  struct Actor *object = Actor_GetById(id);

  *((u8 *)object + 98) = 1;
  object->priorityFlags |= 2;
  Actor_SetModeById(id, 1);
  Actor_SetSpritePriority(id, 3);
  Actor_DisableById(id);
}

void KandoreanTemple_PrepareLiftBase(s32 id) {
  *((u8 *)Actor_GetById(id) + 98) = 1;
  Actor_SetSpritePriority(id, 2);
  Actor_DisableById(id);
}

void KandoreanTemple_CastLash(void) {
  BattleEffect_LoadActionEffectResources(133, 1);
  FieldPsynergy_SetupActorPair(12, 10);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  BattleEffect_CleanupSceneObjects();
}

#define DEFINE_KANDOREAN_TEMPLE_VENT(name, data1, n1, n2, n3, n4)              \
  void name(void) {                                                            \
    s32 position[3];                                                           \
                                                                               \
    if (data1 != 0 && gFrameCount % n1 == 0) {                                 \
      SET_VECTOR(position, n2, 0, n3);                                         \
      KandoreanTemple_BlowSteam(position, n4);                                 \
    }                                                                          \
  }

DEFINE_KANDOREAN_TEMPLE_VENT(KandoreanTemple_BlowLeftVent,
                             gKandoreanTemple_LeftVentBlowing, 9, 0x1280000,
                             0xb00000, 0x20000)

DEFINE_KANDOREAN_TEMPLE_VENT(KandoreanTemple_BlowRightVent,
                             gKandoreanTemple_RightVentBlowing, 5, 0x1580000,
                             0xa00000, 0x40000)

void KandoreanTemple_BlowStream(void) {
  if (gFrameCount % 5 == 0)
    KandoreanTemple_BlowStreamSteam();
}

void KandoreanTemple_LowerLeftColumn(s32 id) {
  Actor_GetById(id)->scaleY = 0x800;
  Actor_SetPositionAndResetMotion_(Actor_GetById(id), PIXELS(216), 0,
                                   PIXELS(216));
}

void KandoreanTemple_LowerRightColumn(s32 id) {
  Actor_GetById(id)->scaleY = 0x800;
  Actor_SetPositionAndResetMotion_(Actor_GetById(id), PIXELS(376), 0,
                                   PIXELS(248));
}

void KandoreanTemple_GrowColumn(s32 id, s32 dy, s32 scale) {
  struct Actor *object = Actor_GetById(id);

  object->scaleY += scale;
  object->y += dy;
  object->z -= dy;
}

void KandoreanTemple_ShrinkColumn(s32 id, s32 dy, s32 scale) {
  struct Actor *object = Actor_GetById(id);

  object->scaleY -= scale;
  object->y -= dy;
  object->z += dy;
}

s32 KandoreanTemple_IsLeftVentBlocked(void) {
  if (GameFlag_Get(0x20a) || GameFlag_Get(0x300))
    return 1;
  return 0;
}

s32 KandoreanTemple_IsRightVentBlocked(void) {
  if (GameFlag_Get(0x20a) || GameFlag_Get(0x301))
    return 1;
  return 0;
}

void KandoreanTemple_StartLeftVent(void) {
  Actor_SetModeById(11, 2);
  Actor_SetModeById(13, 3);
  Actor_SetModeById(14, 3);
  Actor_SetModeById(15, 3);
  Actor_SetModeById(16, 3);
  Actor_SetModeById(9, 0);
  Actor_SetSpritePriority(9, 2);
  Actor_GetById(9)->priorityFlags &= ~2;
  Actor_SetPositionAndResetMotion_(Actor_GetById(9), PIXELS(216), PIXELS(8),
                                   PIXELS(216));
  if (GameFlag_Get(0x208)) {
    Actor_SetModeById(9, 4);
    Actor_SetPositionAndResetMotion_(
        Actor_GetById(gMainRoutineWork.currentOwner), PIXELS(216), PIXELS(12),
        PIXELS(216));
    Actor_SetSingleChildFlags_(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  }
  Actor_GetById(11)->scaleY = 0x10000;
  Actor_SetPositionAndResetMotion_(Actor_GetById(11), PIXELS(216), 0,
                                   PIXELS(216));
  KandoreanTemple_LowerLeftColumn(13);
  KandoreanTemple_LowerLeftColumn(14);
  if (GameFlag_Get(0x301)) {
    KandoreanTemple_LowerLeftColumn(15);
    KandoreanTemple_LowerLeftColumn(16);
  }
  gKandoreanTemple_LeftVentBlowing = 1;
  Event_SetFlag(0x20c);
}

void KandoreanTemple_StartRightVent(void) {
  Actor_SetModeById(12, 2);
  Actor_SetModeById(17, 3);
  Actor_SetModeById(18, 3);
  Actor_SetModeById(19, 3);
  Actor_SetModeById(20, 3);
  Actor_SetModeById(10, 0);
  Actor_SetSpritePriority(10, 2);
  Actor_GetById(10)->priorityFlags &= ~2;
  Actor_SetPositionAndResetMotion_(Actor_GetById(10), PIXELS(376), PIXELS(8),
                                   PIXELS(248));
  if (GameFlag_Get(0x209)) {
    Actor_SetModeById(10, 4);
    Actor_SetPositionAndResetMotion_(
        Actor_GetById(gMainRoutineWork.currentOwner), PIXELS(376), PIXELS(12),
        PIXELS(248));
    Actor_SetSingleChildFlags_(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  }
  Actor_GetById(12)->scaleY = 0x10000;
  Actor_SetPositionAndResetMotion_(Actor_GetById(12), PIXELS(376), 0,
                                   PIXELS(248));
  KandoreanTemple_LowerRightColumn(17);
  KandoreanTemple_LowerRightColumn(18);
  if (GameFlag_Get(0x300)) {
    KandoreanTemple_LowerRightColumn(19);
    KandoreanTemple_LowerRightColumn(20);
  }
  gKandoreanTemple_RightVentBlowing = 1;
  Event_SetFlag(0x20d);
}

void KandoreanTemple_RunLeftVent(void) {
  s32 *timer = &gKandoreanTemple_LeftTimer;
  struct Actor *statue;
  struct Actor *leader;
  u16 *phase;
  s32 limit;
  s32 timerEnd;
  s32 t;
  s32 flag;

  if (*timer < 300) {
    if (KandoreanTemple_IsLeftVentBlocked() != 0)
      return;
    goto count;
  }
  if (*timer == 300) {
    MapGrid_CopyCellAttributes(15, 15, 1, 1, 13, 13);
    MapGrid_CopyCellAttributes(65, 15, 1, 1, 63, 13);
    KandoreanTemple_StartLeftVent();
    goto count;
  }
  timerEnd = 2999;
  if (*timer <= timerEnd) {
    statue = Actor_GetById(9);
    flag = GameFlag_Get(0x301);
    t = *timer;
    if (flag) {
      if (t <= 316) {
        statue->y += 0x48000;
        limit = 0x1000;
        KandoreanTemple_GrowColumn(11, 0x20000, limit);
        KandoreanTemple_GrowColumn(13, 0x18000, limit);
        KandoreanTemple_GrowColumn(14, 0x10000, limit);
        KandoreanTemple_GrowColumn(15, 0x8000, limit);
        KandoreanTemple_GrowColumn(16, 0, limit);
      } else if (t < 600) {
        if (GameFlag_Get(0x208) && *timer == 400) {
          MapGrid_CopyCellAttributes(22, 0, 1, 1, 13, 13);
          Task_Remove(KandoreanTemple_RunLeftVent);
        }
        phase = &gKandoreanTemple_LeftBobPhase;
        statue->y += Trig_Sin(*phase) >> 1;
        *phase += 0x1000;
      } else if (t <= 999) {
        phase = &gKandoreanTemple_LeftBobPhase;
        if (*phase == 0) {
          gKandoreanTemple_LeftVentBlowing = 0;
          *timer = 999;
          GameFlag_Clear(0x20c);
        }
        statue->y += Trig_Sin(*phase) >> 1;
        *phase += 0x1000;
      } else {
        statue->y += -0x6c000;
        limit = 0x1800;
        KandoreanTemple_ShrinkColumn(11, 0x30000, limit);
        KandoreanTemple_ShrinkColumn(13, 0x24000, limit);
        KandoreanTemple_ShrinkColumn(14, 0x18000, limit);
        KandoreanTemple_ShrinkColumn(15, 0xc000, limit);
        KandoreanTemple_ShrinkColumn(16, 0, limit);
        if (Actor_GetById(16)->scaleY <= limit)
          *timer = timerEnd;
      }
    } else {
      if (t <= 316) {
        statue->y += 0x27999;
        limit = 0x1000;
        KandoreanTemple_GrowColumn(11, 0x10000, limit);
        KandoreanTemple_GrowColumn(13, 0x8000, limit);
        KandoreanTemple_GrowColumn(14, 0, limit);
      } else if (t < 600) {
        phase = &gKandoreanTemple_LeftBobPhase;
        statue->y += Trig_Sin(*phase) >> 1;
        *phase += 0x1000;
      } else if (t <= 999) {
        phase = &gKandoreanTemple_LeftBobPhase;
        if (*phase == 0) {
          gKandoreanTemple_LeftVentBlowing = 0;
          *timer = 999;
          GameFlag_Clear(0x20c);
        }
        statue->y += Trig_Sin(*phase) >> 1;
        *phase += 0x1000;
      } else {
        statue->y += -0x3b666;
        limit = 0x1800;
        KandoreanTemple_ShrinkColumn(11, 0x18000, limit);
        KandoreanTemple_ShrinkColumn(13, 0xc000, limit);
        KandoreanTemple_ShrinkColumn(14, 0, limit);
        if (Actor_GetById(14)->scaleY <= limit)
          *timer = timerEnd;
      }
    }
    if (GameFlag_Get(0x208) == 0)
      goto count;
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    leader->y = statue->y + 0x40000;
    leader->targetY = 0x80000000;
    leader->velocityY = 0;
    goto count;
  }
  if (*timer == 3000) {
    MapGrid_CopyCellAttributes(8, 0, 1, 1, 13, 13);
    MapGrid_CopyCellAttributes(58, 0, 1, 1, 63, 13);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
    Actor_SetModeById(9, 1);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(9), 0xd80000, 0,
                                         0xd80000);
    Actor_GetById(9)->priorityFlags |= 2;
    Actor_SetSpritePriority(9, 3);
    if (GameFlag_Get(0x208)) {
      leader = Actor_GetById(gMainRoutineWork.currentOwner);
      Actor_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
      Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
    }
    goto count;
  }
  {
    s32 none = KandoreanTemple_IsLeftVentBlocked();

    if (none != 0)
      return;
    *timer += (Random16() << 7) >> 16;
    if ((u32)*timer > ((Random16() * 10000) >> 16) + 10000)
      *timer = none;
  }
count:
  gKandoreanTemple_LeftTimer++;
}

// Bob the statue on a sine.
#define BOB(statue, phase)                                                     \
  do {                                                                         \
    (statue)->y += Trig_Sin(*(phase)) >> 1;                                    \
    *(phase) += 0x1000;                                                        \
  } while (0)

void KandoreanTemple_RunRightVent(void) {
  s32 *timer = &gKandoreanTemple_RightTimer;
  struct Actor *statue;
  struct Actor *leader;
  u16 *phase;
  s32 limit;
  s32 raised;
  s32 last;
  s32 t;

  t = *timer;
  if (t < 300) {
    if (KandoreanTemple_IsRightVentBlocked() != 0)
      return;
  } else if (t == 300) {
    MapGrid_CopyCellAttributes(15, 15, 1, 1, 23, 15);
    MapGrid_CopyCellAttributes(65, 15, 1, 1, 73, 15);
    KandoreanTemple_StartRightVent();
  } else {
    last = 2999;
    if (t <= last) {
      statue = Actor_GetById(10);
      raised = GameFlag_Get(0x300);
      t = *timer;
      if (raised) {
        if (t <= 316) {
          statue->y += 0x48000;
          limit = 0x1000;
          KandoreanTemple_GrowColumn(12, 0x20000, limit);
          KandoreanTemple_GrowColumn(17, 0x18000, limit);
          KandoreanTemple_GrowColumn(18, 0x10000, limit);
          KandoreanTemple_GrowColumn(19, 0x8000, limit);
          KandoreanTemple_GrowColumn(20, 0, limit);
        } else if (t < 600) {
          if (GameFlag_Get(0x209) && *timer == 400) {
            MapGrid_CopyCellAttributes(23, 0, 1, 1, 23, 15);
            Task_Remove(KandoreanTemple_RunRightVent);
          }
          phase = &gKandoreanTemple_RightBobPhase;
          BOB(statue, phase);
        } else if (t <= 999) {
          phase = &gKandoreanTemple_RightBobPhase;
          if (*phase == 0) {
            gKandoreanTemple_RightVentBlowing = 0;
            *timer = 999;
            GameFlag_Clear(0x20d);
          }
          BOB(statue, phase);
        } else {
          statue->y += -0x6c000;
          limit = 0x1800;
          KandoreanTemple_ShrinkColumn(12, 0x30000, limit);
          KandoreanTemple_ShrinkColumn(17, 0x24000, limit);
          KandoreanTemple_ShrinkColumn(18, 0x18000, limit);
          KandoreanTemple_ShrinkColumn(19, 0xc000, limit);
          KandoreanTemple_ShrinkColumn(20, 0, limit);
          if (Actor_GetById(20)->scaleY <= limit)
            *timer = last;
        }
      } else {
        if (t <= 316) {
          statue->y += 0x27999;
          limit = 0x1000;
          KandoreanTemple_GrowColumn(12, 0x10000, limit);
          KandoreanTemple_GrowColumn(17, 0x8000, limit);
          KandoreanTemple_GrowColumn(18, 0, limit);
        } else if (t < 600) {
          phase = &gKandoreanTemple_RightBobPhase;
          BOB(statue, phase);
        } else if (t <= 999) {
          phase = &gKandoreanTemple_RightBobPhase;
          if (*phase == 0) {
            gKandoreanTemple_RightVentBlowing = 0;
            *timer = 999;
            GameFlag_Clear(0x20d);
          }
          BOB(statue, phase);
        } else {
          statue->y += -0x3b666;
          limit = 0x1800;
          KandoreanTemple_ShrinkColumn(12, 0x18000, limit);
          KandoreanTemple_ShrinkColumn(17, 0xc000, limit);
          KandoreanTemple_ShrinkColumn(18, 0, limit);
          if (Actor_GetById(18)->scaleY <= limit)
            *timer = last;
        }
      }
      if (GameFlag_Get(0x209)) {
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        leader->y = statue->y + 0x40000;
        leader->targetY = 0x80000000;
        leader->velocityY = 0;
      }
    } else if (t == 3000) {
      MapGrid_CopyCellAttributes(8, 0, 1, 1, 23, 15);
      MapGrid_CopyCellAttributes(58, 0, 1, 1, 73, 15);
      Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
      Actor_SetModeById(10, 1);
      FieldActor_SetPositionAndResetMotion(Actor_GetById(10), 0x1780000, 0,
                                           0xf80000);
      Actor_GetById(10)->priorityFlags |= 2;
      Actor_SetSpritePriority(10, 3);
      if (GameFlag_Get(0x209)) {
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        Actor_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
        Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
      }
    } else {
      s32 none = KandoreanTemple_IsRightVentBlocked();

      if (none != 0)
        return;
      *timer += (Random16() << 7) >> 16;
      if ((u32)*timer > ((Random16() * 10000) >> 16) + 10000)
        *timer = none;
    }
  }
  gKandoreanTemple_RightTimer++;
}

void KandoreanTemple_RunWestLift(void) {
  struct Actor *lift = Actor_GetById(13);
  struct Actor *base = Actor_GetById(15);
  s32 *timer = &gKandoreanTemple_LeftTimer;

  if (*timer == 300) {
    Scene_QueueMusic(162);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(13), PIXELS(440),
                                         PIXELS(-20), PIXELS(204));
    Actor_SetPositionAndResetMotion(Actor_GetById(15), PIXELS(440), PIXELS(-20),
                                    PIXELS(200));
    Actor_SetSpritePriority(13, 3);
    Actor_SetSpritePriority(15, 3);
    base->priorityFlags |= 2;
    base->scaleY = 0x400;
    Actor_SetModeById(13, 0);
    Actor_SetModeById(15, 2);
  } else if ((u32)(*timer - 301) <= 1698) {
    lift->y += PIXELS(4);
    KandoreanTemple_GrowColumn(15, 0x4ccc, 0x4ccc);
    // y read as long, another alias set than the reads around it: GCSE then
    // keeps the reload after this call; as lift->y, 2 instructions move.
    if (*(long *)&lift->y > -0x50000)
      Actor_SetSpritePriority(13, 2);
    if (lift->y >= 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 27, 12);
      *timer = 2000;
    }
  } else if (*timer < 2800) {
    u16 *phase = &gKandoreanTemple_LeftBobPhase;

    lift->y += Trig_Sin(*phase) >> 1;
    *phase += 0x1000;
    if (*timer == 2280)
      MapGrid_CopyCellAttributes(27, 10, 1, 1, 27, 12);
    if (*timer > 2300 && *phase == 0) {
      if (GameFlag_Get(0x208)) {
        MapGrid_CopyCellAttributes(21, 0, 1, 1, 27, 12);
        Task_Remove(KandoreanTemple_RunWestLift);
      } else {
        *timer = 2800;
      }
    }
  } else {
    lift->y += PIXELS(-4);
    KandoreanTemple_ShrinkColumn(15, 0x4ccc, 0x4ccc);
    // y read as long, as above.
    if (*(long *)&lift->y < -0x50000)
      Actor_SetSpritePriority(13, 3);
    if (lift->y <= -0x140000) {

      /* FAKEMATCH(temporary): the timer pointer's own address taken, which
       * orders the stack arguments of the 2280 call as the native does; as
       * *timer = 0, 6 of 193 instructions differ in tla-en. */
      s32 **timerRef = &timer;

      Scene_QueueMusic(245);
      Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
      **timerRef = 0;
    }
  }
  gKandoreanTemple_LeftTimer++;
}

void KandoreanTemple_RunEastLift(void) {
  struct Actor *lift = Actor_GetById(14);
  struct Actor *base = Actor_GetById(16);
  s32 *timer = &gKandoreanTemple_RightTimer;

  if (*timer == 300) {
    Scene_QueueMusic(162);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(14), PIXELS(600),
                                         PIXELS(-20), PIXELS(236));
    Actor_SetPositionAndResetMotion(Actor_GetById(16), PIXELS(600), PIXELS(-20),
                                    PIXELS(232));
    Actor_SetSpritePriority(14, 3);
    Actor_SetSpritePriority(16, 3);
    base->priorityFlags |= 2;
    base->scaleY = 0x400;
    Actor_SetModeById(14, 0);
    Actor_SetModeById(16, 2);
  } else if ((u32)(*timer - 301) <= 1698) {
    lift->y += PIXELS(4);
    KandoreanTemple_GrowColumn(16, 0x4ccc, 0x4ccc);
    // y read as long, another alias set than the reads around it: GCSE then
    // keeps the reload after this call; as lift->y, 2 instructions move.
    if (*(long *)&lift->y > -0x50000)
      Actor_SetSpritePriority(14, 2);
    if (lift->y >= 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 37, 14);
      *timer = 2000;
    }
  } else if (*timer < 2800) {
    u16 *phase = &gKandoreanTemple_RightBobPhase;

    lift->y += Trig_Sin(*phase) >> 1;
    *phase += 0x1000;
    if (*timer == 2280)
      MapGrid_CopyCellAttributes(37, 12, 1, 1, 37, 14);
    if (*timer > 2300 && *phase == 0) {
      if (GameFlag_Get(0x209)) {
        MapGrid_CopyCellAttributes(22, 0, 1, 1, 37, 14);
        Task_Remove(KandoreanTemple_RunEastLift);
      } else {
        *timer = 2800;
      }
    }
  } else {
    lift->y += PIXELS(-4);
    KandoreanTemple_ShrinkColumn(16, 0x4ccc, 0x4ccc);
    // y read as long, as above.
    if (*(long *)&lift->y < -0x50000)
      Actor_SetSpritePriority(14, 3);
    if (lift->y <= -0x140000) {

      /* FAKEMATCH(temporary): the timer pointer's own address taken, which
       * orders the stack arguments of the 2280 call as the native does; as
       * *timer = 0, 6 of 193 instructions differ in tla-en. */
      s32 **timerRef = &timer;

      Scene_QueueMusic(245);
      Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
      **timerRef = 0;
    }
  }
  gKandoreanTemple_RightTimer++;
}

void KandoreanTemple_WatchVentPlatforms(void) {
  struct Actor *leader;
  s32 cellX, cellZ, onCell;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  cellX = leader->x >> 20;
  cellZ = leader->z >> 20;
  onCell = 0;
  if (cellX == 13 && cellZ == 13) {
    Event_SetFlag(0x208);
    onCell = 1;
  } else {
    Event_ClearFlag(0x208);
  }
  if (cellX == 23 && cellZ == 15) {
    Event_SetFlag(0x209);
    onCell = 1;
  } else {
    Event_ClearFlag(0x209);
  }
  if (onCell)
    Event_SetFlag(0x17d);
  else
    Event_ClearFlag(0x17d);
  if (gKandoreanTemple_LeftVentBlowing != 0 && leader->y == 0 &&
      leader->x > 0x11c0000 && leader->x < 0x1340000 && leader->z < 0xc80000 &&
      !Event_GetFlag(0x20b))
    KandoreanTemple_PushNorth(leader, 0x26666);
  if (gKandoreanTemple_RightVentBlowing != 0 && leader->y == 0 &&
      leader->x > 0x14a0000 && leader->x <= 0x165ffff && leader->z < 0xd80000 &&
      !Event_GetFlag(0x20b))
    KandoreanTemple_PushNorth(leader, 0x39999);
  if (Event_GetFlag(0x20e)) {
    if (!Event_GetFlag(0x20c) && !Event_GetFlag(0x20d)) {
      Scene_QueueMusic(-1);
      Event_ClearFlag(0x20e);
    }
  } else if (Event_GetFlag(0x20c) || Event_GetFlag(0x20d)) {
    Scene_QueueMusic(162);
    Event_SetFlag(0x20e);
  }
}

void KandoreanTemple_WatchLifts(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x;
  s32 cellX = x >> 20;
  s32 z = leader->z;
  s32 cellZ = z >> 20;
  s32 riding = 0;

  if (leader->y > 0x100000)
    return;
  if ((u32)(x - 0x1900000) > 0xf00000)
    return;
  if ((u32)(z - 0xb40000) > 0x640000)
    return;
  if (cellX == 27 && cellZ == 12) {
    struct Actor *lift = Actor_GetById(13);
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
    Actor_SetModeById(13, 4);
    leader->motionFlags = riding;
    leader->y = lift->y;
    GameFlag_Set(0x208);
    riding = 1;
  }
  if (cellX == 37 && cellZ == 14) {
    struct Actor *lift = Actor_GetById(14);
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
    Actor_SetModeById(14, 4);
    leader->motionFlags = 0;
    leader->y = lift->y;
    GameFlag_Set(0x209);
    riding = 1;
  }
  if (riding) {
    GameFlag_Set(0x17d);
    return;
  }
  Actor_SetModeById(13, 0);
  Actor_SetModeById(14, 0);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  leader->motionFlags = 3;
  GameFlag_Clear(0x208);
  GameFlag_Clear(0x209);
  GameFlag_Clear(0x17d);
}

/* VIEW(kandorean_temple): tag struct MapCell as struct
 * MapCell__kandorean_temple; map.h has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/kandorean_temple.c:1350: arithmetic on pointer to an
 * incomplete type. */
struct MapCell__kandorean_temple {
  u8 unk0[2];
  u8 unk2;
  u8 unk3;
};

struct MapWork170 {
  u8 unk000[0x170];
  struct MapCell__kandorean_temple *cells;
};

void KandoreanTemple_MatchLiftPriority(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 priority = ((struct FieldSprite *)leader->sprite)->priority;

  Actor_SetSpritePriority(13, priority);
  Actor_SetSpritePriority(14, priority);
  Actor_SetSpritePriority(15, priority);
}

void KandoreanTemple_DropOffLowCells(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MapWork170 *map = (struct MapWork170 *)Ram_HeapSlots->mapWork;

  struct MapCell__kandorean_temple *cell = &map->cells[Actor_GetCellIndex()];

  if (cell->unk2 != 0) {
    leader->y = 0;
    leader->terrainHeight = 0;
  }
}

void KandoreanTemple_ReadPoisSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)&MsgKandoreanTemple10, 1);
  Event_End();
}

void KandoreanTemple_TryTheLockedDoor(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)&MsgKandoreanTemple01, 1);
  Event_End();
}

void KandoreanTemple_AskAboutPoi(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(9);
  wait(1);
  Actor_FaceActorById(9, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)&MsgKandoreanTemple02);
  Event_AskYesNoAndShowResponse(9, 0);
  Actor_GetById(9)->facing = FACING_SOUTHEAST + FACING_STEP;
  wait(1);
  Actor_SetMovementTypeById(9, gKandoreanTemple_LookAroundScript);
  wait(1);
  Event_End();
  GameFlag_Set(0x845);
}

void KandoreanTemple_TellOfTheMeditation(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(10, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)&MsgKandoreanTemple03);
  Event_ShowMessageAndWait(10, 0);
  Actor_GetById(10)->facing = FACING_SOUTH + FACING_STEP;
  wait(1);
  Event_End();
  GameFlag_Set(0x846);
}

void KandoreanTemple_AskWhoPassedTheTest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgKandoreanTemple09);
  Event_AskYesNoAndShowResponse(13, 0);
  Event_End();
}

void KandoreanTemple_ReadTheGateStudent(void) {
  Actor_ShowEmoteById(9, 0x100, 0);
  KandoreanTemple_AskAboutPoi();
}

void KandoreanTemple_ReadTheGuard(void) {
  Actor_ShowEmoteById(10, 0x100, 0);
  KandoreanTemple_TellOfTheMeditation();
}

void KandoreanTemple_ClimbDownTheWell(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;
  struct MainRoutineWork *party;
  u8 *flags;
  s32 zero;

  if (z == 5) {
    if (x == 9 && (gInput.held & 16) == 0)
      return;
    if (x == 11 && (gInput.held & 32) == 0)
      return;
  } else if (x == 10 && z == 6 && (gInput.held & 64) == 0) {
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(temporary): the flags address and the zero held in locals, with
   * gMainRoutineWork's address taken in the block below, so the byte store
   * follows the address load; as leader->motionFlags = 0, which stores first,
   * 2 of 92 instructions differ in tla-en. */
  flags = &leader->motionFlags;
  zero = 0;
  party = &gMainRoutineWork;
  *flags = zero;
  Actor_SetSpeedParameters(party->currentOwner, 0x10000, 0x8000);
  Actor_WalkToAndWaitById(party->currentOwner, 168, 80);
  Actor_FaceDirectionById(party->currentOwner, FACING_NORTH);
  Actor_SetSpriteFlags(Actor_GetById(party->currentOwner), 0);
  Actor_SetModeById(party->currentOwner, 13);
  Actor_SetMoveTarget(leader, PIXELS(168), PIXELS(-12), PIXELS(80));
  Actor_WaitForMove(leader);
  Actor_SetModeById(party->currentOwner, 10);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(8);
}

void KandoreanTemple_OpenTheTrapdoor(void) {
  MapGrid_CopyCellPictureIndicesRect(39, 39, 24, 38, 2, 2);
  wait(10);
  MapGrid_CopyCellPictureIndicesRect(39, 41, 24, 38, 2, 2);
  wait(20);
  GameFlag_Set(0x212);
}

void KandoreanTemple_DropThroughTheTrapdoor(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 400, 648);
  Actor_FaceDirectionById(gMainRoutineWork.currentOwner, FACING_NORTH);
  if (GameFlag_Get(0x212) == 0) {
    Sound_Play(188);
    KandoreanTemple_OpenTheTrapdoor();
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Actor_WalkToById(gMainRoutineWork.currentOwner, 400, 640);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(9);
}

void KandoreanTemple_SlipIntoTheCrack(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 416, 288);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  leader->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  for (i = 0; i < 8; i++) {
    leader->z += PIXELS(1);
    wait(1);
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  for (i = 0; i < 8; i++) {
    leader->y -= PIXELS(1);
    wait(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(4);
}

void KandoreanTemple_StartMovingBlock(void) {
  Actor_LiftTargetBlock(gKandoreanTemple_BlockActors);
}

void KandoreanTemple_FinishMovingBlock(void) {
  s32 x = Actor_GetById(12)->x >> 20;

  Actor_PlaceMovedBlock();
  if (x == 43)
    GameFlag_Set(0x211);
  else
    GameFlag_Clear(0x211);
  if (x != 39)
    MapGrid_CopyCellAttributes(39, 0, 1, 1, 39, 10);
}

void KandoreanTemple_StartMovingPillar(void) {
  if ((u32)((Actor_GetById(8)->x >> 20) - 15) > 6)
    GameFlag_Set(0x20a);
  GameFlag_Set(0x20b);
  Actor_LiftTargetBlock(gKandoreanTemple_PillarActors);
}

void KandoreanTemple_FinishMovingPillar(void) {
  struct Actor *object = Actor_GetById(8);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  Actor_PlaceMovedBlock();
  if (x == 13 && z == 13)
    GameFlag_Set(0x300);
  else
    GameFlag_Clear(0x300);
  if (x == 23 && z == 15)
    GameFlag_Set(0x301);
  else
    GameFlag_Clear(0x301);
  GameFlag_Clear(0x20a);
  GameFlag_Clear(0x20b);
}

void KandoreanTemple_StartMovingPot(void) {
  GameFlag_Clear(0x302);
  GameFlag_Clear(0x303);
  Actor_LiftTargetBlock(gKandoreanTemple_PillarActors);
}

void KandoreanTemple_FinishMovingPot(void) {
  s32 x = Actor_GetById(8)->x >> 20;

  Actor_PlaceMovedBlock();
  if (x == 19) {
    GameFlag_Set(0x302);
    GameFlag_Set(0x304);
    Map_CopyCellAttributeRect(0, 0, 3, 3, x, 28);
  } else if (x == 21) {
    GameFlag_Set(0x303);
    GameFlag_Set(0x304);
    MapGrid_CopyCellAttributes(0, 4, 3, 3, 19, 28);
  }
}

void KandoreanTemple_TouchTheHotFloor(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_ShowEmoteById(gMainRoutineWork.currentOwner, 0x100, 20);
  Sound_Play(132);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 10);
  leader->velocityY = PIXELS(6);
  wait(20);
  while (leader->velocityY != 0)
    wait(1);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Event_End();
}

void KandoreanTemple_OpenTheWellPassage(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(188);
  MapGrid_CopyCellPictureIndicesRect(77, 33, 77, 24, 2, 2);
  MapGrid_CopyCellAttributes(32, 30, 2, 1, 17, 25);
  wait(10);
  GameFlag_Set(0x212);
  Event_End();
}

void KandoreanTemple_FindTheHiddenPassage(void) {
  struct Actor *object;

  MapGrid_CopyCellAttributes(0, 0, 1, 1, 5, 13);
  GameFlag_Set(0x842);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(4, 89, 237);
  Actor_FaceDirection(4, FACING_SOUTHEAST, 0);
  Actor_LaunchFromLeader(14, 16, 16, FACING_NORTHWEST);
  Actor_WaitForMoveById(14);
  Event_Wait(20);
  Event_SetMessageId((s32)&MsgKandoreanTemple12);
  Event_ShowMessageAndWait(14, 0);
  Event_Wait(30);
  Actor_SetModeById(14, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Actor_SetSpeed(14, 0x13333, 0x9999);
  Actor_SetModeById(14, 2);
  object = Actor_GetById(4);
  if (object)
    Actor_SetDestinationById(14, *(s16 *)((u8 *)object + 10),
                             *(s16 *)((u8 *)object + 18));
  Actor_WaitForMoveById(14);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Event_End();
}

void KandoreanTemple_ReceiveLash(void) {
  s16 angle = *(s16 *)&Actor_GetById(gMainRoutineWork.currentOwner)->facing;
  struct Actor *object;
  struct FieldSprite *sprite;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  ((void (*)())Actor_ArmCallback)(12, 0);
  Event_SetMessageId((s32)&MsgKandoreanTemple08);
  Event_ShowMessage(0x800c, 0);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 12, 0);
  Actor_SetAnimation(12, 3);
  Event_ShowMessage(0x800c, 0);
  Actor_SetAnimation(12, 3);
  Event_ShowMessage(0x800c, 0);
  Actor_SetAnimation(12, 4);
  Event_ShowMessage(0x800c, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(12, 3);
  Event_ShowMessage(0x800c, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, (u16)angle, 10);
  if (gKandoreanTemple_LashPebble != 0)
    ItemDisplay_ShowPicture(gKandoreanTemple_LashPebble, 3);
  Party_GiveItem(198, 0);
  if (gKandoreanTemple_LashPebble != 0)
    Actor_Release(gKandoreanTemple_LashPebble);
  Actor_SetAnimation(12, 4);
  Event_ShowMessage(0x800c, 0);
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(0x1a80000, 0, 0x1900000, 1);
  object = Actor_GetById(12);
  sprite = object->sprite;
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 424, 376);
  FieldActor_SetSpritePriority(12, 1);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  FieldActor_Jump(12, 4, 0);
  Actor_MoveToAndWait(12, 444, 376);
  Actor_SetAnimation(12, 7);
  object->scaleX = 0xffff0000;
  sprite->rotation = 0xe000;
  object->motionFlags = 0;
  object->y += 0x40000;
  Actor_SetSpriteFlags(Actor_GetById(12), 0);
  Actor_SetSpeed(12, 0x6666, 0x3333);
  FieldActor_SetMoveTarget(object, 0x1ec0000, 0x80000, 0x1780000);
  FieldActor_WaitForMoveById(12);
  object->motionFlags = 3;
  object->terrainHeight = 0;
  object->scaleX = 0x10000;
  sprite->rotation = 0;
  Actor_SetAnimation(12, 1);
  Actor_SetSpriteFlags(Actor_GetById(12), 1);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  FieldActor_Jump(12, 4, 0);
  Actor_MoveToAndWait(12, 504, 376);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 496, 394);
  Actor_WalkToAndWait(12, 432, 394);
  FieldActor_WatchAndSetScript(12, gMainRoutineWork.currentOwner);
  FieldActor_SetSpritePriority(12, 2);
  Event_SetFlag(0x84a);
  Event_End();
}

void KandoreanTemple_StepBehindThePole(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
}

void KandoreanTemple_StepOutFromThePole(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags &= ~32;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
}

void KandoreanTemple_FightChestbeaters(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveWithinMapBounds(PIXELS(328), -1, PIXELS(174), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 328, 174);
  Motion_SetVarCbAndRefresh(11, 2);
  Sound_Play(146);
  Actor_SetModeById(11, 2);
  Actor_GetById(11)->velocityY = PIXELS(4);
  Event_Wait(10);
  Sound_Play(146);
  Actor_SetModeById(12, 2);
  Actor_SetModeById(13, 2);
  Actor_GetById(12)->velocityY = PIXELS(6);
  Actor_GetById(13)->velocityY = PIXELS(6);
  Actor_SetDestinationById(12, 300, 164);
  Actor_MoveToAndWaitById(13, 356, 164);
  Event_Wait(40);
  /* FAKEMATCH(dead): an empty once-only block, whose loop note ends the
   * scheduling block before the battle rule is set, so the last position
   * call's r1 is shifted before r2 is loaded; without it, 2 of 87
   * instructions differ in tla-en. */
  do {
  } while (0);
  gMainRoutineWork.battleRule24b = 2;
  Event_SetAfterBattleDestination((s32)&SceneId_Ov653Room3, 3);
  Field_RequestBattleFromEncounterTable(9, 4);
  Event_End();
}

void KandoreanTemple_CycloneFirstPatch(void) {
  Func_020053d0_(76, 0, 2, 2, 73, 14, 1);
}

void KandoreanTemple_CycloneSecondPatch(void) {
  Func_020053d0_(76, 0, 1, 1, 75, 16, 1);
}

void KandoreanTemple_CycloneThirdPatch(void) {
  Func_020053d0_(76, 0, 1, 1, 74, 18, 1);
}

void KandoreanTemple_CycloneFourthPatch(void) {
  Func_020053d0_(76, 0, 1, 1, 80, 18, 1);
}

void KandoreanTemple_CycloneFifthPatch(void) {
  Func_020053d0_(76, 0, 1, 1, 87, 15, 1);
}

s32 Scene_Initialize(void) {
  s32 scene;

  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x204;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov653Room1)
    KandoreanTemple_StartLiftRoom();
  else if (scene == (s32)&SceneId_Ov653Room2)
    KandoreanTemple_StartRoom2();
  else if (scene == (s32)&SceneId_Ov653Room3)
    KandoreanTemple_StartChestbeaterRoom();
  else if (scene == (s32)&SceneId_Ov653Room4)
    KandoreanTemple_StartVentRoom();
  else if (scene == (s32)&SceneId_Ov653Room5)
    KandoreanTemple_StartRoom5();
  else if (scene == (s32)&SceneId_Ov653Room6)
    KandoreanTemple_StartRoom6();
  else if (scene == (s32)&SceneId_Ov653Room7)
    KandoreanTemple_StartFogRoom();
  else if (scene == (s32)&SceneId_Ov653Room8)
    KandoreanTemple_StartGate();
  else if (scene == (s32)&SceneId_Ov653Room9)
    KandoreanTemple_StartPoisHall();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void KandoreanTemple_StartLiftRoom(void) {
  Tightrope_Initialize();
  Tightrope_Place(0, 8, 9);
  Tightrope_Place(1, 10, 11);
  Actor_SetModeById(8, 4);
  Actor_SetModeById(9, 4);
  Actor_SetModeById(10, 4);
  Actor_SetModeById(11, 4);
  Actor_PlaceBlocks(gKandoreanTemple_BlockActors);
  REG_BLDALPHA = 0xc08;
  REG_BLDCNT = 0x3f10;
  Scene_QueueMusic(245);
  Task_Add(KandoreanTemple_BlowStream, TASK_PASS_VBLANK);
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  KandoreanTemple_PrepareLift(13);
  KandoreanTemple_PrepareLift(14);
  KandoreanTemple_PrepareLiftBase(15);
  KandoreanTemple_PrepareLiftBase(16);
  gKandoreanTemple_LeftTimer = 0;
  gKandoreanTemple_RightTimer = 0;
  gKandoreanTemple_LeftBobPhase = 0;
  gKandoreanTemple_RightBobPhase = 0;
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance == 5)
      KandoreanTemple_ArriveOnWestLift();
    else if (gMainRoutineWork.entrance == 6)
      KandoreanTemple_ArriveOnEastLift();
  }
  if (GameFlag_Get(0x301))
    Task_Add(KandoreanTemple_RunWestLift, TASK_PASS_VBLANK);
  else if (GameFlag_Get(0x300))
    Task_Add(KandoreanTemple_RunEastLift, TASK_PASS_VBLANK);
  Task_Add(KandoreanTemple_WatchLifts, TASK_PASS_VBLANK);
  FieldEffect_StartBlankOverlay(0);
}

void KandoreanTemple_StartRoom2(void) {
  FieldEffect_StartBlankOverlay(0);
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance == 1)
    GameFlag_Clear(0x12f);
#endif
}

void KandoreanTemple_StartChestbeaterRoom(void) {
  struct Actor *object = Actor_GetById(9);

  Actor_DisableById(9);
  object->priorityFlags |= 2;
  Actor_SetModeById(9, 2);
  object = Actor_GetById(8);
  object->motionFlags = 0;
  if (GameFlag_Get(0x302)) {
    Actor_SetPositionAndResetMotion_(Actor_GetById(8), 0x1380000, 0x200000,
                                     0x1d80000);
    MapGrid_CopyCellAttributes(0, 0, 3, 3, 19, 28);
  } else if (GameFlag_Get(0x303)) {
    Actor_SetPositionAndResetMotion_(Actor_GetById(8), 0x1580000, 0x200000,
                                     0x1d80000);
    MapGrid_CopyCellAttributes(0, 4, 3, 3, 19, 28);
  }
  Actor_PlaceBlocks(gKandoreanTemple_PillarActors);
  object = Actor_GetById(10);
  object->scaleX = 0xb333;
  object->scaleY = 0xb333;
  object->collisionFlags = 0;
  FieldEffect_StartBlankOverlay(0);
  if (Event_GetFlag(0x92d)) {
    {
      s32 e = 1;
      s32 f = 4;

      MapGrid_CopyCellPictureIndicesRect(41, 0, 20, 0, e, f);
    }
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  }
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance == 2) {
      Actor_GetById(gMainRoutineWork.currentOwner)->y = 0x400000;
    } else if (gMainRoutineWork.entrance == 3 && Event_GetFlag(0x92d) == 0) {
      Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
      KandoreanTemple_LowerTheLadder();
    }
  }
#if TLA_PATCH_1
  if (gMainRoutineWork.entrance == 2)
    GameFlag_Clear(0x12f);
#endif
}

void KandoreanTemple_LandFromAbove();
void KandoreanTemple_RiseOnLeftVent();
void KandoreanTemple_RiseOnRightVent();

void KandoreanTemple_StartVentRoom(void) {
  s32 t2, t9, p_r3;

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    t2 = ((s32)Actor_GetById(9));
    FieldActor_SetPositionAndResetMotion(t2, PIXELS(216), 0, PIXELS(216));
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
    t9 = ((s32)Actor_GetById(10));
    Actor_SetPositionAndResetMotion(t9, PIXELS(376), 0, PIXELS(248));
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
    GameFlag_Clear(0x20e);
    GameFlag_Clear(0x20c);
    GameFlag_Clear(0x20d);
  }
  if (GameFlag_Get(0x300)) {
    Actor_SetPosition(8, PIXELS(216), PIXELS(216));
    goto L_0200ad0a;
  }
  if (!GameFlag_Get(0x301))
    goto L_0200ad0a;
  Actor_SetPosition(8, PIXELS(376), PIXELS(248));
L_0200ad0a:
  (*(s16 *)0x4000052) = 0xc08;
  (*(s16 *)0x4000050) = 0x3f10;
  gKandoreanTemple_LeftVentBlowing = 0;
  gKandoreanTemple_RightVentBlowing = 0;
  Task_AddFrame(KandoreanTemple_BlowLeftVent, 0x480);
  Task_AddFrame(KandoreanTemple_BlowRightVent, 0x480);
  Task_Add((s32)KandoreanTemple_DropOffLowCells, 0x480);
  KandoreanTemple_PrepareLift(9);
  KandoreanTemple_PrepareLift(10);
  KandoreanTemple_PrepareLiftBase(11);
  KandoreanTemple_PrepareLiftBase(13);
  KandoreanTemple_PrepareLiftBase(14);
  KandoreanTemple_PrepareLiftBase(15);
  KandoreanTemple_PrepareLiftBase(16);
  KandoreanTemple_PrepareLiftBase(12);
  KandoreanTemple_PrepareLiftBase(17);
  KandoreanTemple_PrepareLiftBase(18);
  KandoreanTemple_PrepareLiftBase(19);
  KandoreanTemple_PrepareLiftBase(20);
  Actor_PlaceBlocks(gKandoreanTemple_PillarActors);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
    goto L_0200adda;
  if (gMainRoutineWork.entrance != 3) {
    p_r3 = gMainRoutineWork.entrance;
    goto L_0200add2;
  }
  KandoreanTemple_RiseOnLeftVent();
  goto L_0200adda;
L_0200add2:
  if (p_r3 != 4)
    goto L_0200adda;
  KandoreanTemple_RiseOnRightVent();
L_0200adda:
  gKandoreanTemple_LeftTimer = 0;
  gKandoreanTemple_RightTimer = 0xfa0;
  gKandoreanTemple_LeftBobPhase = 0;
  gKandoreanTemple_RightBobPhase = 0;
  Task_AddFrame(KandoreanTemple_RunLeftVent, 0x480);
  Task_Add((s32)KandoreanTemple_RunRightVent, 0x480);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
    goto L_0200ae24;
  if (gMainRoutineWork.entrance != 5)
    goto L_0200ae24;
  KandoreanTemple_LandFromAbove();
L_0200ae24:
  Task_AddFrame(KandoreanTemple_WatchVentPlatforms, 0x480);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  FieldEffect_StartBlankOverlay(0);
}

#include "../field/field_effect/advance_motion.c"

#undef DEFINE_KANDOREAN_TEMPLE_VENT
#undef BOB
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef gKandoreanTemple_Placements
#undef gKandoreanTemple_VentRoomPlacements
#undef gKandoreanTemple_FogRoomPlacements
#undef gKandoreanTemple_LiftRoomEvents
#undef gKandoreanTemple_ChestbeaterRoomEvents
#undef gKandoreanTemple_VentRoomEvents
#undef gKandoreanTemple_Room5Events
#undef gKandoreanTemple_Room6Events
#undef gKandoreanTemple_FogRoomEvents
#undef gKandoreanTemple_GateEvents
#undef gKandoreanTemple_PoisHallEvents
// @file
// The Kandorean Temple's events, from the vents' rides to Poi's test, and its
// tables.

#include "text/msg_ids.h"

extern s32 gKandoreanTemple_RightTimer;

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void KandoreanTemple_LandFromAbove(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(204);
  leader->motionFlags = 3;
  Event_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (i = 0; i <= 16; i++) {
    s32 velocity[3];

    velocity[0] = Trig_Cos(i << 12);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(i << 12);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14) >> 16;
    velocity[0] += -0x6000;
    velocity[2] += (((Random16() * 3) << 13) >> 16) + -0x3000;
    FieldEffect_Spawn(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                      velocity[2], 0x01090001, &options);
  }
  Sound_Play(188);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Event_End();
}

void KandoreanTemple_GrowColumn(s32 id, s32 offset, s32 limit);

void KandoreanTemple_RiseOnLeftVent(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *statue = Actor_GetById(9);
  struct Actor *ride = FieldCamera_GetActor();
  s16 i;
  u16 next;
  s32 limit;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  leader->motionFlags = 0;
  wait(1);
  GameFlag_Set(0x208);
  GameFlag_Clear(0x209);
  GameFlag_Set(0x301);
  GameFlag_Clear(0x300);
  Sound_Play(162);
  KandoreanTemple_StartLeftVent();
  i = 0;
  do {
    leader->y += 0x48000;
    statue->y += 0x48000;
    ride->y += 0x1cccc;
    limit = 0x1000;
    KandoreanTemple_GrowColumn(11, 0x20000, limit);
    KandoreanTemple_GrowColumn(13, 0x18000, limit);
    KandoreanTemple_GrowColumn(14, 0x10000, limit);
    KandoreanTemple_GrowColumn(15, 0x8000, limit);
    KandoreanTemple_GrowColumn(16, 0, limit);
    next = i;
    next++;
    i = next;
  } while (next <= 15);
  Map_CenterOnFollowedPoint();
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
#define DEFINE_KANDOREAN_TEMPLE_SINK_STEP(n1, n2, n3, n4, n5)                  \
  do {                                                                         \
    leader->y += -0x6c000;                                                     \
    statue->y += -0x6c000;                                                     \
    ride->y += -0x2b333;                                                       \
    limit = 0x1800;                                                            \
    KandoreanTemple_ShrinkColumn(n1, 0x30000, limit);                          \
    KandoreanTemple_ShrinkColumn(n2, 0x24000, limit);                          \
    KandoreanTemple_ShrinkColumn(n3, 0x18000, limit);                          \
    KandoreanTemple_ShrinkColumn(n4, 0xc000, limit);                           \
    KandoreanTemple_ShrinkColumn(n5, 0, limit);                                \
    wait(1);                                                                   \
  } while (Actor_GetById(n5)->scaleY > limit);

  DEFINE_KANDOREAN_TEMPLE_SINK_STEP(11, 13, 14, 15, 16)
  gKandoreanTemple_LeftVentBlowing = 0;
  GameFlag_Clear(0x20c);
  Sound_Play(390);
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Actor_SetModeById(9, 1);
  Actor_SetPositionAndResetMotion_(Actor_GetById(9), 0xd80000, 0, 0xd80000);
  statue->priorityFlags |= 2;
  Actor_SetSpritePriority(9, 3);
  Actor_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Event_End();
}

void KandoreanTemple_StartRightVent(void);

void KandoreanTemple_RiseOnRightVent(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *statue = Actor_GetById(10);
  struct Actor *ride = FieldCamera_GetActor();
  s16 i;
  u16 next;
  s32 limit;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  leader->motionFlags = 0;
  wait(1);
  GameFlag_Clear(0x208);
  GameFlag_Set(0x209);
  GameFlag_Clear(0x301);
  GameFlag_Set(0x300);
  Sound_Play(162);
  KandoreanTemple_StartRightVent();
  i = 0;
  do {
    leader->y += 0x48000;
    statue->y += 0x48000;
    ride->y += 0x1cccc;
    limit = 0x1000;
    KandoreanTemple_GrowColumn(12, 0x20000, limit);
    KandoreanTemple_GrowColumn(17, 0x18000, limit);
    KandoreanTemple_GrowColumn(18, 0x10000, limit);
    KandoreanTemple_GrowColumn(19, 0x8000, limit);
    KandoreanTemple_GrowColumn(20, 0, limit);
    next = i;
    next++;
    i = next;
  } while (next <= 15);
  Map_CenterOnFollowedPoint();
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  DEFINE_KANDOREAN_TEMPLE_SINK_STEP(12, 17, 18, 19, 20)
  gKandoreanTemple_RightVentBlowing = 0;
  GameFlag_Clear(0x20d);
  Sound_Play(390);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
  Actor_SetModeById(10, 1);
  Actor_SetPositionAndResetMotion_(Actor_GetById(10), 0x1780000, 0, 0xf80000);
  statue->priorityFlags |= 2;
  Actor_SetSpritePriority(10, 3);
  Actor_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Event_End();
}

void KandoreanTemple_LeaveBySteam(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

void KandoreanTemple_LeaveBySteamExit3(void) {
  KandoreanTemple_LeaveBySteam();
  Import_EventSetExitRequest(3);
}

void KandoreanTemple_LeaveBySteamExit4(void) {
  KandoreanTemple_LeaveBySteam();
  Import_EventSetExitRequest(4);
}

void KandoreanTemple_SinkIntoTheSteam(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 i;
  struct MainRoutineWork *party;

  Event_Start();
  Event_ResolvePendingActions(0);
  object->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  object->facing = 0;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 16);
  for (i = 0; i <= 7; i++) {
    Actor_SetPositionAndResetMotion(object, object->x, object->y,
                                    object->z + PIXELS(3));
    wait(1);
  }
  Event_Wait(20);
  party = &gMainRoutineWork;
  object->facing = FACING_SOUTH;
  FieldEffect_SetAttached(party->currentOwner, 258);
  Actor_SetModeById(party->currentOwner, 27);
  Event_Wait(30);
  Actor_SetModeById(party->currentOwner, 28);
  Event_Wait(10);
  Actor_SetSpritePriority(party->currentOwner, 3);
  for (i = 0; i <= 39; i++) {
    {
      s32 scale = object->scaleX;

      object->scaleX = scale + -0x400;
    }
    object->scaleY += -0x400;
    object->y += -0x20000;
    wait(1);
    if (i == 10)
      Sound_Play(204);
    if (i == 20) {
      ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
      Event_FinishScreenTransition();
    }
  }
  Import_EventSetExitRequest(5);
}

// The leader and actor 13 rise into view while actor 15 scales.
#define DEFINE_KANDOREAN_TEMPLE_RISE(name, n1, n2, n3, n4, n5, n6, n7, data1)  \
  void name(void) {                                                            \
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);       \
    struct Actor *first = Actor_GetById(n1);                                   \
    struct Actor *second = Actor_GetById(n2);                                  \
                                                                               \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    Scene_QueueMusic(162);                                                     \
    FieldCamera_MoveTo(-1, -1, -1, 0);                                         \
    wait(1);                                                                   \
    leader->motionFlags = 0;                                                   \
    Actor_SetPositionAndResetMotion_(                                          \
        Actor_GetById(gMainRoutineWork.currentOwner), PIXELS(n3), PIXELS(-20), \
        PIXELS(n4));                                                           \
    Actor_SetSingleChildFlags_(Actor_GetById(gMainRoutineWork.currentOwner),   \
                               0);                                             \
    Actor_SetPositionAndResetMotion_(Actor_GetById(n1), PIXELS(n3),            \
                                     PIXELS(-20), PIXELS(n5));                 \
    Actor_SetPositionAndResetMotion_(Actor_GetById(n2), PIXELS(n3),            \
                                     PIXELS(-20), PIXELS(n4));                 \
    Actor_SetSpritePriority(n1, 2);                                            \
    Actor_SetSpritePriority(n2, 3);                                            \
    {                                                                          \
      u8 *flags = &second->priorityFlags;                                      \
      s32 c = 2 | *flags;                                                      \
                                                                               \
      *flags = c;                                                              \
    }                                                                          \
    second->scaleY = 0x400;                                                    \
    Actor_SetModeById(n1, 4);                                                  \
    Actor_SetModeById(n2, 2);                                                  \
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;            \
    Event_StartScreenTransition();                                             \
    Event_WaitTransitionFrames();                                              \
    /* A goto loop, as in KandoreanTemple_RideWestLiftUp. */                   \
  rise:                                                                        \
    first->y += PIXELS(4);                                                     \
    leader->y += PIXELS(4);                                                    \
    KandoreanTemple_GrowColumn(n2, 0x4ccc, 0x4ccc);                            \
    wait(1);                                                                   \
    if (first->y < 0)                                                          \
      goto rise;                                                               \
    MapGrid_CopyCellAttributes(0, 0, 1, 1, n6, n7);                            \
    data1 = 2000;                                                              \
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 516;            \
    Event_End();                                                               \
  }

DEFINE_KANDOREAN_TEMPLE_RISE(KandoreanTemple_ArriveOnWestLift, 13, 15, 440, 200,
                             204, 27, 12, gKandoreanTemple_LeftTimer)

// As KandoreanTemple_ArriveOnWestLift, for actors 14 and 16 at another place.
DEFINE_KANDOREAN_TEMPLE_RISE(KandoreanTemple_ArriveOnEastLift, 14, 16, 600, 232,
                             236, 37, 14, gKandoreanTemple_RightTimer)

// Actor 13 rises out of view while actor 15 scales, then the scene moves on.
void KandoreanTemple_RideWestLiftUp(void) {
  struct Actor *object = Actor_GetById(13);

  Event_Start();
  Event_ResolvePendingActions(0);
  // A goto loop: as a do-while, loop.c hoists the scale constant into r6
  // (differ 19).
rise:
  object->y -= PIXELS(4);
  KandoreanTemple_ShrinkColumn(15, 0x4ccc, 0x4ccc);
  wait(1);
  if (object->y > -0x140000)
    goto rise;
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(6);
}

// As KandoreanTemple_RideWestLiftUp, for actors 14 and 16.
void KandoreanTemple_RideEastLiftUp(void) {
  struct Actor *object = Actor_GetById(14);

  Event_Start();
  Event_ResolvePendingActions(0);
  // A goto loop, as in KandoreanTemple_RideWestLiftUp.
rise:
  object->y -= PIXELS(4);
  KandoreanTemple_ShrinkColumn(16, 0x4ccc, 0x4ccc);
  wait(1);
  if (object->y > -0x140000)
    goto rise;
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(7);
}

void KandoreanTemple_StartRoom5(void) {
  struct MapWork17 *map = (struct MapWork17 *)Ram_HeapSlots->mapWork;

  gKandoreanTemple_SteamCounter = 0;
  map->mode = 1;
  FieldEffect_StartBlankOverlay(0);
#if TLA_PATCH_1
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2)
    GameFlag_Clear(0x12f);
#endif
}

void KandoreanTemple_StartRoom6(void) {
  struct MapWork17 *map = (struct MapWork17 *)Ram_HeapSlots->mapWork;

  gKandoreanTemple_SteamCounter = 0;
  Scene_QueueMusic(170);
  map->mode = 1;
  FieldEffect_StartBlankOverlay(0);
}

void KandoreanTemple_StartFogRoom(void) {
  Rope_Initialize();
  Rope_Place(0, 0x20f, 8, 9);
  if (GameFlag_Get(0x849)) {
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  } else {
    Actor_SetSpriteFlags(Actor_GetById(10), 0);
    Actor_SetModeById(10, 6);
    Actor_SetMovementTypeById(10, gKandoreanTemple_LevitateScript);
  }
  FieldEffect_StartBlankOverlay(0);
}

void KandoreanTemple_StartMeditating(s32 id, s32 base) {
  struct Actor *object = Actor_GetById(id);

  Actor_SetModeById(id, 5);
  object->timer = base + ((Random16() * 400) >> 16);
  object->unk66 = id;
  Actor_SetMovementTypeById(id, gKandoreanTemple_MeditateScript);
}

void KandoreanTemple_StartGate(void) {
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x100;
  if (GameFlag_Get(0x842)) {
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 5, 13);
  } else {
    Actor_DisableById(8);
  }
  if (GameFlag_Get(0x849)) {
    MapGrid_CopyCellAttributes(2, 0, 2, 3, 17, 24);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetPositionAndFacing(11, PIXELS(374), PIXELS(282), FACING_EAST);
    {
      s32 angle = 0xd000;

      Actor_SetPositionAndFacing(12, PIXELS(272), PIXELS(296), angle);
    }
    {
      s32 angle = 0x3000;

      Actor_SetPositionAndFacing(13, PIXELS(216), PIXELS(264), angle);
    }
  } else {
    KandoreanTemple_StartMeditating(11, 0);
    KandoreanTemple_StartMeditating(12, 240);
    KandoreanTemple_StartMeditating(13, 480);
  }
  if (GameFlag_Get(0x212)) {
    MapGrid_CopyCellPictureIndicesRect(77, 33, 77, 24, 2, 2);
    MapGrid_CopyCellAttributes(32, 30, 2, 1, 17, 25);
  }
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 4)
    GameFlag_Clear(0x12f);
#endif
}

void KandoreanTemple_PlaceTheLevitator(void) {
  if (Event_GetFlag(0x210) == 0) {
    Event_SetFlag(0x210);
    Func_020053a8_(1, 0x210, 10, 11);
  }
  {
    s32 angle = 0x3000;

    Actor_SetPositionAndFacing(12, PIXELS(392), PIXELS(360), angle);
  }
}

void KandoreanTemple_MatchLiftPriority(void);

void KandoreanTemple_StartPoisHall(void) {
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
  Rope_Initialize();
  Func_020053a8_(0, 527, 8, 9);
  Rope_Place(1, 528, 10, 11);
  if (Event_GetFlag(0x849) == 0) {
    Actor_SetModeById(13, 5);
    Actor_SetModeById(15, 5);
  }
  Actor_SetModeById(14, 5);
  Task_Add(KandoreanTemple_MatchLiftPriority, TASK_PASS_VBLANK);
  if ((gMainRoutineWork.entrance >= 4 && gMainRoutineWork.entrance <= 6) ||
      gMainRoutineWork.entrance == 14) {
    if (GameFlag_Get(0x84a)) {
      s32 angle = 0x5000;

      Actor_SetPositionAndFacing(12, PIXELS(432), PIXELS(394), angle);
    } else {
      KandoreanTemple_CreateLashPebble();
      if (GameFlag_Get(0x849))
        KandoreanTemple_PlaceTheLevitator();
    }
  }
  if ((gMainRoutineWork.entrance >= 1 && gMainRoutineWork.entrance <= 3) ||
      gMainRoutineWork.entrance == 9) {
    if (GameFlag_Get(0x212))
      KandoreanTemple_OpenTheTrapdoor();
    if (GameFlag_Get(0x849)) {
      Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
      {
        s32 facing = FACING_NORTHWEST + FACING_STEP;

        Actor_SetPositionAndFacing(15, PIXELS(408), PIXELS(664), facing);
      }
    } else if (GameFlag_Get(0x848)) {
      Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
    } else {
      KandoreanTemple_TestTheStudents();
    }
  } else if (gMainRoutineWork.entrance == 6) {
    if (GameFlag_Get(0x849) == 0)
      KandoreanTemple_PraiseTheLevitator();
  } else if (gMainRoutineWork.entrance == 13) {
    KandoreanTemple_FindTheRopeOnTheFloor();
  } else if (gMainRoutineWork.entrance == 14) {
    if (GameFlag_Get(0x849) == 0)
      KandoreanTemple_MeetPoi();
  }
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance >= 8 && gMainRoutineWork.entrance <= 9)
    GameFlag_Clear(0x12f);
#endif
}

void KandoreanTemple_Wobble(struct FieldSprite *sprite, u32 frames, s32 step) {
  u32 count = frames;
  u32 i;

  for (i = 0; i < count; i++) {
    sprite->rotation += step;
    Event_Wait(1);
  }
}

void KandoreanTemple_Sway(struct Actor *object, u32 frames, s32 step) {
  u32 count = frames;
  u32 i;

  for (i = 0; i < count; i++) {
    object->x += step;
    Event_Wait(1);
  }
}

void KandoreanTemple_TestTheStudents(void) {
  struct Actor *object;
  struct FieldSprite *sprite;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                         0x1900000, 0x31c0000);
  wait(1);
  Event_StartScreenTransition();
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 400, 752);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  FieldCamera_MoveWithinMapBounds(0x1b40000, 0x480000, 0x2980000, 1);
  FieldCamera_WaitForArrival();
  Actor_Jump(12, 4, 0);
  Event_SetMessageId((s32)&MsgKandoreanTemple04);
  Event_ShowMessageAndWait(12, 0);
  FieldCamera_MoveWithinMapBounds(0x1c80000, 0, 0x2b00000, 1);
  FieldCamera_WaitForArrival();
  Motion_SetVarCbAndRefresh(13, 2);
  Sound_Play(131);
  Actor_ConfigureSpriteColor(13, 0x100);
  Event_Wait(20);
  Actor_ConfigureSpriteColor(13, 0);
  Event_Wait(20);
  object = Actor_GetById(13);
  sprite = object->sprite;
  object->motionFlags = 0;
  i = 0;
rise13:
  object->y += 0xccc;
  Event_Wait(1);
  i++;
  if (i <= 63)
    goto rise13;
  KandoreanTemple_Wobble(sprite, 32, 32);
  Event_Wait(20);
  Actor_SetAttachedEffect(13, 0x102);
  KandoreanTemple_Wobble(sprite, 32, -64);
  KandoreanTemple_Wobble(sprite, 32, 64);
  KandoreanTemple_Wobble(sprite, 64, 32);
  FieldEffect_SetAttached(13, 0x102);
  Event_Wait(60);
  object->motionFlags = 3;
  Event_Wait(5);
  Sound_Play(159);
  object->velocityY = 0x30000;
  sprite->rotation = 0;
  Event_Wait(60);
  Event_ShowMessageAndWait(13, 0);
  FieldCamera_MoveTo(0x1b40000, 0x480000, 0x2980000, 1);
  FieldCamera_WaitForArrival();
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 476, 664);
  Actor_FaceDirection(12, 0x3000, 0);
  Actor_SetModeById(12, 4);
  Event_ShowMessageAndWait(0x100c, 0);
  Actor_WalkToAndWait(12, 474, 664);
  Actor_WalkToAndWait(12, 472, 648);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Camera_SetSpeedParameters(0x13333, 0x2666);
  FieldCamera_MoveWithinMapBounds(0x16c0000, 0x480000, 0x2980000, 1);
  FieldCamera_WaitForArrival();
  Actor_SetHorizontalPositionWithTerrain(12, 0x1480000, 0x2880000);
  wait(1);
  Actor_WalkToAndWait(12, 326, 664);
  Actor_WalkToAndWait(12, 320, 664);
  Actor_FaceDirection(12, 0x5000, 0);
  Event_ShowMessageAndWait(0x200c, 0);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  FieldCamera_MoveWithinMapBounds(0x1560000, 0, 0x2b00000, 1);
  FieldCamera_WaitForArrival();
  Motion_SetVarCbAndRefresh(14, 2);
  Sound_Play(131);
  Actor_ConfigureSpriteColor(14, 0x100);
  Event_Wait(20);
  Actor_ConfigureSpriteColor(14, 0);
  Event_Wait(20);
  Actor_SetAttachedEffect(14, 0x101);
  object = Actor_GetById(14);
  object->motionFlags = 0;
  i = 0;
rise14:
  object->y += 0x2666;
  Event_Wait(1);
  i++;
  if (i <= 63)
    goto rise14;
  KandoreanTemple_Sway(object, 64, 0x1000);
  KandoreanTemple_Sway(object, 128, -0x1000);
  KandoreanTemple_Sway(object, 128, 0x1000);
  KandoreanTemple_Sway(object, 64, -0x1000);
  Event_Wait(20);
  Actor_SetAttachedEffect(14, 0x100);
  object->motionFlags = 3;
  Event_Wait(5);
  Sound_Play(159);
  object->velocityY = 0x30000;
  Event_Wait(60);
  Event_ShowMessageAndWait(14, 0);
  FieldCamera_MoveWithinMapBounds(0x16c0000, 0x480000, 0x2980000, 1);
  FieldCamera_WaitForArrival();
  Event_ShowMessageAndWait(0x200c, 0);
  Actor_FaceDirection(12, 0x3000, 10);
  Event_ShowMessageAndWait(12, 0);
  FieldCamera_MoveWithinMapBounds(0x1900000, 0, 0x2b00000, 1);
  FieldCamera_WaitForArrival();
  Motion_SetVarCbAndRefresh(15, 2);
  Sound_Play(131);
  Actor_ConfigureSpriteColor(15, 0x100);
  Event_Wait(20);
  Actor_ConfigureSpriteColor(15, 0);
  Event_Wait(20);
  object = Actor_GetById(15);
  sprite = object->sprite;
  object->motionFlags = 0;
  i = 0;
rise15:
  object->y += 0x3333;
  Event_Wait(1);
  i++;
  if (i <= 63)
    goto rise15;
  KandoreanTemple_Wobble(sprite, 32, 64);
  sprite->rotation = 0;
  Motion_SetVarCbAndRefresh(15, 1);
  Event_Wait(60);
  KandoreanTemple_Wobble(sprite, 32, -64);
  sprite->rotation = 0;
  Motion_SetVarCbAndRefresh(15, 1);
  Event_Wait(60);
  i = 0;
fall15:
  object->y += -0x3333;
  Event_Wait(1);
  i++;
  if (i <= 63)
    goto fall15;
  Event_Wait(20);
  Actor_ShowEmoteById(15, 0x108, 0);
  Event_ShowMessageThenWait(15, 0, 20);
  Event_ShowMessageAndWait(0x100c, 0);
  Actor_ShowEmoteById(15, 0x103, 20);
  Actor_SetModeById(15, 1);
  Actor_Jump(15, 4, 0);
  Actor_FaceDirection(15, 0xb000, 20);
  Event_ShowMessageAndWait(0x400f, 0);
  FieldCamera_MoveWithinMapBounds(0x16c0000, 0x480000, 0x2980000, 1);
  FieldCamera_WaitForArrival();
  Actor_ShowEmoteById(12, 0x102, 20);
  Event_ShowMessage(0x800c, 0);
  Event_ShowMessageAndWait(15, 0);
  Actor_FaceDirection(12, 0xb000, 40);
  Actor_SetModeById(12, 4);
  Event_ShowMessage(0x800c, 0);
  Event_ShowMessageAndWait(15, 0);
  Actor_ShowEmoteById(12, 0x105, 80);
  Event_ShowMessage(0x800c, 0);
  Actor_FaceDirection(12, 0x3000, 20);
  Actor_SetModeById(12, 3);
  Event_ShowMessageAndWait(0x800c, 0);
  Actor_WalkToAndWait(12, 326, 664);
  Actor_WalkToAndWaitById(12, 328, 648);
  Actor_SetSpriteFlags(Actor_GetById(12), 0);
  Actor_SetSpriteColor(Actor_GetById(12), 15);
  Actor_SetPosition(12, 0x1b80000, 0x2580000);
  FieldCamera_MoveTo(0x1900000, 0, 0x2b00000, 1);
  Actor_SetSpeed(15, 0xcccc, 0x6666);
  Actor_WalkToAndWait(15, 400, 644);
  Event_Wait(20);
  Sound_Play(188);
  KandoreanTemple_OpenTheTrapdoor();
  Event_ShowMessageAndWait(12, 0);
  Task_Remove(KandoreanTemple_MatchLiftPriority);
  Motion_SetModeAndWaitAnimation(15, 3);
  Actor_SetSpritePriority(15, 3);
  Actor_WalkToAndWait(15, 400, 640);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  FieldCamera_MoveWithinMapBounds(0x1900000, 0, 0x2f00000, 1);
  FieldCamera_WaitForArrival();
  Task_Add(KandoreanTemple_MatchLiftPriority, 0x480);
  Event_End();
  GameFlag_Set(0x848);
}

void KandoreanTemple_PraiseTheLevitator(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                         PIXELS(328), PIXELS(360));
  {
    s32 facing = FACING_SOUTH + FACING_STEP;

    Actor_SetPositionAndFacing(12, PIXELS(472), PIXELS(408), facing);
  }
  Actor_GetById(16)->facing = 0xc000;
  wait(1);
  Event_StartScreenTransition();
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 328, 392);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveWithinMapBounds(PIXELS(408), -1, PIXELS(400), 1);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 376, 408);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(12, 1);
  Event_SetMessageId((s32)&MsgKandoreanTemple05);
  Event_ShowMessageAndWait(12, 0);
  Actor_FaceDirection(12, FACING_WEST, 20);
  Actor_ShowEmoteById(12, 256, 0);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_WalkToAndWait(12, 456, 408);
  Event_ShowMessageAndWait(12, 0);
  Actor_MoveToObject(16, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(16, 0xcccc, 0x6666);
  Actor_WalkToAndWait(16, 376, 392);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(16, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(16, FACING_EAST, 20);
  Actor_SetAttachedEffect(12, 258);
  Event_ShowMessageAndWait(12, 0);
  Actor_ShowEmoteById(16, 265, 0);
  Actor_FaceDirection(16, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
  Event_ShowMessageAndWait(16, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(16, FACING_EAST, 0);
  Actor_WalkToAndWait(12, 468, 428);
  Actor_WalkToAndWait(12, 472, 454);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 514;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(13);
}

void KandoreanTemple_FindTheRopeOnTheFloor(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  wait(1);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 514;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetHorizontalPositionWithTerrain(12, PIXELS(472), PIXELS(648));
  wait(1);
  Actor_WalkToAndWait(12, 472, 664);
  Actor_FaceDirection(12, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(12, FACING_SOUTHEAST + FACING_STEP, 0);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  FieldCamera_MoveWithinMapBounds(PIXELS(456), PIXELS(40), PIXELS(664), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Event_SetMessageId((s32)&MsgKandoreanTemple06);
  Event_ShowMessageAndWait(0x800c, 0);
  Actor_WalkToAndWait(12, 472, 648);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(14);
}

void KandoreanTemple_StudentWalksUp(void) {
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_WalkToAndWait(12, 496, 392);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_ShowMessageAndWait(0x400c, 0);
}

#define WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

void KandoreanTemple_MeetPoi(void) {
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 *flags;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  wait(1);
  Actor_SetPositionAndFacing(16, 0x1780000, 0x1880000, 0);
  Actor_SetHorizontalPositionAndAngleWithTerrain(gMainRoutineWork.currentOwner,
                                                 0x1780000, 0x1980000, 0);
  {
    s32 angle = 0xb000;
    Actor_SetPositionAndFacing(12, 0x1d80000, 0x1c60000, angle);
  }
  WORK->screenEffect = 0x202;
  Event_StartScreenTransition();
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_WalkToAndWait(12, 464, 428);
  Actor_WalkToAndWait(12, 432, 408);
  Actor_FaceDirection(12, 0x8000, 0);
  FieldEffect_SetAttached(12, 0x102);
  Event_SetMessageId((s32)&MsgKandoreanTemple07);
  Event_ShowMessageAndWait(12, 0);
  Actor_ShowEmoteById(12, 0x105, 40);
  Event_ShowMessageAndWait(12, 0);
  Actor_ArmCallback(12, 0, 40);
  Event_ShowMessageAndWait(12, 0);
  Event_ShowMessageAndWait(16, 0);
  Motion_SetVarCbAndRefresh(12, 2);
  Actor_FaceDirection(12, 0x8000, 20);
  Actor_SetModeById(12, 4);
  Event_ShowMessageAndWait(12, 0);
  Actor_FaceDirection(16, 0x3000, 10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  Event_OpenMessage(16, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(12, 4);
    Event_ShowMessageAndWait(12, 0);
    WORK->messageId++;
  } else {
    WORK->messageId++;
    Actor_SetModeById(12, 3);
    Event_ShowMessageAndWait(12, 0);
  }
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(16, 0, 0);
  Actor_ShowEmote(16, 0x101, 20);
  Event_ShowMessageAndWait(16, 0);
  Actor_SetAttachedEffect(12, 0x102);
  Event_ShowMessageAndWait(12, 0);
  Actor_SetModeById(16, 3);
  Event_ShowMessageThenWait(16, 0, 40);
  Actor_FaceDirection(12, 0x5000, 40);
  Event_ShowMessageAndWait(12, 0);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 40);
  Actor_FaceDirection(12, 0x8000, 20);
  Actor_SetModeById(12, 3);
  Event_ShowMessageAndWait(12, 0);
  Actor_ArmCallback(16, 0, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ShowEmoteById(16, 0x101, 0);
  Event_ShowMessageAndWait(16, 0);
  Actor_SetModeById(12, 3);
  Event_ShowMessageAndWait(12, 0);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveWithinMapBounds(0x1a80000, 0x100000, 0x1800000, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetSpeed(16, 0xcccc, 0x6666);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                            gKandoreanTemple_PoiWalksOutScript);
  Actor_SetMovementTypeById(16, gKandoreanTemple_StudentWalksAsideScript);
  Actor_SetMovementTypeAndWaitById(12, gKandoreanTemple_StudentWalksAwayScript);
  Event_Wait(40);
  KandoreanTemple_CastLash();
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xa000, 20);
  Event_OpenMessage(0x5010, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    KandoreanTemple_StudentWalksUp();
    WORK->messageId++;
  } else {
    WORK->messageId++;
    KandoreanTemple_StudentWalksUp();
  }
  Actor_ArmCallback(16, 0, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetModeById(12, 3);
  Event_ShowMessage(0x400c, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_ShowMessage(0x400c, 0);
  Actor_ShowEmoteById(16, 0x100, 40);
  Event_ShowMessage(0x5010, 0);
  Actor_SetModeById(12, 3);
  Event_ShowMessage(0x400c, 0);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xa000, 0);
  Event_OpenMessage(0x5010, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(12, 3);
    Event_ShowMessageAndWait(0x400c, 0);
    Actor_ArmCallback(16, 0, 0);
    Actor_ShowEmoteById(16, 0x107, 20);
    Event_ShowMessageAndWait(0x5010, 0);
    WORK->messageId += 2;
  } else {
    WORK->messageId += 2;
    Actor_SetModeById(12, 4);
    Event_ShowMessageAndWait(0x400c, 0);
    Actor_ArmCallback(16, 0, 0);
    Actor_ShowEmoteById(16, 0x107, 20);
    Event_ShowMessageAndWait(0x5010, 0);
  }
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  FieldCamera_MoveTo(0x1a80000, 0, 0x1900000, 1);
  FieldCamera_SetSpeed(0x9999, 0x1333);
  Actor_SetSpeed(12, 0x9999, 0x4ccc);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 12);
  Actor_WatchAndSetScript(16, 12);
  Actor_WalkToAndWait(12, 488, 416);
  Actor_WalkToAndWait(12, 472, 432);
  Actor_FaceDirection(12, 0x5000, 0);
  Event_ShowMessage(0x400c, 0);
  Event_ShowMessageThenWait(0x1010, 0, 80);
  Actor_ShowEmoteById(12, 0x108, 0);
  Actor_FaceDirection(12, 0xb000, 20);
  Event_ShowMessage(0x400c, 0);
  Actor_SetModeById(12, 3);
  Event_ShowMessageAndWait(0x400c, 0);
  FieldCamera_MoveTo(0x1a80000, 0x100000, 0x1800000, 1);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_WalkToAndWait(12, 488, 416);
  Actor_WalkToAndWait(12, 504, 376);
  Actor_ArmCallback(12, 0x8000, 20);
  object = Actor_GetById(12);
  sprite = object->sprite;
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_Jump(12, 4, 0);
  Actor_MoveToAndWait(12, 492, 376);
  Actor_SetModeById(12, 7);
  object->scaleX = 0xffff0000;
  sprite->rotation = 0xe000;
  /* FAKEMATCH(temporary): the motion flags written through a byte pointer, so
   * the store may alias the rotation and stays after it; as
   * object->motionFlags, which keeps the struct's alias set, 51 of 783
   * instructions differ in tla-en. */
  flags = &object->motionFlags;
  *flags = 0;
  object->y += 0x40000;
  Actor_SetSpriteFlags(Actor_GetById(12), 0);
  Actor_SetSpeed(12, 0x6666, 0x3333);
  Actor_SetMoveTarget((s32)object, 0x1bc0000, 0x280000, 0x1780000);
  Actor_WaitForMoveById(12);
  *flags = 3;
  Actor_SetSpriteFlags(Actor_GetById(12), 1);
  sprite->rotation = 0;
  object->scaleX = 0x10000;
  object->terrainHeight = 0x200000;
  Actor_SetModeById(12, 1);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_Jump(12, 4, 0);
  Actor_MoveToAndWait(12, 424, 376);
  FieldCamera_MoveWithinMapBounds(0x1980000, 0x100000, 0x1900000, 1);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 392, 360);
  Actor_FaceDirection(12, 0x3000, 0);
  Actor_SetSpritePriority(12, 2);
  Actor_Stop(16);
  Actor_Stop(gMainRoutineWork.currentOwner);
  wait(1);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xa000, 10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(16, 3);
  Actor_SetModeById(16, 2);
  {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

    if (leader != NULL)
      Actor_SetDestinationById(16, *(s16 *)((u8 *)leader + 10),
                               *(s16 *)((u8 *)leader + 18));
  }
  Actor_WaitForMoveById(16);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  GameFlag_Set(0x849);
#if TLA_PATCH_1
  WORK->screenEffect = 0x209;
#endif
  Event_End();
}

void KandoreanTemple_CreateLashPebble(void) {
  struct Actor **slot = &gKandoreanTemple_LashPebble;
  s32 zero = 0;
  struct Actor *object = ((struct Actor *)Import_ActorCreate(
      0x1e9, 0x1980000, 0x280000, 0x1580000));

  *slot = object;
  if (object) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    object->motionFlags = zero;
    object->unk5c[0] = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_Alloc(68, 0x608);
    Icon_DrawItem(198);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
}

void KandoreanTemple_LowerTheLadder(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Sound_Play(121);
  MapGrid_CopyCellPictureIndicesRect(41, 0, 20, 0, 1, 1);
  Event_Wait(10);
  MapGrid_CopyCellPictureIndicesRect(41, 1, 20, 1, 1, 1);
  Event_Wait(10);
  MapGrid_CopyCellPictureIndicesRect(41, 2, 20, 2, 1, 1);
  Event_Wait(10);
  MapGrid_CopyCellPictureIndicesRect(41, 3, 20, 3, 1, 1);
  Event_Wait(10);
  GameFlag_Set(0x92d);
  Event_End();
}

void KandoreanTemple_FightFog(void);
void KandoreanTemple_StartMovingPillar(void);
void KandoreanTemple_ReceiveLash(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

s32 gKandoreanTemple_SteamScript[] = {
    0,
    44,
    22,
    38,
    0,
    3,
    0,
    1,
    38,
    46,
    (s32)KandoreanTemple_CountDownSteam,
    22,
    38,
    0,
    3,
    0,
    1,
    38,
};

s16 gKandoreanTemple_BlockActors[] = {
    12, -1,
};

s16 gKandoreanTemple_PillarActors[] = {
    8, -1,
};

u32 gKandoreanTemple_PuffScript[] = {
    0, 24, 38,
};

u32 gKandoreanTemple_StreamPuffScript[] = {
    0, 15, 3, 0, 1, 38,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

u32 gKandoreanTemple_LookAroundScript[] = {
    SC_SET(SW_FACING, FACING_SOUTH + FACING_STEP),
    SC_WAIT(80),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(80),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKandoreanTemple_MeditateScript[] = {
    46,
    (s32)KandoreanTemple_MeditateRestlessly,
};

s32 gKandoreanTemple_LevitateScript[] = {
    SC_CALL(KandoreanTemple_LevitateStudentBob),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gKandoreanTemple_PoiWalksOutScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(456), 0, PIXELS(408)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_END,
};

DEFINE_G_DATA_02002FCC(gKandoreanTemple_StudentWalksAsideScript, 440, 392, 1)

u32 gKandoreanTemple_StudentWalksAwayScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(496), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(504), 0, PIXELS(376)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

s32 gKandoreanTemple_LevitateHighScript[] = {
    SC_CALL(KandoreanTemple_PlaceLevitatingStudent),
    SC_SET(SW_Y, PIXELS(88)),
    SC_END,
};

s32 gKandoreanTemple_LevitateLowScript[] = {
    SC_CALL(KandoreanTemple_PlaceLevitatingStudent),
    SC_SET(SW_Y, PIXELS(44)),
    SC_END,
};

s32 gKandoreanTemple_StandStillScript[] = {
    SC_CALL(KandoreanTemple_PlaceLevitatingStudent),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gKandoreanTemple_ChestbeaterRoomRegions[] = {
    0x4a0140, 0x1500060, 0x70005a, 0x2ffff, 0xffff, 0,
    0, 0,
};

u32 gKandoreanTemple_Room5Regions[] = {
    0x2a0200, 0x2100110, 0x120003a, 0x2ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(14),
    SCENE_EXIT(1, 20, 1),
    SCENE_EXIT(2, 20, 3),
    SCENE_EXIT(3, 19, 1),
    SCENE_EXIT(4, 17, 2),
    SCENE_EXIT(5, 17, 5),
    SCENE_EXIT(6, 17, 3),
    SCENE_EXIT(7, 17, 4),
    SCENE_EXITS(15),
    SCENE_EXIT(1, 22, 9),
    SCENE_EXIT(2, 20, 4),
    SCENE_EXIT(3, 16, 1),
    SCENE_EXIT(4, 20, 2),
    SCENE_EXITS(16),
    SCENE_EXIT(1, 15, 3),
    SCENE_EXIT(2, 22, 8),
    SCENE_EXITS(17),
    SCENE_EXIT(1, 19, 2),
    SCENE_EXIT(2, 14, 4),
    SCENE_EXIT(3, 14, 5),
    SCENE_EXIT(4, 14, 6),
    SCENE_EXITS(18),
    SCENE_EXIT(1, 21, 3),
    SCENE_EXIT(2, 21, 4),
    SCENE_EXITS(19),
    SCENE_EXIT(1, 14, 3),
    SCENE_EXIT(2, 17, 1),
    SCENE_EXITS(20),
    SCENE_EXIT(1, 14, 1),
    SCENE_EXIT(2, 15, 4),
    SCENE_EXIT(3, 14, 2),
    SCENE_EXIT(4, 15, 2),
    SCENE_EXITS(21),
    SCENE_EXIT(1, 2, 4),
    SCENE_EXIT(2, 22, 1),
    SCENE_EXIT(3, 18, 1),
    SCENE_EXIT(4, 18, 2),
    SCENE_EXITS(22),
    SCENE_EXIT(1, 21, 2),
    SCENE_EXIT(2, 22, 4),
    SCENE_EXIT(3, 22, 5),
    SCENE_EXIT(4, 22, 2),
    SCENE_EXIT(5, 22, 3),
    SCENE_EXIT(6, 22, 7),
    SCENE_EXIT(7, 22, 6),
    SCENE_EXIT(8, 16, 2),
    SCENE_EXIT(9, 15, 1),
    SCENE_EXIT(13, 22, 13),
    SCENE_EXIT(14, 22, 14),
    SCENE_EXITS_END,
};

struct ActorPlacement gKandoreanTemple_Placements[] = {
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_020057C0(gKandoreanTemple_LiftRoomPlacements, 407, 1, 2, 3, 4, 272, 5, 351)

struct ActorPlacement gKandoreanTemple_ChestbeaterRoomPlacements[] = {
    {368, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {368, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {450, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(504), FACING_SOUTH, 2, 0},
    {184, CONDITION_ALWAYS, 7, PIXELS(328), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {184, CONDITION_ALWAYS, 7, PIXELS(296), 0, PIXELS(140), FACING_SOUTH, 2, 0},
    {184, CONDITION_ALWAYS, 7, PIXELS(360), 0, PIXELS(140), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_VentRoomPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {351, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_WEST, 2, 2},
    {351, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_EAST, 2, 2},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_FogRoomPlacements[] = {
    {407, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 3},
    {91, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(340), FACING_SOUTH, 2, 0},
    {244, 0x4b, 1, PIXELS(632), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_GatePlacements[] = {
    {307, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(224), FACING_SOUTH, 2, 0},
    {91, CONDITION_ALWAYS, (s32)gKandoreanTemple_LookAroundScript, PIXELS(264), 0, PIXELS(424), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(424), FACING_SOUTH + FACING_STEP, 1, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(334), 0, PIXELS(317), FACING_SOUTHEAST + FACING_STEP, 2, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(284), 0, PIXELS(328), FACING_SOUTHEAST + FACING_STEP, 2, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(246), 0, PIXELS(318), FACING_SOUTH + FACING_STEP, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_PoisHallPlacements[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(676), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(676), FACING_SOUTH, 2, 1},
    {407, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(376), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(376), FACING_SOUTH, 2, 1},
    {21, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(664), FACING_SOUTH, 3, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(696), FACING_SOUTH, 2, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(696), FACING_SOUTH, 2, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(400), 0, PIXELS(712), FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gKandoreanTemple_LevitateHighScript, PIXELS(439), 0, PIXELS(672), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gKandoreanTemple_LevitateHighScript, PIXELS(359), 0, PIXELS(672), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gKandoreanTemple_LevitateLowScript, PIXELS(412), 0, PIXELS(324), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gKandoreanTemple_StandStillScript, PIXELS(90), 0, PIXELS(230), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gKandoreanTemple_StandStillScript, PIXELS(174), 0, PIXELS(72), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gKandoreanTemple_Events[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_LiftRoomEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {8, 0, CONDITION_ALWAYS, EVENT(KandoreanTemple_StartMovingBlock)},
    {9, 0, CONDITION_ALWAYS, EVENT(KandoreanTemple_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, CONDITION_ALWAYS, EVENT(KandoreanTemple_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT(KandoreanTemple_FinishMovingBlock)},
    {TRIGGER_STEP, 10, 0x211, EVENT(KandoreanTemple_SinkIntoTheSteam)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(KandoreanTemple_RideWestLiftUp)},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(KandoreanTemple_RideEastLiftUp)},
    {TRIGGER_TABLE_END},
};

s16 gKandoreanTemple_Room2Events[] = {
    1, 0, 1, -1, 1, 0, 49, 0,
    2, -1, 2, 0, 33, 0, 3, -1,
    3, 0, 49, 0, 4, -1, 4, 0,
    -1, -1, 0, 0, 0, 0,
};

struct Trigger gKandoreanTemple_ChestbeaterRoomEvents[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x304, EVENT(KandoreanTemple_StartMovingPot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x304, EVENT(KandoreanTemple_FinishMovingPot)},
    {0x602, 10, 0x304, EVENT(KandoreanTemple_TouchTheHotFloor)},
    {0xc602, 11, 0x304, EVENT(KandoreanTemple_TouchTheHotFloor)},
    {0x8602, 12, 0x304, EVENT(KandoreanTemple_TouchTheHotFloor)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(KandoreanTemple_ReadPoisSign)},
    {TRIGGER_STEP, 6, 0x92d, EVENT(KandoreanTemple_FightChestbeaters)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_VentRoomEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {8, 0, CONDITION_ALWAYS, EVENT(KandoreanTemple_StartMovingPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(KandoreanTemple_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(KandoreanTemple_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(KandoreanTemple_FinishMovingPillar)},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(KandoreanTemple_LeaveBySteamExit3)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(KandoreanTemple_LeaveBySteamExit4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_Room5Events[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gKandoreanTemple_Room6Events[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_PushThroughTheSteam)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_FogRoomEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x21, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x20f, 0},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(KandoreanTemple_FightFog)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_StepBehindThePole)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_StepOutFromThePole)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFieldItSeemsIMayNotHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldIsItReallyPossibleForAnyone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_GateEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x4602, 6, CONDITION_ALWAYS, EVENT(KandoreanTemple_SlipIntoTheCrack)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKandoreanTemple13))},
    {TRIGGER_STEP, 20, 0x230, EVENT(KandoreanTemple_KradenSuspectsTheWall)},
    {0x4602, 4, 0x212, EVENT(KandoreanTemple_OpenTheWellPassage)},
    {0xc602, 5, 0x212, EVENT(KandoreanTemple_OpenTheWellPassage)},
    {TRIGGER_EXAMINE, 10, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_TryTheLockedDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_FindTheHiddenPassage)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(KandoreanTemple_CycloneFirstPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(KandoreanTemple_CycloneSecondPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32,
     CONDITION_ALWAYS, EVENT(KandoreanTemple_CycloneThirdPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33,
     CONDITION_ALWAYS, EVENT(KandoreanTemple_CycloneFourthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 34,
     CONDITION_ALWAYS, EVENT(KandoreanTemple_CycloneFifthPatch)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(KandoreanTemple_AskAboutPoi)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_TellOfTheMeditation)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1033, 0x845,
     EVENT(KandoreanTemple_ReadTheGateStudent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgFieldIDoNotUnderstandMasterPoi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1034, 0x846,
     EVENT(KandoreanTemple_ReadTheGuard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldIWasHopingToHearMaster)},
    {TRIGGER_TALK, 11, 0x849, MSGNO(MsgFieldIHopeToEnterTheTemple)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldHeyWhatsAnOutsiderDoingOn)},
    {TRIGGER_TALK, 12, 0x849, MSGNO(MsgFieldPleaseDontBreakMyConcentrationI)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS,
     MSGNO(MsgFieldLooksLikeNoneOfOurBrothers)},
    {TRIGGER_TALK, 13, 0x849, MSGNO(MsgFieldIAmNotJustMeditatingI)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgFieldSomedayImGoingToBeA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x849,
     MSGNO(MsgFieldIStillCantBelieveThatThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldGahIKeptMyEyesShut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x849,
     MSGNO(MsgFieldMyMindIsEmptyMyBody)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgFieldIveBeenMeditatingAllDayBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x849,
     MSGNO(MsgFieldEmptyTheMindToLightenThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgFieldWheneverTheMasterGoesToTown)},
    {TRIGGER_TABLE_END},
};

// clang-format off
struct Trigger gKandoreanTemple_PoisHallEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {0x202, 8, CONDITION_ALWAYS, EVENT(KandoreanTemple_ClimbDownTheWell)},
    {0xc602, 9, CONDITION_ALWAYS, EVENT(KandoreanTemple_DropThroughTheTrapdoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x20f, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 10, 0x210, 0},
    {TRIGGER_TALK, 13, 0x849, MSGNO(MsgFieldHeWentInBeforeMeWhat)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(KandoreanTemple_AskWhoPassedTheTest)},
    {TRIGGER_TALK, 14, 0x849, MSGNO(MsgFieldEmptyMindEmptyBodyByClearing)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgFieldIHearThatSomeonePassedThe)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgFieldWhenIAwokeILearnedThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x849, MSGNO(MsgFieldTheresNoWayImRootingFor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgFieldTheMasterSeemedSoDisappointedThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x849, MSGNO(MsgFieldArrghIWasSoCloseI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgFieldIKnewItItShouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgFieldBeforeIPassedOutISaw)},
    {TRIGGER_EXAMINE, 16, 0x84a, EVENT(KandoreanTemple_ReceiveLash)},
    {TRIGGER_TALK, 12, 0x84a, MSGNO(MsgFieldTakeThePowerOfLashIt)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouWishToPracticeThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgFieldHoHoIFeelTheStrength)},
    {TRIGGER_TABLE_END},
};
// clang-format on
#undef KandoreanTemple_MeditateRestlessly
#undef KandoreanTemple_LevitateStudentBob
#undef KandoreanTemple_PlaceLevitatingStudent
