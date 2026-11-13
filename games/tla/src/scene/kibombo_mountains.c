/// @file
/// The scene code of overlay 0x66f, in ROM order.

#include "flags.h"
#include "shared/map_cells.h"
#include "shared/actors.h"
#include "field/field.h"

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "event_work.h"
#include "module/scene/kibombo_mountains.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "text/msg_ids.h"

void Field_TryJumpForward(void);

void Func_02000038(void) { Field_RequestDjinnBattle(18, 70); }

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

DEFINE_ACTOR_BOB(Actor_Bob)

#include "scene_ids.h"

/* VIEW(first): routine gData_02001294 as this part declared it; the joined file has another. */
extern const u8 gData_02001294__first[] __asm__("gData_02001294");
#define gData_02001294 gData_02001294__first
/* VIEW(first): routine gData_020015f0 as this part declared it; the joined file has another. */
extern const u8 gData_020015f0__first[] __asm__("gData_020015f0");
#define gData_020015f0 gData_020015f0__first
/* VIEW(first): routine gData_020018c8 as this part declared it; the joined file has another. */
extern const u8 gData_020018c8__first[] __asm__("gData_020018c8");
#define gData_020018c8 gData_020018c8__first

const u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov66fRoom1)
    return gData_020015f0;
  if (scene == (s32)&SceneId_Ov66fRoom2)
    return gData_020018c8;
  return gData_02001294;
}

DEFINE_PILLAR_SET_MOVED(Pillar_SetMoved, 11, 24, 32, 22, 0x221)

void Func_02000124(void) {
  MapGrid_CopyCellAttributes(30, 35, 1, 1, 28, 34);
  MapGrid_CopyCellAttributes(30, 34, 1, 1, 28, 33);
  Event_SetFlag(0x222);
}

DEFINE_ACTOR_WAIT_TO_LAND(Actor_WaitToLand)

void Func_02000190(s32 unused, s32 id) {
  if (id == 15)
    FieldActor_SetSpritePriority(15, 1);
}

#include "iwram_call.h"
#include "call.h"

/* VIEW(first): routine gData_02001404 as this part declared it; the joined file has another. */
extern const u8 gData_02001404__first[] __asm__("gData_02001404");
#define gData_02001404 gData_02001404__first

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

    if (!Event_GetFlag(0x223) && (x = block->x >> 20) == 47 &&
        (z = block->z >> 20) == 28) {
      Actor_WaitToLand(14);
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

      Actor_WaitToLand(15);
      Actor_SetSpritePriority(15, 2);
      other = Actor_GetById(9);
      dx = block->x - other->x;
      if (dx >= 0 ? dx < 0x80000 : other->x - block->x < 0x80000) {
        other->motionFlags = 0;
        other->speedLimit = 0x10000;
        FieldActor_SetMoveTarget(other, block->x, block->y, block->z);
        FieldActor_WaitForMove(other);
        Actor_StartScript(block, gData_02001404);
        Actor_StartIdle(other);
        other->x = 0;
        other->z = 0;
        Event_SetFlag(0x200);
      } else {
        Call3((void (*)())Iwram_CopyWordsEntry, (s32)&saved, (s32)other, 128);
        Actor_ResetTargetsAndVelocity(9);
        FieldActor_Jump(9, 4, 0);
        FieldEvent_Wait(30);
        Actor_ShowEmote(9, 257, 0);
        Actor_FaceActor(9, 15, 0);
        FieldEvent_Wait(60);
        Actor_SetAnimationAndWait(9, 4);
        FieldEvent_Wait(20);
        {
          void *from = &saved;
          Iwram_CopyWordsEntry(other, from, 128);
        }
        MapGrid_CopyCellAttributes(53, 35, 1, 1, 53, 36);
        Event_SetFlag(0x224);
      }
    }
  }
}

