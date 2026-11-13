/// @file
/// The scene code of overlay 0x66f, in ROM order.

#include "field/field.h"

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_0200114c(void);
void Func_0200103c(struct Actor *object, s32 value);
void Func_02000da8(s32 value);
void Func_02000e28(s32 value);

void Func_02000038(void) { Func_080d2b4c(18, 70); }

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "field_event.h"

s32 Math_Cosine(s32 angle);

s32 Func_02000058(struct Actor *object) {
  s32 step = object->riseCounter & 63;
  struct FieldSprite *sprite = object->sprite;

  object->motionFlags = 3;
  if (step == 0)
    object->velocityY = PIXELS(3);
  if (step == 16)
    object->velocityY = PIXELS(2);
  if (step == 24)
    object->velocityY = PIXELS(3);
  sprite->rotation = Math_Cosine(step << 12) / 64;
  object->riseCounter++;
  return -1;
}

#include "scene_ids.h"

extern const u8 gData_02001294[];
extern const u8 gData_020015f0[];
extern const u8 gData_020018c8[];

const u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov66fRoom1)
    return gData_020015f0;
  if (scene == (s32)&SceneId_Ov66fRoom2)
    return gData_020018c8;
  return gData_02001294;
}

void Func_020000ec(void) {
  struct Actor *object = Actor_Get(11);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  MapGrid_CopyCellAttributes(24, 32, 1, 1, 22, 32);
  Event_SetFlag(0x221);
}

void Func_02000124(void) {
  MapGrid_CopyCellAttributes(30, 35, 1, 1, 28, 34);
  MapGrid_CopyCellAttributes(30, 34, 1, 1, 28, 33);
  Event_SetFlag(0x222);
}

void Func_0200015c(s32 id) {
  s32 n = 60;
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 3;
  while (n != 0) {
    System_Wait(1);
    if (object->velocityY == 0)
      break;
    n--;
  }
  System_Wait(10);
  object->motionFlags = 0;
}

void Func_02000190(s32 unused, s32 id) {
  if (id == 15)
    FieldActor_SetSpritePriority(15, 1);
}

#include "iwram_call.h"
#include "call.h"

extern const u8 gData_02001404[];

void Actor_ResetTargetsAndVelocity(s32 actor);
void Actor_StartScript(struct Actor *object, const u8 *script);
void Func_0200104c(struct Actor *object);

void Func_020001a0(s32 unused, s32 id) {
  struct Actor saved;

  if (id == 14) {
    struct Actor *block = Actor_GetById(14);
    struct Actor *other;
    s32 x;
    s32 z;
    s32 cell;

    if (!Event_GetFlag(0x223) && (x = block->x >> 20) == 47 &&
        (z = block->z >> 20) == 28) {
      Func_0200015c(14);
      MapGrid_CopyCellAttributes(49, 28, 1, 1, x, z);
      Event_SetFlag(0x223);
      Actor_SetSpritePriority(14, 2);
      other = Actor_GetById(10);
      cell = other->z >> 20;
      if (cell == 27) {
        Sound_Play(257);
        other->z = 220 << 17;
      } else if (cell == 28 || cell == 29) {
        Sound_Play(258);
        other->z = 236 << 17;
      }
      if (block->z < other->z)
        Event_SetFlag(0x201);
      else
        Event_SetFlag(0x203);
    } else {
      Actor_SetSpritePriority(id, 1);
    }
  }
  if (id == 15) {
    struct Actor *block = Actor_GetById(15);
    struct Actor *other;

    if (!Event_GetFlag(0x224) && block->x >> 20 == 53 && block->z >> 20 == 36) {
      s32 dx;

      Func_0200015c(15);
      Actor_SetSpritePriority(15, 2);
      other = Actor_GetById(9);
      dx = block->x - other->x;
      if (dx >= 0 ? dx < 0x80000 : other->x - block->x < 0x80000) {
        other->motionFlags = 0;
        other->speedLimit = 0x10000;
        FieldActor_SetMoveTarget(other, block->x, block->y, block->z);
        Actor_WaitForMove(other);
        Actor_StartScript(block, gData_02001404);
        Func_0200104c(other);
        other->x = 0;
        other->z = 0;
        Event_SetFlag(0x200);
      } else {
        Call3((void (*)())Iwram_CopyWords, (s32)&saved, (s32)other, 128);
        Actor_ResetTargetsAndVelocity(9);
        FieldActor_Jump(9, 4, 0);
        FieldEvent_Wait(30);
        Actor_ShowEmote(9, 257, 0);
        Actor_FaceActor(9, 15, 0);
        FieldEvent_Wait(60);
        Actor_SetAnimationAndWait(9, 4);
        FieldEvent_Wait(20);
        { void *from = &saved; Iwram_CopyWords(other, from, 128); }
        MapGrid_CopyCellAttributes(53, 35, 1, 1, 53, 36);
        Event_SetFlag(0x224);
      }
    }
  }
}

