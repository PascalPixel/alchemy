/// @file
/// The Kibombo Mountains, the pass the Kibombo warriors guard on the way to
/// their village, in the rooms of this overlay: crates the party pushes and
/// vines it climbs while it keeps out of the guards' sight, a guard who sees
/// it sending it back, a guard dog and its bone, and the Jupiter Djinni Waft
/// (KibomboMountains_FightWaft).

#include "flags.h"
#include "item_ids.h"
#include "field/field.h"
#include "types.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "field_event.h"
#include "scene_ids.h"
#include "system/iwram_call.h"
#include "call.h"
#include "system/ram_buffer.h"
#include "version.h"
#include "shared/tables.h"
#include "objcmd.h"

void Func_02000da8(s32 value);
void Func_02000e28(s32 value);

void Func_0200036c(void);

void Func_02000438(void);

void Func_02000440(void);

void Func_02000478(void);

void Func_02000960(void);

void Func_02000ea8(void);

void Func_02000ef4(void);

void Func_02000f40(void);

void Func_02000f4c(void);

void Func_02000f58(void);

void Func_02000f78(void);
void Func_02000f84(void);
void Func_02000f90(void);
void Func_02000fdc(void);
void Func_02000ff4(void);

extern u32 gData_020011a4[];
extern u32 gData_02001284[];
extern u32 gData_020012ac[];
extern u32 gData_02001334[];
extern u32 gData_02001410[];
extern u32 gData_020017f8[];

void Func_02000fc4(void);

void Rope_Place(s32 a, s32 b, s32 c, s32 d);

#include "text/msg_ids.h"

void Field_TryJumpForward(void);

void KibomboMountains_FightWaft(void) { Field_RequestDjinnBattle(18, 70); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "shared/actor_bob.h"

extern struct ActorPlacement gKibomboMountains_Placements[];
extern struct ActorPlacement gKibomboMountains_Room1Placements[];
extern struct ActorPlacement gKibomboMountains_Room2Placements[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_KibomboMountains1)
    return gKibomboMountains_Room1Placements;
  if (scene == (s32)&SceneId_KibomboMountains2)
    return gKibomboMountains_Room2Placements;
  return gKibomboMountains_Placements;
}

void Pillar_SetMoved(void) {
  struct Actor *object = Actor_Get(11);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  MapGrid_CopyCellAttributes(24, 32, 1, 1, 22, 32);
  Event_SetFlag(0x221);
}

void Func_02000124(void) {
  MapGrid_CopyCellAttributes(30, 35, 1, 1, 28, 34);
  MapGrid_CopyCellAttributes(30, 34, 1, 1, 28, 33);
  Event_SetFlag(FLAG_SCENE_TEMP_22);
}

#include "shared/actor_wait_to_land.h"

void Func_02000190(s32 unused, s32 id) {
  if (id == 15)
    FieldActor_SetSpritePriority(15, 1);
}

extern s32 gData_02001404[];

void Actor_StartScript(struct Actor *object, const u8 *script);
void Actor_StartIdle(struct Actor *object);