void Func_0200036c(void) {
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

void Func_02000428(void) { GameFlag_Set(0x882); }

void Func_02000438(void) { Field_TryJumpForward(); }

DEFINE_PILLAR_SET_MOVED(Func_02000440, 18, 6, 16, 4, 0x210)

DEFINE_PILLAR_SET_MOVED(Func_02000478, 19, 8, 30, 10, 0x211)

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

/* VIEW(first): routine gData_02001ad8 as this part declared it; the joined file has another. */
extern const u8 gData_02001ad8__first[] __asm__("gData_02001ad8");
#define gData_02001ad8 gData_02001ad8__first
/* VIEW(first): routine gData_02001ae4 as this part declared it; the joined file has another. */
extern const u8 gData_02001ae4__first[] __asm__("gData_02001ae4");
#define gData_02001ae4 gData_02001ae4__first
/* VIEW(first): routine gData_02001c58 as this part declared it; the joined file has another. */
extern const u8 gData_02001c58__first[] __asm__("gData_02001c58");
#define gData_02001c58 gData_02001c58__first
/* VIEW(first): routine gData_02001db4 as this part declared it; the joined file has another. */
extern const u8 gData_02001db4__first[] __asm__("gData_02001db4");
#define gData_02001db4 gData_02001db4__first

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

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      EVENT_WORK_VALUE;
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov66fRoom1) {
    Actor_SetSpritePriority(18, 1);
    Actor_SetSpritePriority(19, 1);
    Actor_SetSpriteFlags(Actor_GetById(19), 0);
    Actor_GetById(19)->collisionFlags = 0;
    Actor_GetById(19)->priorityFlags |= 2;
    Actor_GetById(12)->collisionFlags = 0;
    if (GameFlag_Get(0x222)) {
      Map_CopyCellAttributeRect(30, 35, 1, 1, 28, 34);
      Map_CopyCellAttributeRect(30, 34, 1, 1, 28, 33);
    }
    if (GameFlag_Get(0x221))
      MapGrid_CopyCellAttributes(24, 32, 1, 1, 22, 32);
    if (GameFlag_Get(0x223))
      MapGrid_CopyCellAttributes(49, 28, 1, 1, 47, 28);
    else
      Actor_SetSpritePriority(14, 1);
    if (GameFlag_Get(0x224))
      MapGrid_CopyCellAttributes(53, 35, 1, 1, 53, 36);
    flag = GameFlag_Get(0x202);
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
    Actor_ShowItemIconById(17, 231);
    if (GameFlag_Get(0x882))
      Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
    else
      Actor_GetById(13)->scaleY = 0x14000;
  } else if (scene == (s32)&SceneId_Ov66fRoom2) {
    Actor_SetSpritePriority(8, 1);
    Actor_SetSpritePriority(9, 1);
    Rope_Initialize();
    Rope_Place(0, 0x9e8, 8, 9);
    if (GameFlag_Get(0x210))
      MapGrid_CopyCellAttributes(6, 16, 1, 1, 4, 16);
    if (GameFlag_Get(0x211))
      MapGrid_CopyCellAttributes(8, 30, 1, 1, 10, 30);
    if (GameFlag_Get(0x8e7)) {
      object = Actor_GetById(20);
      Actor_SetPosition(20, 0x480000, 0x1c80000);
      object->motionFlags = 0;
      height = Map_GetTerrainHeight(0, object->x, object->z);
      object->terrainHeight = height;
      object->y = height;
      MapGrid_CopyCellAttributes(3, 28, 1, 1, 4, 28);
    }
    if (GameFlag_Get(0x8e8)) {
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
  Actor_SetPosition(27, PIXELS(312), PIXELS(304));
  Event_ShowMessage(27, 0);
  FieldCamera_MoveTo(PIXELS(256), -1, PIXELS(392), 1);
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

  Actor_SelectAnimation(object, 1);
  object->acceleration = 0x4000;
  object->speedLimit = 0x8000;
}

void Func_02000af0(void) {
  s32 owner = gMainRoutineWork.currentOwner;

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

#undef gData_02001db4
#undef gData_02001c58
#undef gData_02001ae4
#undef gData_02001ad8
#undef gData_02001404
#undef gData_020018c8
#undef gData_020015f0
#undef gData_02001294
#undef EVENT_WORK_VALUE
#undef gData_02001db4
#undef gData_02001c58
#undef gData_02001ae4
#undef gData_02001ad8
#undef gData_02001404
#undef gData_020018c8
#undef gData_020015f0
#undef gData_02001294
// ---- scene/data/kibombo_mountains.c ----
#include "shared/tables.h"
#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "objcmd.h"
#include "module/scene/kibombo_mountains.h"

void Func_02000038(void);
/* VIEW(data): routine Actor_Bob as this part declared it; the joined file has another. */
void Actor_Bob__data(void) __asm__("Actor_Bob");
#define Actor_Bob Actor_Bob__data
void Pillar_SetMoved(void);
void Func_02000124(void);
/* VIEW(data): routine Func_02000190 as this part declared it; the joined file has another. */
void Func_02000190__data(void) __asm__("Func_02000190");
#define Func_02000190 Func_02000190__data
/* VIEW(data): routine Func_020001a0 as this part declared it; the joined file has another. */
void Func_020001a0__data(void) __asm__("Func_020001a0");
#define Func_020001a0 Func_020001a0__data
void Func_02000428(void);
/* VIEW(data): routine Func_020004b0 as this part declared it; the joined file has another. */
void Func_020004b0__data(void) __asm__("Func_020004b0");
#define Func_020004b0 Func_020004b0__data
/* VIEW(data): routine Func_0200051c as this part declared it; the joined file has another. */
void Func_0200051c__data(void) __asm__("Func_0200051c");
#define Func_0200051c Func_0200051c__data
void Func_02000f9c(void);

extern u32 gData_02001728[];

// clang-format off
u32 gData_020011a4[] = {
    SC_ARG(2),
    SC_END,
    SC_ARG(0),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, PIXELS(24)),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(107),
    SCENE_EXIT_IF(1, 2, 26, -1),
    SCENE_EXIT_IF(2, 108, 2, -1),
    SCENE_EXIT_IF(3, 108, 3, -1),
    SCENE_EXIT_IF(4, 108, 4, -1),
    SCENE_EXIT_IF(5, 109, 1, -1),
    SCENE_EXIT_IF(6, 109, 2, -1),
    SCENE_EXIT_IF(7, 109, 3, -1),
    SCENE_EXITS(108),
    SCENE_EXIT_IF(1, 2, 25, -1),
    SCENE_EXIT_IF(2, 107, 2, -1),
    SCENE_EXIT_IF(3, 107, 3, -1),
    SCENE_EXIT_IF(4, 107, 4, -1),
    SCENE_EXITS(109),
    SCENE_EXIT_IF(1, 107, 5, -1),
    SCENE_EXIT_IF(2, 107, 6, -1),
    SCENE_EXIT_IF(3, 107, 7, -1),
    SCENE_EXITS_END,
};

u32 gData_02001284[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

struct ActorPlacement gData_02001294[] = {
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

DEFINE_G_DATA_02001334(gData_02001334, 856, 584, 792)

s32 gData_02001404[] = {
    SC_CALL(Actor_Bob),
    SC_END,
};

u32 gData_02001410[] = {
    SC_SET(SW_SPEED, 0x14000),
    SC_SET(SW_ACCEL, 0x4000),
    SC_SET(SW_COLLISION, 129),
    SC_ARG(3),
    SC_SET(SW_VELOCITY_Y, PIXELS(3)),
    SC_MOVE(PIXELS(760), 0, PIXELS(488)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, PIXELS(2)),
    SC_MOVE(PIXELS(764), 0, PIXELS(472)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, PIXELS(2)),
    SC_MOVE(PIXELS(760), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, PIXELS(2)),
    SC_MOVE(PIXELS(764), 0, PIXELS(440)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, PIXELS(2)),
    SC_MOVE(PIXELS(760), 0, PIXELS(424)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, PIXELS(2)),
    SC_MOVE(PIXELS(764), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(760), 0, PIXELS(392)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, PIXELS(3)),
    SC_MOVE(PIXELS(764), 0, PIXELS(408)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, PIXELS(2)),
    SC_MOVE(PIXELS(760), 0, PIXELS(424)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, PIXELS(2)),
    SC_MOVE(PIXELS(764), 0, PIXELS(440)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, PIXELS(2)),
    SC_MOVE(PIXELS(760), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, PIXELS(2)),
    SC_MOVE(PIXELS(764), 0, PIXELS(472)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, PIXELS(2)),
    SC_MOVE(PIXELS(760), 0, PIXELS(488)),
    SC_SYNC,
    SC_MOVE(PIXELS(764), 0, PIXELS(504)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct ActorPlacement gData_020015f0[] = {
    {83, 0x8ff, (s32)gData_020012ac, PIXELS(616), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {83, 0x8ff, (s32)gData_02001334, PIXELS(824), 0, PIXELS(584), FACING_NORTH, 2, 0},
    {189, 0x8ff, (s32)gData_02001410, PIXELS(760), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(520), FACING_NORTH, 2, 0},
    {299, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(544), FACING_NORTH, 2, 0},
    {307, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(320), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, (s32)gData_02001284, PIXELS(712), 0, PIXELS(472), FACING_NORTH, 2, 0},
    {287, CONDITION_ALWAYS, (s32)gData_02001284, PIXELS(888), 0, PIXELS(552), FACING_NORTH, 2, 0},
    {288, CONDITION_ALWAYS, 1, PIXELS(732), 0, PIXELS(376), FACING_NORTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(732), 0, PIXELS(376), FACING_NORTH, 2, 0},
    {246, 0x76, 1, PIXELS(440), 0, PIXELS(472), FACING_SOUTH, 0, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(712), 0, PIXELS(584), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_02001334(gData_02001728, 184, 296, 248)

DEFINE_G_DATA_02001334(gData_020017f8, 248, 448, 264)

struct ActorPlacement gData_020018c8[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(200), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(200), FACING_NORTH, 2, 0},
    {83, 0x8ff, (s32)gData_02001728, PIXELS(200), 0, PIXELS(296), FACING_SOUTH, 2, 0},
    {83, 0x8ff, (s32)gData_020017f8, PIXELS(248), 0, PIXELS(456), FACING_SOUTH, 2, 0},
    {83, 0x8ff, (s32)gData_020012ac, PIXELS(424), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {250, CONDITION_ALWAYS, (s32)gData_02001284, PIXELS(456), 0, PIXELS(472), FACING_NORTH, 2, 0},
    {287, CONDITION_ALWAYS, (s32)gData_02001284, PIXELS(136), 0, PIXELS(296), FACING_NORTH, 2, 0},
    {287, CONDITION_ALWAYS, (s32)gData_02001284, PIXELS(312), 0, PIXELS(248), FACING_NORTH, 2, 1},
    {287, CONDITION_ALWAYS, (s32)gData_02001284, PIXELS(328), 0, PIXELS(552), FACING_NORTH, 2, 1},
    {287, CONDITION_ALWAYS, (s32)gData_02001284, PIXELS(440), 0, PIXELS(536), FACING_NORTH, 2, 1},
    {290, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(264), FACING_NORTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(488), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, (s32)gData_02001284, PIXELS(88), 0, PIXELS(456), FACING_NORTH, 2, 0},
    {305, CONDITION_ALWAYS, (s32)gData_020011a4, PIXELS(232), 0, PIXELS(536), FACING_WEST, 2, 0},
    {305, CONDITION_ALWAYS, (s32)gData_020011a4, PIXELS(280), 0, PIXELS(536), FACING_EAST, 2, 1},
    {83, CONDITION_ALWAYS, (s32)gData_020012ac, 0, 0, 0, 0x3f00, 2, 0},
    {83, CONDITION_ALWAYS, (s32)gData_02001334, 0, 0, 0, 0x4100, 2, 0},
    {83, CONDITION_ALWAYS, (s32)gData_020012ac, 0, 0, 0, FACING_EAST, 2, 0},
    {83, CONDITION_ALWAYS, (s32)gData_02001334, 0, 0, 0, FACING_EAST, 2, 0},
    {83, CONDITION_ALWAYS, (s32)gData_020012ac, 0, 0, 0, FACING_WEST, 2, 0},
    {83, CONDITION_ALWAYS, (s32)gData_02001334, 0, 0, 0, FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_02001ad8[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02001ae4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0xce01, 5, CONDITION_ALWAYS, 5},
    {0xce01, 6, IF_FLAG_SET(0x882), 6},
    {0xce01, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_STEP, 20, 0x8ff, EVENT(Func_02000f84)},
    {TRIGGER_STEP, 21, 0x8ff, EVENT(Func_02000f90)},
    {TRIGGER_STEP, 22, 0x8ff, EVENT(Func_02000f9c)},
    {TRIGGER_STEP, 23, 0x8ff, EVENT(Func_02000fc4)},
    {TRIGGER_STEP, 24, 0x8ff, EVENT(Func_02000fdc)},
    {TRIGGER_STEP, 25, 0x8ff, EVENT(Func_02000ff4)},
    {0x8602, 33, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02000190)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_020001a0)},
    {TRIGGER_TALK, 10, IF_FLAG_SET(0x202), MSGNO(MsgSniffSniffLick)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x202), MSGNO(MsgABoneThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgThisRockIs)},
    {TRIGGER_TALK, 15, IF_FLAG_SET(0x200), MSGNO(MsgHeyWhoS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(0x200), MSGNO(MsgItSDark)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Func_02000038)},
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

struct Trigger gData_02001c58[] = {
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
    {8, 0, CONDITION_ALWAYS, EVENT(Func_020004b0)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_0200051c)},
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

s16 gData_02001db4[] = {
    1, 0, 1, -1, 1, 0, 1, 0,
    2, -1, 2, 0, 1, 0, 3, -1,
    3, 0, -1, -1, 0, 0, 0, 0,
};
// clang-format on
#undef Func_0200051c
#undef Func_020004b0
#undef Func_020001a0
#undef Func_02000190
#undef Actor_Bob
