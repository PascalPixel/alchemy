/// @file
/// The scene code of overlay 0x6a8 that its scene tables belong to, in ROM order; the rest of the overlay is in prox/.

#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "actor.h"
#include "version.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "call.h"
#include "prox/local.h"
#include "input.h"
#include "../../../../games/tla/src/scene/prox/local.h"
#include "frame.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b);
s32 ArcTan2(s32 y, s32 x);
void Func_020043c4(struct Actor *object, s32 mode);

s32 Func_020002d8(struct Actor *object, struct Actor *target, s32 range,
                  s32 force) {
  s32 result = 0;
  s32 *targetPos;
  s32 *objectPos;

  if (BYTE_AT(object, 99) != 0) {
    object->movementState = 1;
    return 1;
  }
  targetPos = &target->x;
  objectPos = &object->x;
  if (SceneActor_CalculateFixedPointDistance(targetPos, objectPos) < range ||
      force != 0) {
    u32 angle = (u16)ArcTan2(target->z - object->z, *targetPos - *objectPos);
    u32 left = (angle - 0x1000) & 0xf000;
    u32 right = (angle + 0x1000) & 0xf000;
    u32 ahead = angle & 0xf000;
    u32 facing = object->angle & 0xf000;

    if (ahead == facing || right == facing || left == facing || force != 0) {
      /* FAKEMATCH(temporary): the count and movementState's address held in
       * locals before the test; as blockedSteps read in the test and
       * object->movementState at each store, 16 of 107 instructions differ in
       * tla-en. */
      s16 count = ((struct EventWork *)Ram_HeapSlots->eventWork)->blockedSteps;
      u8 *movementState = &object->movementState;

      if (count > 120) {
        *movementState = 0;
        Func_020043c4(object, 2);
      } else {
        *movementState = 1;
        Func_020043c4(object, 1);
        result = 1;
      }
    }
  } else {
    object->movementState = 0;
    Func_020043c4(object, 2);
  }
  return result;
}

// The Japanese form of this function, the older revision without the test
// that the leader stands within 11 of the actor, is still on its listing.
#if TLA_PATCH_1

s32 SceneActor_CalculateFixedPointDistance(s32 *from, s32 *to);
s32 Func_020002d8(struct Actor *actor, struct Actor *target, s32 distance,
                  s32 flag);
void Func_020043c4(struct Actor *actor, s32 mode);

/// Once the leader is within 11 of the actor, stops it (Func_020043c4 mode
/// 2). Otherwise it runs Func_020002d8 toward object 18, 16 or 17 by its
/// action (0, 1, other) at 24, and if that returns 0, toward the leader at
/// 32, flagged when action bit 1 is set while the event work's halfword
/// 0x160 or byte 4 of the window tiles is nonzero.
s32 Func_020003bc(struct Actor *actor) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *work = Ram_HeapSlots->eventWork;
  s32 flag = 0;
  struct Actor *target;

  target = Actor_Get(gMainRoutineWork.currentOwner);
  if (SceneActor_CalculateFixedPointDistance(&target->x, &actor->x) <= 11) {
    actor->movementState = flag;
    Func_020043c4(actor, 2);
    return 0;
  }
  if (actor->action == 0)
    target = Actor_Get(18);
  else if (actor->action == 1)
    target = Actor_Get(16);
  else
    target = Actor_Get(17);
  if (Func_020002d8(actor, target, 24, 0) == 0) {
    target = Actor_Get(gMainRoutineWork.currentOwner);
    if ((*(s16 *)(work + 352) != 0 || tiles[4] != 0) && (actor->action & 2))
      flag = 1;
    Func_020002d8(actor, target, 32, flag);
  }
  return 0;
}

#else

s32 Func_020002d8(struct Actor *actor, struct Actor *target, s32 distance,
                  s32 flag);

s32 Func_020003bc(struct Actor *actor) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *work = Ram_HeapSlots->eventWork;
  s16 *action = &actor->action;
  s32 flag = 0;
  struct Actor *target;

  if (*action == 0)
    target = Actor_Get(18);
  else if (*action == 1)
    target = Actor_Get(16);
  else
    target = Actor_Get(17);
  if (Func_020002d8(actor, target, 24, 0) == 0) {
    target = Actor_Get(gMainRoutineWork.currentOwner);
    if ((*(s16 *)(work + 352) != 0 || tiles[4] != 0) && (*action & 2))
      flag = 1;
    Func_020002d8(actor, target, 32, flag);
  }
  return 0;
}