void Func_020001a0(s32 unused, s32 id) {
  struct Actor saved;

  if (id == 14) {
    struct Actor *block = Actor_GetById(14);
    struct Actor *other;
    s32 x;
    s32 z;
    s32 cell;

    if (!Event_GetFlag(FLAG_SCENE_TEMP_23) && (x = block->x >> 20) == 47 &&
        (z = block->z >> 20) == 28) {
      Actor_WaitToLand(14);
      MapGrid_CopyCellAttributes(49, 28, 1, 1, x, z);
      Event_SetFlag(FLAG_SCENE_TEMP_23);
      Actor_SetSpritePriority(14, 2);
      other = Actor_GetById(10);
      cell = other->z >> 20;
      if (cell == 27) {
        Sound_Play(SE_VOICE_ANIMAL_2);
        other->z = 220 << 17;
      } else if (cell == 28 || cell == 29) {
        Sound_Play(SE_VOICE_ANIMAL_3);
        other->z = 236 << 17;
      }
      if (block->z < other->z)
        Event_SetFlag(FLAG_SCENE_TEMP_01);
      else
        Event_SetFlag(FLAG_SCENE_TEMP_03);
    } else {
      Actor_SetSpritePriority(id, 1);
    }
  }
  if (id == 15) {
    struct Actor *block = Actor_GetById(15);
    struct Actor *other;

    if (!Event_GetFlag(FLAG_SCENE_TEMP_24) && block->x >> 20 == 53 &&
        block->z >> 20 == 36) {
      s32 dx;

      Actor_WaitToLand(15);
      Actor_SetSpritePriority(15, 2);
      other = Actor_GetById(9);
      dx = block->x - other->x;
      if (dx >= 0 ? dx < 0x80000 : other->x - block->x < px(8)) {
        other->motionFlags = 0;
        other->speedLimit = px(1);
        FieldActor_SetMoveTargetFixed(other, block->x, block->y, block->z);
        FieldActor_WaitForMove(other);
        Actor_StartScript(block, gData_02001404);
        Actor_StartIdle(other);
        other->x = 0;
        other->z = 0;
        Event_SetFlag(FLAG_SCENE_TEMP_00);
      } else {
        Iwram_CopyWords((s32)&saved, (s32)other, 128);
        Actor_ResetTargetsAndVelocity(9);
        FieldActor_Jump(9, 4, 0);
        FieldEvent_Wait(30);
        Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        Actor_FaceActor(9, 15, 0);
        FieldEvent_Wait(60);
        Actor_SetAnimationAndWait(9, 4);
        FieldEvent_Wait(20);
        {
          void *from = &saved;
          Iwram_CopyWordsEntry(other, from, 128);
        }
        MapGrid_CopyCellAttributes(53, 35, 1, 1, 53, 36);
        Event_SetFlag(FLAG_SCENE_TEMP_24);
      }
    }
  }
}

void Func_0200036c(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetDestination(17, 764, 384);
  Actor_MoveToAndWait(16, 746, 380);
  Actor_Get(17)->velocityY = px(4);
  FieldActor_WaitForMoveById(17);
  Actor_Get(17)->priorityFlags |= 2;
  if (!Event_GetFlag(0x8ff) && !Event_GetFlag(FLAG_SCENE_TEMP_01)) {
    FieldActor_Stop(10);
    Actor_MoveToAndWait(10, 764, 388);
    Actor_SetDestination(17, 764, 376);
    Actor_WalkToAndWait(10, 764, 380);
    Actor_SetAnimation(10, 4);
    Actor_SetAttachedEffect(10,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Event_SetFlag(FLAG_SCENE_TEMP_02);
  }
  Event_End();
}

void Func_02000428(void) { GameFlag_Set(0x882); }

void Func_02000438(void) { Field_TryJumpForward(); }

void Func_02000440(void) {
  struct Actor *object = Actor_Get(18);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  MapGrid_CopyCellAttributes(6, 16, 1, 1, 4, 16);
  Event_SetFlag(0x210);
}

void Func_02000478(void) {
  struct Actor *object = Actor_Get(19);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  MapGrid_CopyCellAttributes(8, 30, 1, 1, 10, 30);
  Event_SetFlag(0x211);
}

void Func_020004b0(s32 unused, s32 cell) {
  if (cell == 16 || cell == 17)
    MapGrid_CopyCellAttributes(120, 33, 8, 2, 20, 33);
  if (cell == 14 || cell == 15)
    MapGrid_CopyCellAttributes(112, 15, 16, 4, 8, 15);
}

void Func_020004f4(s32 id, s32 a, s32 b) {
  struct Actor *object = Actor_Get(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  MapGrid_CopyCellAttributes(a, b, 1, 1, x, z);
}

void Func_0200051c(s32 unused, s32 cell) {
  if (cell == 20) {
    struct Actor *object = Actor_Get(20);

    if (!Event_GetFlag(0x8e7)) {
      s32 x = object->x >> 20;

      if (x == 4) {
        Actor_WaitToLand(20);
        MapGrid_CopyCellAttributes(3, 28, 1, 1, x, 28);
        Event_SetFlag(0x8e7);
      }
    }
  }
  if (cell == 13) {
    struct Actor *object = Actor_Get(13);

    if (!Event_GetFlag(0x8e8)) {
      s32 x = object->x >> 20;

      if (x == 28) {
        s32 z = object->z >> 20;

        if (z == 30) {
          Actor_WaitToLand(13);
          MapGrid_CopyCellAttributes(32, 30, 1, 2, x, z);
          Event_SetFlag(0x8e8);
        }
      }
    }
  }
  if (cell == 14 || cell == 15) {
    Func_020004f4(14, 8, 17);
    Func_020004f4(15, 8, 17);
  }
  if (cell == 16 || cell == 17) {
    Func_020004f4(16, 22, 33);
    Func_020004f4(17, 22, 33);
  }
}

extern struct Trigger gKibomboMountains_Triggers[];
extern struct Trigger gKibomboMountains_Room1Triggers[];
extern struct Trigger gKibomboMountains_Room2Triggers[];
extern s16 gKibomboMountains_Room3Triggers[];

const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_KibomboMountains1)
    return gKibomboMountains_Room1Triggers;
  if (scene == (s32)&SceneId_KibomboMountains2)
    return gKibomboMountains_Room2Triggers;
  if (scene == (s32)&SceneId_KibomboMountains3)
    return gKibomboMountains_Room3Triggers;
  return gKibomboMountains_Triggers;
}