void Event_Start(void);
void Event_End(void);

void Func_0200036c(void) {
  /* FAKEMATCH(wrapper): the placements and flags through event.h's inlines,
   * which build each constant again at each call; as direct calls, which share
   * 764 and 380 in saved registers, 36 of 76 instructions differ in tla-en. */
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetDestination(17, 764, 384);
  Actor_MoveToAndWait(16, 746, 380);
  Actor_Get(17)->velocityY = PIXELS(4);
  FieldActor_WaitForMoveById(17);
  Actor_Get(17)->priorityFlags |= 2;
  if (!Event_GetFlag(0x8ff) && !Event_GetFlag(0x201)) {
    FieldActor_Stop(10);
    Actor_MoveToAndWait(10, 764, 388);
    Actor_SetDestination(17, 764, 376);
    Actor_WalkToAndWait(10, 764, 380);
    Actor_SetAnimation(10, 4);
    Actor_SetAttachedEffect(10, 257);
    Event_SetFlag(0x202);
  }
  Event_End();
}

void Func_02000428(void) { GameFlag_SetBit(0x882); }

void Func_02000438(void) { Func_0200114c(); }

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
        Func_0200015c(20);
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
          Func_0200015c(13);
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

extern const u8 gData_02001ad8[];
extern const u8 gData_02001ae4[];
extern const u8 gData_02001c58[];
extern const u8 gData_02001db4[];

const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov66fRoom1)
    return gData_02001ae4;
  if (scene == (s32)&SceneId_Ov66fRoom2)
    return gData_02001c58;
  if (scene == (s32)&SceneId_Ov66fRoom3)
    return gData_02001db4;
  return gData_02001ad8;
}