#endif

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gData_02004b14[];
extern struct ActorPlacement gData_02004b2c[];
extern struct ActorPlacement gData_02004c4c[];
extern struct ActorPlacement gData_02004d9c[];
extern struct ActorPlacement gData_02004f04[];
extern struct ActorPlacement gData_0200506c[];
extern struct ActorPlacement gData_0200521c[];
extern struct ActorPlacement gData_02005444[];
extern struct ActorPlacement gData_020054a4[];
extern struct ActorPlacement gData_0200551c[];

const struct ActorPlacement *Func_0200048c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6a8Room1 && Event_GetFlag(0x9c5) &&
      !Event_GetFlag(0x9cc))
    return gData_02004b2c;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov6a8Room2)
    return gData_02004c4c;
  if (scene == (s32)&SceneId_Ov6a8Room3) {
    if (Event_GetFlag(0xa3d))
      return gData_02004f04;
    return gData_02004d9c;
  }
  if (scene == (s32)&SceneId_Ov6a8Room4) {
    if (Event_GetFlag(0xa3d)) {
      gData_0200521c[1].talkFacing = 3;
      gData_0200521c[4].talkFacing = 3;
      return gData_0200521c;
    }
    return gData_0200506c;
  }
  if (scene == (s32)&SceneId_Ov6a8Room5) {
    if (Event_GetFlag(0xa3d))
      return gData_020054a4;
    return gData_02005444;
  }
  if (scene == (s32)&SceneId_Ov6a8Room6)
    return gData_0200551c;
  return gData_02004b14;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_020045cc();

void Func_02000588(s32 a0) { Call3(Func_020045cc, a0, 0x40, 0x300); }

extern const u8 gData_0200554c[];
extern const u8 gData_02005558[];
extern const u8 gData_02005588[];
extern const u8 gData_020057f8[];
extern const u8 gData_02005a20[];
extern const u8 gData_02005da4[];
extern const u8 gData_02005eac[];

const u8 *Func_02000598(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6a8Room1)
    return gData_02005558;
  if (scene == (s32)&SceneId_Ov6a8Room2)
    return gData_02005588;
  if (scene == (s32)&SceneId_Ov6a8Room3)
    return gData_020057f8;
  if (scene == (s32)&SceneId_Ov6a8Room4)
    return gData_02005a20;
  if (scene == (s32)&SceneId_Ov6a8Room5)
    return gData_02005da4;
  if (scene == (s32)&SceneId_Ov6a8Room6)
    return gData_02005eac;
  return gData_0200554c;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02004464();
s32 Func_0200446c();
extern u8 MsgProx33[];

void Func_02000620(void) {
  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgProx33);
  Event_AskYesNo(0xf, 0);
  Func_0200446c();
}