void Func_02000af0(void);
void Rope_Initialize(void);

#if !TLA_PATCH_1
#define EVENT_WORK_VALUE 0x201
#else
#define EVENT_WORK_VALUE 0x204
#endif

s32 Scene_Initialize(void) {
  struct Actor *object;
  s16 scene;
  s32 flag;
  s32 height;

  gFieldWork->transition = EVENT_WORK_VALUE;
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_KibomboMountains1) {
    Actor_SetSpritePriority(18, 1);
    Actor_SetSpritePriority(19, 1);
    Actor_SetSpriteFlags(Actor_GetById(19), 0);
    Actor_GetById(19)->collisionFlags = 0;
    Actor_GetById(19)->priorityFlags |= 2;
    Actor_GetById(12)->collisionFlags = 0;
    if (GameFlag_Get(FLAG_SCENE_TEMP_22)) {
      Map_CopyCellAttributeRect(30, 35, 1, 1, 28, 34);
      Map_CopyCellAttributeRect(30, 34, 1, 1, 28, 33);
    }
    if (GameFlag_Get(FLAG_SCENE_TEMP_21))
      MapGrid_CopyCellAttributes(24, 32, 1, 1, 22, 32);
    if (GameFlag_Get(FLAG_SCENE_TEMP_23))
      MapGrid_CopyCellAttributes(49, 28, 1, 1, 47, 28);
    else
      Actor_SetSpritePriority(14, 1);
    if (GameFlag_Get(FLAG_SCENE_TEMP_24))
      MapGrid_CopyCellAttributes(53, 35, 1, 1, 53, 36);
    flag = GameFlag_Get(FLAG_SCENE_TEMP_02);
    if (flag == 0) {
      struct Actor *lift;

      Actor_GetById(16)->priorityFlags |= 0x80;
      lift = Actor_GetById(17);
      lift->priorityFlags |= 0x40;
      lift->y = px(224);
      lift->terrainHeight = px(224);
      lift->collisionFlags = flag;
    } else {
      Actor_SetAttachedEffect(10, ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ATTACH);
    }
    Actor_ShowItemIconById(17, ITEM_BONE);
    if (GameFlag_Get(0x882))
      Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
    else
      Actor_GetById(13)->scaleY = Q16_ONE * 5 / 4;
  } else if (scene == (s32)&SceneId_KibomboMountains2) {
    Actor_SetSpritePriority(8, 1);
    Actor_SetSpritePriority(9, 1);
    Rope_Initialize();
    Rope_Place(0, 0x9e8, 8, 9);
    if (GameFlag_Get(FLAG_SCENE_TEMP_10))
      MapGrid_CopyCellAttributes(6, 16, 1, 1, 4, 16);
    if (GameFlag_Get(FLAG_SCENE_TEMP_11))
      MapGrid_CopyCellAttributes(8, 30, 1, 1, 10, 30);
    if (GameFlag_Get(0x8e7)) {
      object = Actor_GetById(20);
      Actor_SetPosition(20, 72, 456);
      object->motionFlags = 0;
      height = Map_GetTerrainHeight(0, object->x, object->z);
      object->terrainHeight = height;
      object->y = height;
      MapGrid_CopyCellAttributes(3, 28, 1, 1, 4, 28);
    }
    if (GameFlag_Get(0x8e8)) {
      object = Actor_GetById(13);
      Actor_SetPosition(13, 456, 488);
      object->motionFlags = 0;
      height = Map_GetTerrainHeight(0, object->x, object->z);
      object->terrainHeight = height;
      object->y = height;
      MapGrid_CopyCellAttributes(32, 30, 1, 2, 28, 30);
    }
    MapGrid_CopyCellAttributes(8, 15, 16, 4, 112, 15);
    MapGrid_CopyCellAttributes(20, 33, 8, 2, 120, 33);
    Func_020004f4(14, 8, 17);
    Func_020004f4(15, 8, 17);
    Func_020004f4(16, 22, 33);
    Func_020004f4(17, 22, 33);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance == 8)
      Func_02000af0();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000960(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgIsTheWay);
  Actor_SetPosition(27, 312, 304);
  Event_ShowMessage(27, 0);
  FieldCamera_MoveTo(256, CAMERA_KEEP, 392, 1);
  FieldCamera_WaitForArrival();
  Actor_ResetTargetsAndVelocity(11);
  Actor_FaceActor(11, 27, 0);
  Actor_WalkToAndWait(27, 264, 312);
  Actor_WalkToAndWait(27, 256, 400);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(11, 0);
  Actor_RunRepeatedMotion(11, 1);
  Event_ShowMessage(11, 0);
  Actor_SetAnimationAndWait(27, 4);
  Event_ShowMessage(27, 0);
  Event_ShowMessage(27, 0);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessage(27, 0);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessage(27, 0);
  Actor_WalkToAndWait(27, 264, 312);
  Actor_WalkTo(27, 360, 312);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(11, 0);
  FieldActor_WatchAndSetScript(11, gMainRoutineWork.currentOwner);
  System_Wait(30);
  Event_ShowMessage(11, 0);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(60);
  Actor_SetPosition(27, 0, 0);
  Actor_SetScript(11, gData_020017f8);
  Event_End();
  Event_SetFlag(0x880);
}