#include "ram_buffer.h"
#include "version.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02000af0(void);
void Func_02001174(void);
void Func_0200117c(s32 a, s32 b, s32 c, s32 d);

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

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = EVENT_WORK_VALUE;
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov66fRoom1) {
    Actor_SetSpritePriority(18, 1);
    Actor_SetSpritePriority(19, 1);
    Actor_SetSingleChildFlags(Actor_GetById(19), 0);
    Actor_GetById(19)->collisionFlags = 0;
    Actor_GetById(19)->priorityFlags |= 2;
    Actor_GetById(12)->collisionFlags = 0;
    if (GameFlag_Test(0x222)) {
      Map_CopyCellAttributeRect(30, 35, 1, 1, 28, 34);
      Map_CopyCellAttributeRect(30, 34, 1, 1, 28, 33);
    }
    if (GameFlag_Test(0x221))
      MapGrid_CopyCellAttributes(24, 32, 1, 1, 22, 32);
    if (GameFlag_Test(0x223))
      MapGrid_CopyCellAttributes(49, 28, 1, 1, 47, 28);
    else
      Actor_SetSpritePriority(14, 1);
    if (GameFlag_Test(0x224))
      MapGrid_CopyCellAttributes(53, 35, 1, 1, 53, 36);
    flag = GameFlag_Test(0x202);
    if (flag == 0) {
      struct Actor *lift;

      Actor_GetById(16)->priorityFlags |= 0x80;
      lift = Actor_GetById(17);
      lift->priorityFlags |= 0x40;
      lift->y = 0xe00000;
      lift->terrainHeight = 0xe00000;
      lift->collisionFlags = flag;
    } else {
      Actor_SetAttachedEffect(10, 257);
    }
    Func_080d3b28(17, 231);
    if (GameFlag_Test(0x882))
      Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
    else
      Actor_GetById(13)->scaleY = 0x14000;
  } else if (scene == (s32)&SceneId_Ov66fRoom2) {
    Actor_SetSpritePriority(8, 1);
    Actor_SetSpritePriority(9, 1);
    Func_02001174();
    Func_0200117c(0, 0x9e8, 8, 9);
    if (GameFlag_Test(0x210))
      MapGrid_CopyCellAttributes(6, 16, 1, 1, 4, 16);
    if (GameFlag_Test(0x211))
      MapGrid_CopyCellAttributes(8, 30, 1, 1, 10, 30);
    if (GameFlag_Test(0x8e7)) {
      object = Actor_GetById(20);
      Actor_SetPosition(20, 0x480000, 0x1c80000);
      object->motionFlags = 0;
      height = Map_GetTerrainHeight(0, object->x, object->z);
      object->terrainHeight = height;
      object->y = height;
      MapGrid_CopyCellAttributes(3, 28, 1, 1, 4, 28);
    }
    if (GameFlag_Test(0x8e8)) {
      object = Actor_GetById(13);
      Actor_SetPosition(13, 0x1c80000, 0x1e80000);
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
    if (!GameFlag_Test(0x109) && gMainRoutineWork.entrance == 8)
      Func_02000af0();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

extern u8 MsgIsTheWay[];
extern const u8 gData_020017f8[];

void Func_02001134(void);

void Func_02000960(void) {
  /* FAKEMATCH(wrapper): the calls through event.h's inlines keep each call's
   * own constants and argument order; as direct Actor_SetPositionAndReset calls
   * for Actor_WalkToAndWait, 42 of 140 instructions differ in tla-en. */
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgIsTheWay);
  Actor_SetPosition(27, PIXELS(312), PIXELS(304));
  Event_ShowMessage(27, 0);
  FieldCamera_MoveTo(PIXELS(256), -1, PIXELS(392), 1);
  Func_02001134();
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
  Actor_ShowEmote(11, 258, 30);
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(27, 256, 30);
  Event_ShowMessage(27, 0);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(27, 258, 30);
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

  Func_0200103c(object, 1);
  object->acceleration = 0x4000;
  object->speedLimit = 0x8000;
}

extern u8 MsgHereTheIntruder[];

void Func_02000af0(void) {
  s32 owner = gMainRoutineWork.currentOwner;

  /* FAKEMATCH(wrapper): the calls through event.h's inlines, as in
   * Func_02000960, build each call's constants again in the original's argument
   * order; as direct calls, 126 of 251 instructions differ in tla-en. */
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(23, PIXELS(248), PIXELS(472));
  Actor_SetPosition(24, PIXELS(264), PIXELS(472));
  Actor_SetPosition(25, PIXELS(235), PIXELS(483));
  Actor_SetPosition(26, PIXELS(232), PIXELS(496));
  Actor_SetPosition(27, PIXELS(277), PIXELS(483));
  Actor_SetPosition(28, PIXELS(280), PIXELS(496));
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
  Actor_ShowEmote(27, 259, 30);
  Event_ShowMessage(27, 0);
  Actor_FaceActor(27, 25, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(25, 3);
  Actor_FaceActor(27, owner, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(27, 263, 30);
  Event_ShowMessage(27, 0);
  Actor_SetAnimationAndWait(27, 3);
  Event_ShowMessage(27, 0);
  Actor_FaceActor(25, 23, 0);
  Actor_ShowEmote(25, 256, 30);
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
  Event_SetFlag(0x300);
}

extern const u8 MsgGuardsAnIntruder[];
extern const u8 MsgGrrrrrRrrruffRrufff[];

void Event_SetDestination(s32 scene, s32 entrance);

void Func_02000d80(void) {
  s32 moved = Event_GetFlag(0x300);
  s32 scene = (s32)&SceneId_Ov66fRoom2;

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
  Actor_ShowEmote(id, 0x100, 30);
  Actor_FaceActor(id, owner, 20);
  FieldEvent_SetMessageId(MsgGuardsAnIntruder);
  Event_ShowMessage(id, 0);
  Actor_SetAttachedEffect(owner, 0x101);
  Actor_SetAnimation(id, 4);
  FieldEvent_Wait(30);
  Event_ShowMessage(id, 0);
  Actor_SetAttachedEffect(owner, 0x100);
  Func_02000d80();
  Event_End();
}

void Func_02000e28(s32 id) {
  s32 owner = gMainRoutineWork.currentOwner;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(id);
  Actor_ShowEmote(id, 0x100, 30);
  Actor_FaceActor(id, owner, 20);
  FieldEvent_SetMessageId(MsgGrrrrrRrrruffRrufff);
  Event_ShowMessage(id, 0);
  Actor_SetAttachedEffect(owner, 0x101);
  Actor_SetAnimation(id, 4);
  FieldEvent_Wait(30);
  Event_ShowMessage(8, 0);
  Actor_SetAttachedEffect(owner, 0x100);
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
  if (!Event_GetFlag(514) && !Event_GetFlag(513))
    Func_02000e28(10);
}

void Func_02000fc4(void) {
  if (!Event_GetFlag(512))
    Func_02000da8(9);
}

void Func_02000fdc(void) {
  if (!Event_GetFlag(512))
    Func_02000da8(9);
}

void Func_02000ff4(void) {
  if (!Event_GetFlag(515) && !Event_GetFlag(514))
    Func_02000e28(10);
}
