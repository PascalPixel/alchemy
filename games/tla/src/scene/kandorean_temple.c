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

// ---- scene/kandorean_temple/kandorean_temple.c ----

/// The Kandorean Temple's rooms: their tables' getters, the steam, vents and
/// lifts.

#include "flags.h"
#include "kandorean_temple.h"
#include "field.h"
#include "module/scene/kandorean_temple.h"
#include "module/scene/scene.h"
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

#include "battle.h"

#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

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

#include "event_inline.h"
#include "macros.h"
#include "actor.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "version.h"
#include "task.h"

extern u32 gKandoreanTemple_Placements[];
extern u32 gKandoreanTemple_LiftRoomPlacements[];
extern u32 gKandoreanTemple_ChestbeaterRoomPlacements[];
extern u32 gKandoreanTemple_VentRoomPlacements[];
extern u32 gKandoreanTemple_FogRoomPlacements[];

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

extern s32 gKandoreanTemple_Events[];
extern s32 gKandoreanTemple_LiftRoomEvents[];
extern s32 gKandoreanTemple_ChestbeaterRoomEvents[];
extern s32 gKandoreanTemple_VentRoomEvents[];
extern s32 gKandoreanTemple_Room5Events[];
extern s32 gKandoreanTemple_Room6Events[];
extern s32 gKandoreanTemple_FogRoomEvents[];
extern s32 gKandoreanTemple_GateEvents[];
extern s32 gKandoreanTemple_PoisHallEvents[];

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
 * games/tla/src/scene/kandorean_temple.c:298: dereferencing pointer to
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
 * games/tla/src/scene/kandorean_temple.c:1135: arithmetic on pointer to an
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
  s32 t1, t2, t9, t19, t21, t39, t44, p_r3;

  t1 = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
  if (t1 == 0)
    goto L_0200acd2;
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
L_0200acd2:
  t19 = GameFlag_Get(0x300);
  if (t19 == 0)
    goto L_0200acee;
  Actor_SetPosition(8, PIXELS(216), PIXELS(216));
  goto L_0200ad0a;
L_0200acee:
  t21 = GameFlag_Get(0x301);
  if (t21 == 0)
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
  t39 = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
  if (t39 != 0)
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
  t44 = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
  if (t44 != 0)
    goto L_0200ae24;
  if (gMainRoutineWork.entrance != 5)
    goto L_0200ae24;
  KandoreanTemple_LandFromAbove();
L_0200ae24:
  Task_AddFrame(KandoreanTemple_WatchVentPlatforms, 0x480);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  FieldEffect_StartBlankOverlay(0);
}