void Func_02000ad4(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SelectAnimation(object, 1);
  object->acceleration = px(1) / 4;
  object->speedLimit = px(1) / 2;
}

void Func_02000af0(void) {
  s32 owner = gMainRoutineWork.currentOwner;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(23, 248, 472);
  Actor_SetPosition(24, 264, 472);
  Actor_SetPosition(25, 235, 483);
  Actor_SetPosition(26, 232, 496);
  Actor_SetPosition(27, 277, 483);
  Actor_SetPosition(28, 280, 496);
  Func_02000ad4(owner);
  Func_02000ad4(23);
  Func_02000ad4(24);
  Func_02000ad4(25);
  Func_02000ad4(26);
  Func_02000ad4(27);
  Func_02000ad4(28);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgHereTheIntruder);
  Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  Event_ShowMessage(27, 0);
  Actor_FaceActor(27, 25, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(25, 3);
  Actor_FaceActor(27, owner, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessage(27, 0);
  Actor_SetAnimationAndWait(27, 3);
  Event_ShowMessage(27, 0);
  Actor_FaceActor(25, 23, 0);
  Actor_ShowEmote(25, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessage(25, 0);
  Actor_WalkTo(23, 248, 536);
  FieldEvent_Wait(1);
  Actor_WalkTo(24, 264, 536);
  FieldActor_WatchAndSetScript(25, owner);
  FieldActor_WatchAndSetScript(26, owner);
  FieldActor_WatchAndSetScript(27, owner);
  FieldActor_WatchAndSetScript(28, owner);
  FieldEvent_Wait(2);
  Actor_Get(owner)->actionFlags &= ~1;
  Actor_WalkToAndWait(owner, 256, 552);
  FieldEvent_Wait(1);
  Actor_Get(owner)->actionFlags |= 1;
  Actor_SetAnimation(23, 1);
  Actor_SetAnimation(24, 1);
  FieldEvent_Wait(20);
  Actor_SetAnimation(23, 4);
  FieldEvent_Wait(3);
  Actor_SetAnimationAndWait(24, 4);
  FieldEvent_Wait(20);
  Actor_WalkTo(23, 248, 488);
  FieldEvent_Wait(2);
  Actor_WalkToAndWait(24, 264, 488);
  Actor_WalkTo(23, 248, 440);
  Actor_WalkTo(24, 264, 440);
  Actor_WalkTo(25, 232, 456);
  Actor_WalkTo(26, 232, 472);
  Actor_WalkTo(27, 280, 456);
  Actor_WalkToAndWait(28, 280, 472);
  Actor_SetPosition(23, 0, 0);
  Actor_SetPosition(24, 0, 0);
  Actor_SetPosition(25, 0, 0);
  Actor_SetPosition(26, 0, 0);
  Actor_SetPosition(27, 0, 0);
  Actor_SetPosition(28, 0, 0);
  Event_End();
  Event_SetFlag(FLAG_AREA_TEMP_00);
}

void Func_02000d80(void) {
  s32 moved = Event_GetFlag(FLAG_AREA_TEMP_00);
  s32 scene = (s32)&SceneId_KibomboMountains2;

  if (moved)
    Event_SetDestination(scene, 9);
  else
    Event_SetDestination(scene, 8);
}

void Func_02000da8(s32 id) {
  s32 owner = gMainRoutineWork.currentOwner;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_ResetTargetsAndVelocity(id);
  Actor_ShowEmote(id, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceActor(id, owner, 20);
  FieldEvent_SetMessageId(MsgGuardsAnIntruder);
  Event_ShowMessage(id, 0);
  Actor_SetAttachedEffect(owner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(id, 4);
  FieldEvent_Wait(30);
  Event_ShowMessage(id, 0);
  Actor_SetAttachedEffect(owner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Func_02000d80();
  Event_End();
}

void Func_02000e28(s32 id) {
  s32 owner = gMainRoutineWork.currentOwner;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(id);
  Actor_ShowEmote(id, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceActor(id, owner, 20);
  FieldEvent_SetMessageId(MsgGrrrrrRrrruffRrufff);
  Event_ShowMessage(id, 0);
  Actor_SetAttachedEffect(owner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(id, 4);
  FieldEvent_Wait(30);
  Event_ShowMessage(8, 0);
  Actor_SetAttachedEffect(owner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Func_02000d80();
  Event_End();
}

void Func_02000ea8(void) {
  struct Actor *owner = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *guard = Actor_Get(14);

  if ((guard->z - owner->z >= 0 ? guard->z - owner->z : owner->z - guard->z) <
          0x100000 &&
      guard->x > owner->x)
    return;
  Func_02000da8(10);
}

void Func_02000ef4(void) {
  struct Actor *owner = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *guard = Actor_Get(15);

  if ((guard->z - owner->z >= 0 ? guard->z - owner->z : owner->z - guard->z) <
          0x100000 &&
      guard->x < owner->x)
    return;
  Func_02000da8(10);
}

void Func_02000f40(void) { Func_02000da8(11); }

void Func_02000f4c(void) { Func_02000da8(11); }

void Func_02000f58(void) {
  if (gMainRoutineWork.renderMode != 3)
    Func_02000da8(12);
}

void Func_02000f78(void) { Func_02000da8(12); }

void Func_02000f84(void) { Func_02000da8(8); }

void Func_02000f90(void) { Func_02000da8(8); }

void Func_02000f9c(void) {
  if (!Event_GetFlag(FLAG_SCENE_TEMP_02) && !Event_GetFlag(FLAG_SCENE_TEMP_01))
    Func_02000e28(10);
}

void Func_02000fc4(void) {
  if (!Event_GetFlag(FLAG_SCENE_TEMP_00))
    Func_02000da8(9);
}

void Func_02000fdc(void) {
  if (!Event_GetFlag(FLAG_SCENE_TEMP_00))
    Func_02000da8(9);
}

void Func_02000ff4(void) {
  if (!Event_GetFlag(FLAG_SCENE_TEMP_03) && !Event_GetFlag(FLAG_SCENE_TEMP_02))
    Func_02000e28(10);
}

#undef EVENT_WORK_VALUE
#include "text/msg_ids.h"

void KibomboMountains_FightWaft(void);
void Pillar_SetMoved(void);
void Func_02000124(void);
void Func_02000428(void);
void Func_02000f9c(void);

extern u32 gData_02001728[];

// clang-format off
u32 gData_020011a4[] = {
    SC_ARG(2),
    SC_END,
    SC_ARG(0),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, px(24)),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(KibomboMountains1),
    SCENE_EXIT_UNLESS(1, WorldMap, 26, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, KibomboMountains2, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, KibomboMountains2, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, KibomboMountains2, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, KibomboMountains3, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, KibomboMountains3, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, KibomboMountains3, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(KibomboMountains2),
    SCENE_EXIT_UNLESS(1, WorldMap, 25, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, KibomboMountains1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, KibomboMountains1, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, KibomboMountains1, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(KibomboMountains3),
    SCENE_EXIT_UNLESS(1, KibomboMountains1, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, KibomboMountains1, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, KibomboMountains1, 7, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

u32 gData_02001284[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

struct ActorPlacement gKibomboMountains_Placements[] = {
    {SCENE_TABLE_END},
};

u32 gData_020012ac[] = {
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x1999),
    SC_SET(SW_COLLISION, 129),
    SC_ARG(1),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_ARG(4),
    SC_WAIT(90),
    SC_ARG(1),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(4),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gData_02001334[] = {                                                               
      SC_SET(SW_SPEED, 0x8000),                                                
      SC_SET(SW_ACCEL, 0x1999),                                                
      SC_SET(SW_COLLISION, 129),                                               
      SC_ARG(2),                                                               
      SC_MOVE(px(856), 0, px(584)),                                      
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_WAIT(3),                                                              
      SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),                       
      SC_WAIT(30),                                                             
      SC_ARG(4),                                                               
      SC_WAIT(90),                                                             
      SC_ARG(2),                                                               
      SC_MOVE(px(792), 0, px(584)),                                      
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_WAIT(3),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(30),                                                             
      SC_ARG(4),                                                               
      SC_WAIT(90),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

s32 gData_02001404[] = {
    SC_CALL(Actor_Bob),
    SC_END,
};

u32 gData_02001410[] = {
    SC_SET(SW_SPEED, 0x14000),
    SC_SET(SW_ACCEL, 0x4000),
    SC_SET(SW_COLLISION, 129),
    SC_ARG(3),
    SC_SET(SW_VELOCITY_Y, px(3)),
    SC_MOVE(px(760), 0, px(488)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(px(764), 0, px(472)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(px(760), 0, px(456)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(px(764), 0, px(440)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(px(760), 0, px(424)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(px(764), 0, px(408)),
    SC_SYNC,
    SC_MOVE(px(760), 0, px(392)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(3)),
    SC_MOVE(px(764), 0, px(408)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(px(760), 0, px(424)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(px(764), 0, px(440)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(px(760), 0, px(456)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(px(764), 0, px(472)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, px(2)),
    SC_MOVE(px(760), 0, px(488)),
    SC_SYNC,
    SC_MOVE(px(764), 0, px(504)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct ActorPlacement gKibomboMountains_Room1Placements[] = {
    ACTOR_PLACEMENT(83, 0x8ff, (s32)gData_020012ac, 616, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, 0x8ff, (s32)gData_02001334, 824, 0, 584, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(189, 0x8ff, (s32)gData_02001410, 760, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 520, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(299, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, (s32)gData_02001284, 712, 0, 472, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, (s32)gData_02001284, 888, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(288, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 732, 0, 376, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 732, 0, 376, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(246, 0x76, MOVEMENT_TYPE_STAND, 440, 0, 472, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 584, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

u32 gData_02001728[] = {                                                               
      SC_SET(SW_SPEED, 0x8000),                                                
      SC_SET(SW_ACCEL, 0x1999),                                                
      SC_SET(SW_COLLISION, 129),                                               
      SC_ARG(2),                                                               
      SC_MOVE(px(184), 0, px(296)),                                      
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_WAIT(3),                                                              
      SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),                       
      SC_WAIT(30),                                                             
      SC_ARG(4),                                                               
      SC_WAIT(90),                                                             
      SC_ARG(2),                                                               
      SC_MOVE(px(248), 0, px(296)),                                      
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_WAIT(3),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(30),                                                             
      SC_ARG(4),                                                               
      SC_WAIT(90),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

u32 gData_020017f8[] = {                                                               
      SC_SET(SW_SPEED, 0x8000),                                                
      SC_SET(SW_ACCEL, 0x1999),                                                
      SC_SET(SW_COLLISION, 129),                                               
      SC_ARG(2),                                                               
      SC_MOVE(px(248), 0, px(448)),                                      
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_WAIT(3),                                                              
      SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),                       
      SC_WAIT(30),                                                             
      SC_ARG(4),                                                               
      SC_WAIT(90),                                                             
      SC_ARG(2),                                                               
      SC_MOVE(px(264), 0, px(448)),                                      
      SC_SYNC,                                                                 
      SC_ARG(1),                                                               
      SC_WAIT(3),                                                              
      SC_SET(SW_FACING, FACING_SOUTH),                                         
      SC_WAIT(30),                                                             
      SC_ARG(4),                                                               
      SC_WAIT(90),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

struct ActorPlacement gKibomboMountains_Room2Placements[] = {
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, 0x8ff, (s32)gData_02001728, 200, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, 0x8ff, (s32)gData_020017f8, 248, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, 0x8ff, (s32)gData_020012ac, 424, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(250, CONDITION_ALWAYS, (s32)gData_02001284, 456, 0, 472, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, (s32)gData_02001284, 136, 0, 296, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, (s32)gData_02001284, 312, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, (s32)gData_02001284, 328, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, (s32)gData_02001284, 440, 0, 536, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 264, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 488, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, (s32)gData_02001284, 88, 0, 456, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(305, CONDITION_ALWAYS, (s32)gData_020011a4, 232, 0, 536, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(305, CONDITION_ALWAYS, (s32)gData_020011a4, 280, 0, 536, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, (s32)gData_020012ac, 0, 0, 0, 0x3f00, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, (s32)gData_02001334, 0, 0, 0, 0x4100, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, (s32)gData_020012ac, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, (s32)gData_02001334, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, (s32)gData_020012ac, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, (s32)gData_02001334, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gKibomboMountains_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gKibomboMountains_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 6, IF_FLAG_SET(0x882), 6},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_STEP, 20, 0x8ff, EVENT(Func_02000f84)},
    {TRIGGER_STEP, 21, 0x8ff, EVENT(Func_02000f90)},
    {TRIGGER_STEP, 22, 0x8ff, EVENT(Func_02000f9c)},
    {TRIGGER_STEP, 23, 0x8ff, EVENT(Func_02000fc4)},
    {TRIGGER_STEP, 24, 0x8ff, EVENT(Func_02000fdc)},
    {TRIGGER_STEP, 25, 0x8ff, EVENT(Func_02000ff4)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 33, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02000190)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_020001a0)},
    {TRIGGER_TALK, 10, IF_FLAG_SET(0x202), MSGNO(MsgSniffSniffLick)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x202), MSGNO(MsgABoneThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgThisRockIs)},
    {TRIGGER_TALK, 15, IF_FLAG_SET(0x200), MSGNO(MsgHeyWhoS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(0x200), MSGNO(MsgItSDark)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(KibomboMountains_FightWaft)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x221, EVENT(Pillar_SetMoved)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 12, 0x222, EVENT(Func_02000124)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, EVENT(Func_02000428)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(Func_020001a0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, 0x200, EVENT(Func_02000190)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, 0x200, EVENT(Func_020001a0)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), -14320, 0x202, EVENT(Func_0200036c)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 2064, 0x202, EVENT(Func_0200036c)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibomboMountains_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 30, 0x8ff, EVENT(Func_02000ea8)},
    {TRIGGER_STEP, 31, 0x8ff, EVENT(Func_02000ef4)},
    {TRIGGER_STEP, 32, 0x8ff, EVENT(Func_02000f40)},
    {TRIGGER_STEP, 33, 0x8ff, EVENT(Func_02000f4c)},
    {TRIGGER_STEP, 34, 0x880, EVENT(Func_02000960)},
    {TRIGGER_STEP, 35, 0x8ff, EVENT(Func_02000f58)},
    {TRIGGER_STEP, 36, 0x8ff, EVENT(Func_02000f78)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_020004b0)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_0200051c)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x9e8, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, 0x210, EVENT(Func_02000440)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 19, 0x211, EVENT(Func_02000478)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 21, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 22, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, EVENT(Func_0200051c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 20, 0x8e7, EVENT(Func_0200051c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14, CONDITION_ALWAYS, EVENT(Func_020004b0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, EVENT(Func_020004b0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, CONDITION_ALWAYS, EVENT(Func_020004b0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(Func_020004b0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(Func_0200051c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(Func_0200051c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, CONDITION_ALWAYS, EVENT(Func_0200051c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(Func_0200051c)},
    {TRIGGER_TABLE_END},
};

s16 gKibomboMountains_Room3Triggers[] = {
    1, 0, 1, -1, 1, 0, 1, 0,
    2, -1, 2, 0, 1, 0, 3, -1,
    3, 0, -1, -1, 0, 0, 0, 0,
};
// clang-format on