s32 Func_02000644(void) {
  u16 turn;
  s32 facing;

  turn = Actor_Get(gMainRoutineWork.currentOwner)->angle + 0x5fff;
  facing = 1;
  if (turn >= 0x3fff)
    facing = 0;
  return facing;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02000644();
s32 Func_02004634();
s32 Func_02004464();
s32 Func_0200446c();
extern u8 MsgProx135[];
extern u8 MsgProx56[];

void Func_02000674(void) {
  s32 t0;
  s32 t4;

  t0 = Func_02000644();
  if (t0 != 0) {
    Func_02004634(0xe, 0x16);
  } else {
    Func_02004464();
    FieldEvent_ResolvePendingActions(0);
    t4 = Event_GetFlag(0xa3d);
    if (t4 != 0) {
      FieldEvent_SetMessageId((s32)MsgProx135);
    } else {
      FieldEvent_SetMessageId((s32)MsgProx56);
    }
    Event_ShowMessage(22, 0);
    Func_0200446c();
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02000644();
s32 Func_02004624();
s32 Func_02004464();
s32 Func_0200446c();
extern u8 MsgProx132[];
extern u8 MsgProx53[];

void Func_020006c4(void) {
  s32 t0;
  s32 t4;

  t0 = Func_02000644();
  if (t0 != 0) {
    Func_02004624(0x1f, 0xa);
  } else {
    Func_02004464();
    FieldEvent_ResolvePendingActions(0);
    t4 = Event_GetFlag(0xa3d);
    if (t4 != 0) {
      FieldEvent_SetMessageId((s32)MsgProx132);
    } else {
      FieldEvent_SetMessageId((s32)MsgProx53);
    }
    Event_ShowMessage(10, 0);
    Func_0200446c();
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02000644();
s32 Func_0200462c();
s32 Func_02004464();
s32 Func_0200446c();
extern u8 MsgProx139[];
extern u8 MsgProx60[];

void Func_02000714(void) {
  s32 t0;
  s32 t4;

  t0 = Func_02000644();
  if (t0 != 0) {
    Func_0200462c(8);
  } else {
    Func_02004464();
    FieldEvent_ResolvePendingActions(0);
    t4 = Event_GetFlag(0xa3d);
    if (t4 != 0) {
      FieldEvent_SetMessageId((s32)MsgProx139);
    } else {
      FieldEvent_SetMessageId((s32)MsgProx60);
    }
    Event_ShowMessage(8, 0);
    Func_0200446c();
  }
}

/// The bytes of an actor these handlers use. unk63 is the last of the four
/// scene-owned bytes the Broken Seal's actors keep at this place.
struct TalkActor {
  u8 unk00[6];
  u16 angle; ///< 1/65536 turns
  u8 unk08[0x5b];
  u8 unk63; ///< Func_020002d8 reads it
};

extern u8 MsgProx113[];
extern u8 MsgProx114[];
extern u8 MsgProx115[];
extern u8 MsgProx117[];
extern u8 MsgProx118[];
extern u8 MsgProx119[];

void Func_02000764(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_Get(16));
  s16 angle = object->angle;

  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_FaceActor(16, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)MsgProx113);
  } else {
    FieldEvent_SetMessageId((s32)MsgProx117);
  }
  Event_ShowMessage(16, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_0200446c();
}

void Func_020007f4(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_Get(17));
  s16 angle = object->angle;

  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_FaceActor(17, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)MsgProx114);
  } else {
    FieldEvent_SetMessageId((s32)MsgProx118);
  }
  Event_ShowMessage(17, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_0200446c();
}

void Func_02000884(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_Get(18));
  s16 angle = object->angle;

  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_FaceActor(18, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)MsgProx115);
  } else {
    FieldEvent_SetMessageId((s32)MsgProx119);
  }
  Event_ShowMessage(18, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_0200446c();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02004464();
s32 Func_0200446c();
extern u8 MsgProx47[];

void Func_02000914(void) {
  s32 t0;
  s32 t8;

  t0 = Event_GetFlag(0xa3d);
  if (t0 != 0) {
  } else {
    Func_02004464();
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(158);
    MapGrid_CopyCellsTo(0x59, 0, 0x4d, 0x10, 1, 2);
    Event_SetFlag(0x200);
    FieldEvent_Wait(10);
    {
      s32 y = 0xc0;
      Actor_SetPositionAndFacing(8, PIXELS(520), PIXELS(360), y << 6);
    }
    t8 = ((s32)FieldCamera_GetActor());
    FIELD_AT((t8 + 0x55), u8, 0) = t0;
    FieldCamera_SetSpeed(0x19999, 0x3333);
    FieldCamera_MoveTo(PIXELS(552), -1, PIXELS(388), 1);
    FieldEvent_Wait(10);
    Actor_SetSpeed(8, 0x13333, 0x9999);
    Actor_WalkToAndWait(8, 526, 384);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Actor_WalkToAndWait(8, 540, 388);
    Actor_FaceDirectionAndWait(8, 0);
    Actor_SetAnimation(8, 4);
    FieldEvent_SetMessageId((s32)MsgProx47);
    Event_ShowMessage(0x4008, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Event_ShowMessage(9, 0);
    Actor_SetAnimation(8, 3);
    Event_ShowMessage(0x4008, 0);
    Actor_ShowEmote(9, 0x100, 20);
    Actor_FaceDirection(9, 0xb000, 0);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(8, 0xb000, 40);
    Actor_FaceDirectionAndWait(8, 0);
    Actor_SetAnimation(8, 4);
    Event_ShowMessage(0x4008, 0);
    Actor_FaceDirectionAndWait(9, FACING_WEST);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessage(9, 0);
    Actor_ShowEmote(8, 0x109, 20);
    Event_ShowMessage(0x4008, 0);
    Actor_SetAnimation(9, 3);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(9, 0xb000, 0);
    Actor_FaceDirectionAndWait(8, 0xb000);
    Event_SetFlag(0x9c0);
    Func_0200446c();
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02004464();
s32 Func_0200457c();
s32 Func_0200446c();
extern u8 MsgProx62[];

void Func_02000ad8(void) {
  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(160), -1, PIXELS(160), 1);
  Func_0200457c();
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(0xa, 1);
  Actor_FaceDirectionAndWait(10, 0xb000);
  FieldEvent_SetMessageId((s32)MsgProx62);
  Event_ShowMessage(10, 0);
  Actor_SetAttachedEffect(0xc, 0x102);
  Actor_SetAttachedEffect(0xd, 0x102);
  Actor_SetAttachedEffect(0xe, 0x102);
  Actor_SetAttachedEffect(0xf, 0x102);
  Actor_SetAttachedEffect(0x10, 0x102);
  Actor_SetAttachedEffect(0x11, 0x102);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(10, 0x5000);
  Event_ShowMessage(10, 0);
  Event_ShowMessage(10, 0);
  Actor_SetAttachedEffect(0xc, 0x102);
  Actor_SetAttachedEffect(0xd, 0x102);
  Actor_SetAttachedEffect(0xe, 0x102);
  Actor_SetAttachedEffect(0xf, 0x102);
  Actor_SetAttachedEffect(0x10, 0x102);
  Actor_SetAttachedEffect(0x11, 0x102);
  FieldEvent_Wait(20);
  Actor_SetAnimation(12, 4);
  Event_ShowMessage(12, 0);
  Actor_FaceDirection(10, 0x3000, 0);
  Actor_ShowEmote(10, 0x100, 0);
  Actor_SetAnimationAndWait(13, 4);
  Event_ShowMessage(13, 0);
  FieldActor_Jump(15, 4, 10);
  Event_ShowMessage(15, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_ShowEmote(10, 0x105, 40);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(16, 0x107, 20);
  Event_ShowMessage(16, 0);
  FieldActor_Jump(14, 2, 0);
  Actor_SetAnimation(14, 3);
  Event_ShowMessage(14, 0);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_ShowEmote(10, 0x108, 40);
  Actor_FaceDirectionAndWait(10, 0x5000);
  Event_ShowMessage(10, 0);
  Actor_FaceDirectionAndWait(11, 0);
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(10, 0x100, 20);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_SetAnimation(11, 4);
  Event_ShowMessage(11, 0);
  Actor_SetAttachedEffect(0xa, 0x102);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(13, 0, 0);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirection(15, 0, 0);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Actor_FaceDirection(17, 0, 40);
  Actor_FaceDirection(12, 0xb000, 0);
  Actor_FaceDirection(13, 0xb000, 0);
  Actor_FaceDirection(14, 0xd000, 0);
  Actor_FaceDirection(15, 0xd000, 0);
  Actor_FaceDirection(16, 0xd000, 0);
  Actor_FaceDirection(17, 0xd000, 20);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(11, 0x109, 40);
  Event_ShowMessage(11, 0);
  Actor_FaceDirection(10, 0xb000, 80);
  Actor_SetAnimationAndWait(10, 4);
  Event_ShowMessage(10, 0);
  Event_SetFlag(0x9c1);
  Func_0200446c();
}

s32 Func_02004464();
s32 Func_020044dc();
s32 Func_0200446c();
extern u8 MsgProx75[];
extern u8 gData_020049c4[];
extern u8 gData_02004980[];
extern u8 gData_02004a08[];

void Func_02000dac(void) {
  u32 i;
  s32 result;
  s32 owner;

  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(160), -1, PIXELS(160), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 134, 156);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Prox_Jump(11, 0x100, 20);
  Actor_FaceDirectionAndWait(11, 0x5000);
  FieldEvent_SetMessageId((s32)MsgProx75);
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(10, 0x100, 0);
  Actor_FaceDirection(10, FACING_WEST, 40);
  Event_ShowMessage(10, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 144, 154);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Func_020044dc(0x12, gMainRoutineWork.currentOwner);
  Func_020044dc(0, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Actor_FaceDirection(14, 0xb000, 0);
  Actor_SetSpeed(18, 0xcccc, 0x6666);
  Actor_SetSpeed(0, 0xcccc, 0x6666);
  Actor_SetScript(0, (s32)gData_020049c4);
  Actor_RunScriptAndWait(18, (s32)gData_02004980);
  Actor_FaceDirection(11, 0x3000, 0);
  Actor_ShowEmote(11, 0x108, 20);
  Event_ShowMessage(11, 0);
  Actor_SetAnimation(18, 3);
  Event_ShowMessage(0x8012, 0);
  Actor_SetAnimation(0, 3);
  Event_ShowMessage(0xa000, 0);
  Actor_FaceDirection(11, 0, 0);
  Actor_FaceDirection(10, FACING_WEST, 40);
  Actor_FaceDirectionAndWait(11, 0x3000);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(10, 0x5000);
  Event_ShowMessage(10, 0);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(13, 0, 0);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirection(15, 0, 0);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Actor_FaceDirection(17, 0, 0);
  Actor_FaceDirectionAndWait(0, FACING_SOUTH);
  Actor_SetSpeed(0, 0x26666, 0x13333);
  Actor_Get(0)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(0, 160, 140);
  FieldEvent_Wait(1);
  Actor_Get(0)->actionFlags |= 1;
  Actor_SetAnimationAndWait(0, 0x15);
  Actor_FaceDirection(12, 0xb000, 0);
  Actor_FaceDirection(13, 0xb000, 0);
  Actor_FaceDirection(14, 0xb000, 0);
  Actor_FaceDirection(15, 0xd000, 0);
  Actor_FaceDirection(16, 0xd000, 0);
  Actor_FaceDirection(17, 0xd000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(18, 0xb000);
  Event_ShowMessage(0x8012, 0);
  Actor_SetAnimation(0, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimation(0, 1);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
  Actor_SetAnimation(18, 3);
  Event_ShowMessage(0x8012, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_SetSpeed(0, 0xcccc, 0x6666);
  Actor_WalkToAndWait(0, 160, 154);
  Actor_FaceDirection(0, FACING_NORTHWEST, 0);
  Actor_SetAnimationAndWait(18, 4);
  Event_ShowMessage(0x8012, 0);
  Prox_Jump(18, 0x109, 20);
  Event_ShowMessage(0x8012, 0);
  Prox_Jump(11, 0x105, 0);
  Actor_ShowEmote(10, 0x105, 60);
  Actor_ShowEmote(0, 0x109, 20);
  Event_ShowMessage(0xa000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirection(18, FACING_WEST, 40);
  Actor_SetAnimationAndWait(0, 4);
  Event_ShowMessage(0xa000, 0);
  // Waits up to 80 frames, or until a key is held.
  i = 0;
  goto test_wait;
next_wait:
  i++;
test_wait:
  if (i < 80) {
    System_Wait(1);
    if (gInput.held == 0)
      goto next_wait;
  }
  Actor_RunRepeatedMotion(0xc, 1);
  Event_ShowMessage(12, 0);
  Actor_ShowEmote(0, 0x100, 0);
  Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(0, 3);
  Prox_Jump(15, 0x109, 20);
  Event_ShowMessage(15, 0);
  Actor_FaceDirectionAndWait(0, FACING_SOUTH);
  Actor_SetAnimationAndWait(0, 3);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(11, 0);
  Actor_SetAnimation(13, 3);
  Event_ShowMessage(13, 0);
  Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(0, 3);
  Actor_FaceDirection(11, 0x3000, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_SetAnimationAndWait(16, 3);
  Event_ShowMessage(16, 0);
  Actor_FaceDirectionAndWait(0, FACING_SOUTHWEST);
  Actor_ShowEmote(0, 0x108, 20);
  Actor_SetAnimation(14, 3);
  Event_ShowMessage(14, 0);
  Actor_FaceDirectionAndWait(0, FACING_SOUTH);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(17, 4);
  Event_ShowMessage(17, 0);
  Actor_FaceDirection(0, FACING_SOUTHWEST, 20);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(18, 0xb000, 20);
  Actor_ShowEmote(11, 0x109, 40);
  Event_ShowMessage(11, 0);
  Actor_FaceDirection(0, FACING_NORTH, 40);
  Actor_SetAnimationAndWait(10, 3);
  Event_ShowMessage(10, 0);
  Actor_FaceDirectionAndWait(18, 0xd000);
  Actor_SetAnimation(18, 3);
  Event_ShowMessage(0x8012, 0);
  Actor_SetAnimation(11, 3);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTH);
  Event_ShowMessage(0xa000, 0);
  Actor_SetAnimation(10, 3);
  Event_ShowMessage(10, 0);
  Event_ShowMessage(0x8012, 0);
  Actor_SetAnimation(11, 4);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(0, FACING_WEST);
  Prox_MessageOpen(0xa000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  owner = gMainRoutineWork.currentOwner;
  if (result == 0) {
    Actor_WaitForAnimationChange(owner);
    Event_ShowMessage(0xa000, 0);
    EVENT_WORK->messageId++;
  } else {
    FieldActor_WaitForMoveById(owner);
    EVENT_WORK->messageId++;
    Event_ShowMessage(0xa000, 0);
  }
  Prox_Jump(18, 0x108, 0);
  Actor_FaceDirectionAndWait(18, FACING_WEST);
  Event_ShowMessage(0x8012, 0);
  Actor_FaceDirection(11, 0, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_SetAnimation(11, 3);
  Actor_SetAnimationAndWait(10, 3);
  Actor_FaceDirection(11, 0x3000, 0);
  Actor_FaceDirectionAndWait(10, 0x5000);
  Actor_ShowEmote(10, 0x108, 20);
  Event_ShowMessage(10, 0);
  Actor_SetAnimation(11, 3);
  Event_ShowMessage(11, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(0, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(18, FACING_NORTHWEST);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimationAndWait(18, 3);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(13, 0, 0);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirection(15, 0, 0);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(17, 0);
  Actor_SetAnimation(12, 3);
  Actor_SetAnimation(13, 3);
  Actor_SetAnimation(14, 3);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(16, 3);
  Actor_SetAnimationAndWait(17, 3);
  Actor_FaceDirectionAndWait(18, FACING_WEST);
  Event_ShowMessage(0x8012, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirectionAndWait(0, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(0, 3);
  Actor_SetScript(0, (s32)gData_02004a08);
  Actor_RunScriptAndWait(18, (s32)gData_02004a08);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 160, 154);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_RunRepeatedMotion(0xa, 1);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x101, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 20);
  Actor_SetAnimation(10, 4);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(11, 0x100, 20);
  Actor_FaceDirectionAndWait(11, 0);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_SetAnimationAndWait(10, 3);
  Actor_FaceDirectionAndWait(10, 0x5000);
  Event_ShowMessage(10, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirectionAndWait(11, 0x3000);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(11, 0);
  Event_SetFlag(0x9c2);
  Func_0200446c();
}

s32 Func_02004464();
s32 Func_0200446c();

void Func_020015d0(void) {
  struct Actor *object;

  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
  Actor_SetSpeed(20, 0x20000, 0x10000);
  object = Actor_Get(20);
  object->terrainId = 1;
  FieldActor_SetSpritePriority(20, 3);
  object = Actor_Get(gMainRoutineWork.currentOwner);
  object->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x198, 0x146);
  FieldEvent_Wait(1);
  object = Actor_Get(gMainRoutineWork.currentOwner);
  object->actionFlags |= 1;
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  Event_PlaySound(0xa1);
  object = (struct Actor *)((s32)FieldCamera_GetActor());
  object->y += 0x20000;
  object = Actor_Get(20);
  object->actionFlags &= 0xfe;
  Actor_WalkToAndWait(20, 0x198, 0xc8);
  FieldEvent_Wait(1);
  object = Actor_Get(20);
  object->actionFlags |= 1;
  Actor_SetPosition(20, 0, 0);
  Event_PlaySound(0xb0);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  FieldEvent_Wait(30);
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_PlaySound(0x7f);
  FieldEvent_Wait(30);
  Event_PlaySound(0x92);
  FieldEvent_Wait(20);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x105, 40);
  FieldActor_SetSpritePriority(20, 2);
  object = Actor_Get(20);
  object->priorityFlags |= 1;
  MapGrid_CopyCellsTo(0x48, 3, 0x45, 3, 3, 3);
  MapGrid_CopyCellsTo(0x30, 0x24, 0x45, 8, 3, 2);
  MapGrid_CopyCellAttributes(0x48, 0xd, 1, 3, 0x46, 0x14);
  Event_SetFlag(0xa2d);
  Func_0200446c();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02001770(void) {
  if (Event_GetFlag(0xa3d) == 0 && Event_GetFlag(64) == 0) {
    u8 *work = Ram_HeapSlots->eventWork;

    Func_02004464();
    FieldEvent_ResolvePendingActions(1);
    {
      s32 y = 0x80;
      Actor_SetPositionAndFacing(20, PIXELS(408),
                                                     PIXELS(145), y << 7);
    }
    System_Wait(1);
    FieldActor_Jump(20, 4, 20);
    Actor_FaceActor(20, gMainRoutineWork.currentOwner, 0);
    FIELD_AT(work, u16, 362) = 200;
    Func_0200446c();
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020017e4(void) { MapGrid_CopyCellAttributes(0x19, 2, 1, 1, 0x19, 9); }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02001800(void) { MapGrid_CopyCellAttributes(0x19, 3, 1, 1, 0x19, 9); }

struct RisingObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  u8 unk10[8];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[0x44];
  s16 drift;
  u8 unk66[2];
  s32 life;
};

void Func_020043e4(struct RisingObject *object);

void Func_0200181c(struct RisingObject *object) {
  object->x += object->drift << 8;
  object->y += 0x8000;
  object->scaleX += 0x51e;
  object->scaleY += 0x51e;
  object->drift += 2;
  if (--object->life == 0)
    Func_020043e4(object);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02001860(s32 a0, s32 a1, s32 a2) {
  s32 t0;

  t0 = FieldActor_Create(0x11d, a0, a1, a2);
  if (t0 != 0) {
    FIELD_AT((t0 + 0x5c), u8, 0) = 2;
    FIELD_AT((t0 + 0x55), u8, 0) = 0;
    FIELD_AT(((t0 + 0x55) + 0xf), u16, 0) = 0;
    FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
    Actor_SetSpriteColor(t0, 7);
    FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
        ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
    Actor_SetSingleChildFlags(t0, 0);
    FIELD_AT(t0, s32, 24) = 0x8000;
    FIELD_AT(t0, s32, 28) = 0x8000;
    FIELD_AT(t0, s32, 104) = 0x3c;
    FIELD_AT(t0, s32, 108) = (s32)Func_0200181c;
    Call2(Func_020043c4, t0, 5);
    Animation_SetStateFlags(t0, 1);
  }
}

void Func_02001860(s32 x, s32 y, s32 z);

void Func_020018e0(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6a8Room2) {
    if ((gFrameCount & 63) == 0)
      Func_02001860(536 << 16, 40 << 16, 232 << 16);
    if (((gFrameCount + 60) & 63) == 0)
      Func_02001860(168 << 16, 40 << 16, 296 << 16);
    if (((gFrameCount + 20) & 63) == 0) {
      Func_02001860(280 << 16, 42 << 16, 184 << 16);
      Func_02001860(152 << 16, 48 << 16, 424 << 16);
    }
    if (((gFrameCount + 40) & 63) == 0)
      Func_02001860(336 << 16, 42 << 16, 362 << 16);
  } else if (scene == (s32)&SceneId_Ov6a8Room3) {
    if ((gFrameCount & 63) == 0)
      Func_02001860(568 << 16, 104 << 16, 296 << 16);
  }
}
