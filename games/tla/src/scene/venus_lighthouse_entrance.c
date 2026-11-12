/// @file
/// The scene code of overlay 0x64d that its scene tables belong to, in ROM order; the rest of the overlay is in venus_lighthouse_entrance/.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "event_inline.h"

void Func_020023a4(void *object);
void Func_0200260c(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002a34(s32 a, s32 b);
void Func_02002a44(void);
void Func_02002a4c(void);
void Func_02002a54(void);
void Func_02002a5c(struct Actor *object, s32 mode);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void VenusLighthouseEntrance_RunActorSparkBurst(void *object) {
  Func_02002a34(8, 0x4084);
  Func_02002a44();
  Func_02002a5c(Actor_Get(5), 2);
  FieldEvent_Wait(20);
  Func_020023a4(object);
  Func_02002a5c(Actor_Get(5), 0);
  Func_02002a54();
  Func_02002a4c();
  Field_StartPaletteTransition(8);
}

void VenusLighthouseEntrance_UpdateSparkTrail(struct Actor *object) {
  Func_0200260c(object, object->x, object->y, object->z);
  if (++object->action % 8 == 0)
    Event_PlaySound(125);
}

void Func_020026f4(void);
void VenusLighthouseEntrance_StopSparks(void);

/// The spark trail while the actor steps 96 out and 96 back.
void Func_020000c8(s32 actor) {
  struct Actor *object = Actor_Get(actor);

  Func_020026f4();
  object->update = VenusLighthouseEntrance_UpdateSparkTrail;
  object->priorityFlags = 0;
  {
    s32 a = actor;
    /* FAKEMATCH(pin): the 96 pinned to r1; as
     * Actor_CommitPositionAndActivate(a, 96, dz), the pin below kept, 2 of 35
     * instructions differ in tla-en. */
    register s32 dx asm("r1") = 96;
    /* FAKEMATCH(pin): the -96 pinned to r2, the same call's second; as
     * Actor_CommitPositionAndActivate(actor, 96, -96) with neither pin nor the
     * copy, 23 of 35 instructions differ in tla-en. */
    register s32 dz asm("r2") = -96;

    Actor_CommitPositionAndActivate(a, dx, dz);
  }
  {
    /* FAKEMATCH(pin): the arguments pinned in order, so the -96 is negated
     * after the actor is copied; as Actor_CommitPositionAndActivate(
     * actor, -96, 96), 2 of 35 instructions differ in tla-en. */
    register s32 a asm("r0") = actor;
    s32 dx = -96;

    Actor_CommitPositionAndActivate(a, dx, 96);
  }
  object->update = 0;
  object->priorityFlags = 1;
  FieldEvent_Wait(30);
  VenusLighthouseEntrance_StopSparks();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

#include "ram_buffer.h"
#include "main_routine_work.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020028b4(void);
void Func_0200030c(void);
void Func_020028bc(void);
s32 Func_020028a4();
void Func_02001c30(void);
s32 Party_RemoveActiveOwner();
s32 Func_0200299c();
s32 Func_02002a04();

extern char SceneId_VenusLighthouseEntrance;

s32 Func_02000128(void) {
  s32 t37;
  s32 t0;
  s32 t7;
  s32 t10;
  s32 t21;
  s32 t33;

  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x100;
  t0 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  FIELD_AT((t0 + 0x23), u8, 0) = (0x20 | FIELD_AT((t0 + 0x23), u8, 0));
  Actor_SetAnimation(10, 5);
  FieldActor_SetSpritePriority(10, 1);
  Actor_SetAnimation(11, 5);
  FieldActor_SetSpritePriority(11, 1);
  FieldActor_SetSpritePriority(14, 2);
  FieldActor_SetSpritePriority(19, 2);
  t7 = Event_GetFlag(0x892);
  if (t7 != 0) {
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
  }
  t10 = Event_GetFlag(0x890);
  if (t10 != 0) {
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(21, 0, 0);
  }
  if (gMainRoutineWork.entrance == 9) {
    t21 = Event_GetFlag(0x891);
    if (t21 == 0) {
      Func_020028b4();
      FieldEvent_ResolvePendingActions(0);
      Func_0200030c();
      Func_020028bc();
    }
  }
  if (gMainRoutineWork.entrance == 0x62) {
    gMainRoutineWork.secondary = 1;
    gMainRoutineWork.entrance = 1;
    Func_020028a4(1);
    Func_02001c30();
  }
  if (gMainRoutineWork.entrance == 0x63) {
    Event_SetFlag(0x893);
  }
  if (gMainRoutineWork.entrance == 0x5a) {
    gMainRoutineWork.currentOwner = 5;
    Party_RemoveActiveOwner(4);
    Party_RemoveActiveOwner(6);
    Party_RemoveActiveOwner(7);
    Func_0200299c((s32)&SceneId_VenusLighthouseEntrance, 1);
  }
  t33 = Event_GetFlag(0x893);
  if (t33 == 0) {
    Actor_SetAnimation(22, 5);
    FieldActor_SetSpritePriority(22, 1);
  } else {
    Actor_SetPosition(22, 40 << 16, 248 << 16);
    t37 = ((s32)Actor_Get(22));
    FIELD_AT(t37, u16, 6) = 0x4000;
    Func_02002a04(0x16, 0x4000, 0);
    FieldActor_SetSpritePriority(22, 3);
    Actor_SetActionCallback(Actor_Get(22), 8);
  }
  return 0;
}

#include "call.h"

s32 Scene_PrepareMap(void) { return 0; }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_02002864();
void Func_0200298c(void);
extern u8 gData_0200b000[];

void Func_0200030c(void) {
  s32 t1;
  s32 t3;
  s32 t9;
  s32 t16;

  Event_SetFlag(0x891);
  t1 = ((s32)Actor_Get(10));
  FIELD_AT((t1 + 0x55), u8, 0) = 0;
  FIELD_AT(t1, s32, 72) = 0x28f;
  FIELD_AT(t1, s32, 48) = 0x18000;
  FIELD_AT(t1, s32, 52) = 0x8000;
  FieldActor_SetPositionAndResetMotion(t1, PIXELS(216), PIXELS(96),
                                       PIXELS(176));
  t3 = ((s32)Actor_Get(11));
  FIELD_AT((t3 + 0x55), u8, 0) = 0;
  FIELD_AT(t3, s32, 72) = 0x7ae;
  FIELD_AT(t3, s32, 48) = 0x18000;
  FIELD_AT(t3, s32, 52) = 0x8000;
  FieldActor_SetPositionAndResetMotion(t3, PIXELS(616), PIXELS(96),
                                       PIXELS(152));
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(376), -1, PIXELS(248), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  t9 = ((s32)Actor_Get(10));
  Actor_SetAnimation(10, 4);
  Event_PlaySound(165);
  FieldActor_SetMoveTarget(t9, PIXELS(352), PIXELS(8), PIXELS(176));
  Func_02002864(t9, (s32)gData_0200b000);
  FieldEvent_Wait(25);
  FieldCamera_MoveTo(PIXELS(376), -1, PIXELS(104), 1);
  t16 = ((s32)Actor_Get(11));
  Actor_SetAnimation(11, 4);
  FieldActor_SetMoveTarget(t16, PIXELS(416), PIXELS(8), PIXELS(152));
  Func_02002864(t16, (s32)gData_0200b000);
  FieldEvent_Wait(60);
  Event_PlaySound(165);
  FieldActor_WaitForMoveById(10);
  FieldActor_WaitForMoveById(11);
  Func_0200298c();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gData_02003040[];

void Func_02000440(void) {
  s32 t7;
  s32 t11;

  Event_SetFlag(0x892);
  Func_020028b4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(165);
  Actor_SetAnimation(10, 3);
  FieldActor_SetAnimation(11, 3);
  FieldEvent_Wait(21);
  t7 = Actor_Get(10);
  FIELD_AT((t7 + 0x55), u8, 0) = 0;
  FIELD_AT(t7, s32, 48) = 0x20000;
  FIELD_AT(t7, s32, 52) = 0x8000;
  Actor_SetAnimation(10, 4);
  FieldActor_SetMoveTarget(t7, PIXELS(616), PIXELS(96), PIXELS(176));
  Func_02002864(t7, (s32)gData_02003040);
  t11 = Actor_Get(11);
  FIELD_AT((t11 + 0x55), u8, 0) = 0;
  FIELD_AT(t11, s32, 48) = 0x20000;
  FIELD_AT(t11, s32, 52) = 0x8000;
  Actor_SetAnimation(11, 4);
  FieldActor_SetMoveTarget(t11, PIXELS(120), PIXELS(96), PIXELS(176));
  Call2(Func_02002864, t11, (s32)gData_02003040);
  Func_020028bc();
}

void Func_02002994(s32 id, s32 useSetter);
extern u8 MsgThereTheyAre[];

struct SparkRecord {
  u8 unk00[100];
  u16 delay;
  u8 unk66[6];
  s32 update;
};

void Func_02000510(void) {
  u32 i;
  s32 p10;
  s32 p8;
  s32 rec;
  u8 *record;
  s32 v5;
  s32 callback;

  Event_SetFlag(0x890);
  Func_020028b4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgThereTheyAre);
  Actor_SetSpeed(12, 0x10000, 0x8000);
  Actor_SetSpeed(13, 0x10000, 0x8000);
  FieldEvent_ShowMessageThenWait(12, 0, 0);
  Actor_WalkToAndWait(5, 0x178, 0x118);
  FieldEvent_Wait(10);
  Actor_ShowEmote(5, 0x101, 40);
  Actor_FaceDirection(5, 0x6000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(5, 0x2000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(5, 0x4000, 0);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(20);
  Actor_WalkTo(5, 0x178, 0x168);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(0x1780000, -1, 0x1a80000, 1);
  Func_0200298c();
  FieldActor_WaitForMoveById(5);
  FieldEvent_Wait(10);
  Actor_ShowEmote(12, 0x102, 40);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(13, 0, 10);
  Actor_ShowEmote(17, 0x107, 30);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  FieldActor_Jump(18, 6, 15);
  FieldActor_Jump(18, 6, 25);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  Actor_FaceDirection(14, 0x8000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(14, 0xa000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(14, 0x8000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(14, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_RunRepeatedMotion(15, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_FaceDirection(19, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(19, 0, 10);
  Actor_SetAnimationAndWait(20, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(20, 0, 10);
  Actor_SetAnimationAndWait(19, 3);
  FieldEvent_Wait(10);
  Actor_FaceDirection(15, 0xe000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(16, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_SetAnimationAndWait(16, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(16, 0, 10);
  Actor_WalkByAndWait(16, 64, 0);
  Actor_SetPosition(16, 0, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(20, 0xa000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(21, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(20, 0, 10);
  Actor_SetAnimationAndWait(21, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(21, 0, 10);
  Actor_WalkByAndWait(21, -64, 64);
  Actor_SetPosition(21, 0, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(14, 0xa000, 0);
  Actor_FaceDirection(15, 0xa000, 0);
  Actor_FaceDirection(19, 0xe000, 0);
  Actor_FaceDirection(20, 0xe000, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(0x1780000, -1, 0x1680000, 1);
  Func_0200298c();
  FieldEvent_Wait(10);
  FieldActor_LaunchFromLeader(8, -16, -16, 0x4000);
  FieldActor_LaunchFromLeader(9, 16, -16, 0x4000);
  FieldActor_WaitForMoveById(8);
  FieldEvent_Wait(20);
  Actor_SetSpeed(9, 0x10000, 0x8000);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(8, 0, 16);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(8, 0, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_FaceDirection(5, 0x8000, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(5, 0x4000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x2005, 0, 10);
  Actor_ShowEmote(9, 0x101, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_WalkByAndWait(9, 0, 16);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0x8000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(5, 0x102, 60);
  Actor_FaceDirection(5, 0x4000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_SetSpeed(9, 0x8000, 0x4000);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(9, 0, 16);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(5, 0x2000, 0);
  Actor_FaceDirection(8, 0x2000, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(8, 0x101, 30);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_FaceDirection(9, 0xa000, 0);
  FieldEvent_Wait(20);
  ((void (*)())FieldMessage_ShowAtActor)(9, 0);
  FieldEvent_Wait(10);
  if (FieldEvent_AskYesNoWithActorAnimation(5, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_FaceDirection(5, 0x8000, 0);
    Actor_FaceDirection(8, 0, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(5, 3);
    FieldEvent_Wait(30);
    Actor_FaceDirection(5, 0x2000, 0);
    Actor_FaceDirection(8, 0x2000, 0);
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(8, 0, 10);
    *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) += 1;
  } else {
    FieldEvent_Wait(30);
    Actor_FaceDirection(8, 0, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimationAndWait(8, 4);
    *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) += 1;
    FieldEvent_ShowMessageThenWait(8, 0, 10);
    Actor_FaceDirection(8, 0x2000, 0);
  }
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, 0x5000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(5, 0x6000, 0);
  Actor_FaceDirection(8, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(0x1380000, -1, 0x1a80000, 1);
  Func_0200298c();
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_FaceDirection(17, 0x8000, 0);
  Actor_FaceDirection(18, 0, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(17, 0xe000, 0);
  Actor_FaceDirection(18, 0xe000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_ShowEmote(18, 0x107, 30);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  FieldCamera_MoveTo(0x1780000, -1, 0x1a80000, 1);
  Func_0200298c();
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, 0x2000, 0);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  Actor_FaceDirection(8, 0x2000, 0);
  Actor_FaceDirection(5, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  FieldCamera_MoveTo(0x1780000, -1, 0x1680000, 1);
  Func_0200298c();
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0x8000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(5, 0x102, 40);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  FieldCamera_MoveTo(0x1780000, -1, 0x1980000, 1);
  Func_0200298c();
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, 0x3000, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(9, 0x8000, 0x4000);
  Actor_WalkByAndWait(9, 0, 16);
  Actor_FaceDirection(9, 0x2000, 0);
  Actor_FaceDirection(17, 0xf000, 0);
  Actor_FaceDirection(18, 0xf000, 0);
  Actor_FaceDirection(19, 0xf000, 0);
  Actor_FaceDirection(20, 0xf000, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(12, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_SetAnimationAndWait(9, 4);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(13, 0, 10);
  Actor_FaceDirection(9, 0x9000, 0);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(9, 0x2000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(12, 0, 0);
  Actor_FaceDirection(13, 0x8000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(12, 3);
  Actor_SetAnimationAndWait(13, 3);
  FieldEvent_Wait(10);
  Actor_FaceDirection(12, 0xa000, 0);
  Actor_FaceDirection(13, 0xa000, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_SetSpeed(13, 0x13333, 0x9999);
  Actor_WalkByAndWait(12, -26, 8);
  Actor_FaceDirection(12, 0xb000, 0);
  Actor_WalkByAndWait(13, -16, -12);
  Actor_FaceDirection(13, 0x8000, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimation(9, 8);
  FieldEvent_Wait(50);
  Func_02002a44();
  record = ((u8 *)Actor_GetById(9));
  Func_02002a5c((s32)record, 2);
  Event_PlaySound(178);
  FieldEvent_Wait(20);
  Func_02002a34(8, 0x4084);
  FieldEvent_Wait(60);
  Event_PlaySound(0x186);
  FieldActor_SetSpritePriority(12, 0);
  Func_020023a4(12);
  Actor_ShowEmote(17, 0x100, 0);
  Actor_ShowEmote(18, 0x100, 0);
  Actor_ShowEmote(19, 0x100, 0);
  Actor_ShowEmote(20, 0x100, 0);
  Actor_ShowEmote(13, 0x100, 0);
  Actor_ShowEmote(14, 0x100, 0);
  Actor_ShowEmote(15, 0x100, 50);
  FieldActor_SetSpritePriority(13, 1);
  Func_020023a4(13);
  FieldEvent_Wait(30);
  record = ((u8 *)Actor_GetById(9));
  Func_02002a5c((s32)record, 0);
  Func_02002a54();
  Func_02002a4c();
  Field_StartPaletteTransition(16);
  FieldEvent_Wait(10);
  Actor_SetAnimation(9, 9);
  FieldEvent_Wait(30);
  Actor_ShowEmote(14, 0x102, 40);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_RunRepeatedMotion(15, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  FieldActor_StartRepeatedMotion(14, 3);
  Actor_SetAttachedEffect(14, 0x102);
  FieldActor_StartRepeatedMotion(15, 3);
  Actor_SetAttachedEffect(15, 0x102);
  FieldEvent_Wait(50);
  Actor_FaceDirection(14, 0x8000, 0);
  Actor_FaceDirection(15, 0x8000, 0);
  FieldActor_Jump(14, 6, 15);
  FieldActor_Jump(14, 6, 25);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_FaceDirection(5, 0x6000, 0);
  Actor_FaceDirection(8, 0x5000, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(9, 0x108, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkByAndWait(9, -16, 16);
  Actor_FaceDirection(9, 0x6000, 0);
  FieldCamera_MoveTo(0x1680000, -1, 0x1980000, 1);
  Func_0200298c();
  FieldActor_StartRepeatedMotion(17, 3);
  Actor_SetAttachedEffect(17, 0x102);
  FieldActor_StartRepeatedMotion(18, 3);
  Actor_SetAttachedEffect(18, 0x102);
  FieldActor_StartRepeatedMotion(19, 3);
  Actor_SetAttachedEffect(19, 0x102);
  FieldActor_StartRepeatedMotion(20, 3);
  Actor_SetAttachedEffect(20, 0x102);
  FieldActor_StartRepeatedMotion(21, 3);
  Actor_SetAttachedEffect(21, 0x102);
  FieldEvent_Wait(50);
  Actor_SetAnimationAndWait(17, 4);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_ShowEmote(18, 0x102, 40);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimationAndWait(19, 4);
  FieldEvent_ShowMessageThenWait(19, 0, 10);
  FieldActor_StartRepeatedMotion(20, 3);
  Actor_SetAttachedEffect(20, 0x102);
  FieldEvent_Wait(55);
  Actor_FaceDirection(17, 0x8000, 0);
  Actor_FaceDirection(18, 0, 0);
  Actor_FaceDirection(19, 0x5000, 0);
  Actor_FaceDirection(20, 0xe000, 0);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(20, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(20, 0, 10);
  FieldActor_Jump(18, 6, 0);
  FieldActor_Jump(19, 6, 0);
  FieldActor_Jump(20, 6, 25);
  Func_020026f4();
  Actor_SetSpeed(20, 0x1cccc, 0xe666);
  Actor_SetSpeed(19, 0x1cccc, 0xe666);
  Actor_SetSpeed(18, 0x1cccc, 0xe666);
  Actor_SetSpeed(17, 0x1cccc, 0xe666);
  v5 = 0;
  rec = ((u8 *)Actor_GetById(20));
  callback = (s32)VenusLighthouseEntrance_UpdateSparkTrail;
  ((struct SparkRecord *)rec)->delay = v5;
  {
    s32 next = 19;

    ((struct SparkRecord *)rec)->update = callback;
    rec = (s32)((u8 *)Actor_GetById(next));
  }
  ((struct SparkRecord *)rec)->delay = v5;
  {
    s32 next = 18;

    ((struct SparkRecord *)rec)->update = callback;
    rec = (s32)((u8 *)Actor_GetById(next));
  }
  ((struct SparkRecord *)rec)->delay = v5;
  {
    s32 next = 20;

    ((struct SparkRecord *)rec)->update = callback;
    Actor_SetAnimation(next, 10);
  }
  Actor_SetAnimation(19, 10);
  Actor_SetAnimation(18, 10);
  Actor_SetDestinationOffset(20, -24, 24);
  Actor_SetDestinationOffset(19, -24, 24);
  Actor_SetDestinationOffset(18, -24, 24);
  FieldActor_WaitForMoveById(18);
  Actor_SetDestination(20, 224, 0x1e0);
  Actor_SetDestination(19, 224, 0x1e0);
  Actor_MoveToAndWait(18, 224, 0x1e0);
  rec = (s32)((u8 *)Actor_GetById(20));
  ((struct SparkRecord *)rec)->update = v5;
  rec = (s32)((u8 *)Actor_GetById(19));
  ((struct SparkRecord *)rec)->update = v5;
  rec = (s32)((u8 *)Actor_GetById(18));
  ((struct SparkRecord *)rec)->update = v5;
  FieldEvent_Wait(1);
  FieldActor_Jump(17, 6, 15);
  FieldActor_Jump(17, 6, 25);
  rec = ((u8 *)Actor_GetById(17));
  p10 = rec + 100;
  ((struct SparkRecord *)rec)->delay = v5;
  {
    s32 next = 17;

    ((struct SparkRecord *)rec)->update = callback;
    Actor_SetAnimation(next, 10);
  }
  Actor_SetDestinationOffset(17, -20, 20);
  FieldActor_WaitForMoveById(17);
  FieldActor_Jump(17, 4, 0);
  Actor_SetDestinationOffset(17, -20, 20);
  FieldActor_WaitForMoveById(17);
  Actor_SetAnimation(17, 1);
  Event_PlaySound(133);
  *(s32 *)(rec + 8) += 0x80000;
  ((struct SparkRecord *)rec)->update = v5;
  *(s32 *)(rec + 16) += -0x80000;
  {
    s32 shown = 0xc000;

    ((struct {
      u8 pad[6];
      u16 v;
    } *)rec)
        ->v = shown;
  }
  {
    s32 target = *(s32 *)(rec + 80);
    s32 shown = 0xa000;

    *(u16 *)(target + 18) = shown;
  }
  FieldEvent_Wait(55);
  *(s32 *)(rec + 8) += -0x40000;
  *(s32 *)(rec + 16) += 0x40000;
  {
    s32 shown = 0x4000;
    s32 target = *(s32 *)(rec + 80);

    ((struct {
      u8 pad[18];
      u16 v;
    } *)target)
        ->v = v5;
    ((struct {
      u8 pad[6];
      u16 v;
    } *)rec)
        ->v = shown;
  }
  FieldActor_Jump(17, 6, 40);
  Actor_SetAnimation(17, 11);
  Event_PlaySound(135);
  FieldEvent_Wait(14);
  Event_PlaySound(135);
  FieldEvent_Wait(14);
  Event_PlaySound(135);
  FieldEvent_Wait(14);
  Event_PlaySound(135);
  FieldEvent_Wait(14);
  FieldEvent_Wait(20);
  ((struct SparkRecord *)rec)->delay = v5;
  {
    s32 next = 17;

    ((struct SparkRecord *)rec)->update = callback;
    Actor_SetAnimation(next, 10);
  }
  Actor_MoveToAndWait(17, 224, 0x1d8);
  FieldEvent_Wait(30);
  ((struct SparkRecord *)rec)->update = v5;
  Actor_SetPosition(20, 0, 0);
  Actor_SetPosition(19, 0, 0);
  Actor_SetPosition(18, 0, 0);
  Actor_SetPosition(17, 0, 0);
  VenusLighthouseEntrance_StopSparks();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 4);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  FieldCamera_MoveTo(0x1780000, -1, 0x1980000, 1);
  Func_0200298c();
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0xb000, 0);
  {
    s32 a0 = 5;

    /* FAKEMATCH(dead): an empty once-only block before the angle's load; as the
     * plain call, 2 of 2142 instructions differ in tla-en. */
    do {
    } while (0);
    Actor_FaceDirection(a0, 0x4000, 0);
  }
  Actor_FaceDirection(8, 0x3000, 0);
  FieldEvent_Wait(15);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(14, 0x2000, 0);
  Actor_FaceDirection(15, 0xa000, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(14, 0x8000, 0);
  Actor_FaceDirection(15, 0x8000, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(14, 0x18000, 0xc000);
  Actor_WalkByAndWait(14, -32, 0);
  record = ((u8 *)Actor_GetById(9));
  {
    s32 shown = 0x2000;

    *(u16 *)((s32)record + 6) = shown;
  }
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  Actor_SetSpeed(9, 0x18000, 0xc000);
  Actor_WalkByAndWait(9, 0, 16);
  Actor_FaceDirection(9, 0, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  FieldEvent_Wait(10);
  FieldActor_SetSpritePriority(14, 3);
  Actor_SetSpeed(14, 0x20000, 0x10000);
  *(u8 *)(((u8 *)Actor_GetById(14)) + 90) &= 254;
  Actor_WalkToAndWait(14, 0x1c8, 0x1a8);
  FieldEvent_Wait(1);
  /* FAKEMATCH(dead): an empty once-only block ends CSE's block, so the 1 of
   * these six sets stays one register; as no block, 80 of 2142 instructions
   * differ in tla-en. */
  do {
  } while (0);
  *(u8 *)(((u8 *)Actor_GetById(14)) + 90) |= 1;
  Actor_FaceDirection(14, 0x6000, 0);
  Actor_SetSpeed(14, 0x10000, 0x8000);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  Actor_SetSpeed(9, 0xb333, 0x5999);
  Actor_WalkByAndWait(9, 48, 0);
  Actor_FaceDirection(9, 0x1000, 0);
  Actor_FaceDirection(5, 0x2000, 0);
  Actor_FaceDirection(8, 0x2000, 0);
  Actor_SetSpeed(15, 0x10000, 0x8000);
  *(u8 *)(((u8 *)Actor_GetById(15)) + 90) &= 254;
  Actor_WalkToAndWait(15, 0x1c8, 0x1b8);
  FieldEvent_Wait(1);
  *(u8 *)(((u8 *)Actor_GetById(15)) + 90) |= 1;
  FieldEvent_Wait(10);
  Actor_ShowEmote(9, 0x108, 30);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  Actor_RunRepeatedMotion(14, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_WalkByAndWait(9, 16, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(9, 0x107, 30);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  FieldActor_StartRepeatedMotion(14, 2);
  Actor_RunRepeatedMotion(15, 2);
  FieldEvent_Wait(20);
  Actor_SetSpeed(14, 0x8000, 0x4000);
  Actor_SetSpeed(15, 0x8000, 0x4000);
  *(u8 *)(((u8 *)Actor_GetById(14)) + 90) &= 254;
  Actor_WalkToAndWait(14, 0x1e8, 0x1a8);
  FieldEvent_Wait(1);
  *(u8 *)(((u8 *)Actor_GetById(14)) + 90) |= 1;
  *(u8 *)(((u8 *)Actor_GetById(15)) + 90) &= 254;
  Actor_WalkToAndWait(15, 0x1e8, 0x1b8);
  FieldEvent_Wait(1);
  *(u8 *)(((u8 *)Actor_GetById(15)) + 90) |= 1;
  Actor_WalkToAndWait(9, 0x1c8, 0x1b0);
  Actor_FaceDirection(9, 0, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1009, 0, 10);
  FieldActor_StartRepeatedMotion(14, 3);
  Actor_SetAttachedEffect(14, 0x102);
  FieldActor_StartRepeatedMotion(15, 3);
  Actor_SetAttachedEffect(15, 0x102);
  FieldEvent_Wait(50);
  *(u8 *)(((u8 *)Actor_GetById(14)) + 90) &= 254;
  Actor_WalkToAndWait(14, 0x1f8, 0x1a8);
  FieldEvent_Wait(1);
  *(u8 *)(((u8 *)Actor_GetById(14)) + 90) |= 1;
  Actor_SetPosition(14, 0, 0);
  *(u8 *)(((u8 *)Actor_GetById(15)) + 90) &= 254;
  Actor_WalkToAndWait(15, 0x1f8, 0x1b8);
  FieldEvent_Wait(1);
  {
    u8 *flags = ((u8 *)Actor_GetById(15)) + 90;
    u8 v = *flags | 1;

    *flags = v;
  }
  Actor_SetPosition(15, 0, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkByAndWait(9, 64, 0);
  Actor_SetPosition(9, 0, 0);
  FieldEvent_Wait(30);
  Func_02002994(5, 1);
  Func_0200298c();
  FieldEvent_Wait(10);
  Actor_ShowEmote(8, 0x105, 30);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  FieldEvent_Wait(10);
  FieldActor_Jump(8, 4, 0);
  Actor_FaceDirection(8, 0, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_ShowEmote(5, 0x105, 50);
  Actor_FaceDirection(5, 0x8000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2005, 0, 10);
  Actor_ShowEmote(8, 0x101, 30);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_ShowEmote(5, 0x109, 80);
  FieldEvent_Wait(10);
  {
    s32 a1 = 0x106;
    s32 a2 = 40;
    s32 a0 = 8;

    Actor_ShowEmote(a0, a1, a2);
  }
  Event_OpenMessage(8, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(5, 0) == 0) {
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(8, 0, 10);
    *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) += 1;
  } else {
    FieldEvent_Wait(40);
    *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) += 1;
    FieldEvent_ShowMessageThenWait(8, 0, 10);
  }
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(8, 0x5000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 10);
  Actor_FaceDirection(8, 0, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Actor_SetAnimation(8, 2);
  record = (s32)Actor_Get(5);
  if ((s32)record != 0) {
    Actor_SetDestination(8, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
  }
  FieldActor_WaitForMoveById(8);
  Actor_SetPosition(8, 0, 0);
  FieldCamera_FollowActor(5, 1);
  Func_020028bc();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_020029a4();
extern u8 MsgWhyDoI[];

void Func_02001a2c(void) {
  Func_020028b4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgWhyDoI);
  Actor_WalkToAndWait(5, 72, 266);
  Actor_FaceDirection(5, 0x8000, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 0xa);
  FieldActor_SetAnimation(22, 1);
  FieldEvent_Wait(10);
  Actor_ShowEmote(22, 0x101, 50);
  FieldEvent_Wait(10);
  Actor_FaceDirection(22, 0, 0);
  FieldEvent_Wait(20);
  FieldActor_Jump(22, 6, 15);
  FieldActor_Jump(22, 6, 35);
  Actor_FaceDirection(22, 0xb000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(22, 0xc);
  FieldEvent_ShowMessageThenWait(22, 0, 0);
  Actor_SetAnimation(22, 1);
  Actor_ShowEmote(22, 0x100, 50);
  Actor_FaceDirection(22, 0, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(22, 0, 0xa);
  FieldActor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(22, 0x10000, 0x8000);
  Actor_WalkByAndWait(22, 0x10, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 0xa);
  Actor_SetSpeed(22, 0x20000, 0x10000);
  FieldActor_Jump(22, 6, 15);
  FieldActor_Jump(22, 6, 25);
  FieldActor_Jump(22, 6, 0);
  Actor_WalkBy(22, 0x1a, 0);
  FieldEvent_Wait(8);
  Actor_SetAnimation(22, 1);
  Event_SetAfterBattleDestination((s32)&SceneId_VenusLighthouseEntrance, 0x63);
  Event_SetDefeatDestination((s32)&SceneId_VenusLighthouseEntrance, 0x62);
  gMainRoutineWork.battleRule24b = 2;
  Call2(Func_020029a4, 9, 0);
  Func_020028bc();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 MsgNotThatWayThe[];

void Func_02001bb0(void) {
  s32 t7;

  Func_020028b4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgNotThatWayThe);
  FieldActor_LaunchFromLeader(8, 0x10, 0, 0x8000);
  FieldActor_WaitForMoveById(8);
  FieldEvent_ShowMessageThenWait(0x2008, 0, 0xa);
  Actor_SetAnimation(8, 2);
  t7 = ((s32)Actor_Get(5));
  if (t7 != 0) {
    Actor_SetDestination(8, FIELD_AT(t7, s16, 10), FIELD_AT(t7, s16, 18));
  }
  FieldActor_WaitForMoveById(8);
  Actor_SetPosition(8, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(5, -16, 0);
  Func_020028bc();
}

#include "iwram_call.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 MsgAreAreYou[];

void Func_02001c30(void) {
  s32 t96;

  Func_020028b4();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(5, PIXELS(364), PIXELS(156));
  Actor_SetPosition(8, PIXELS(362), PIXELS(142));
  Actor_SetPosition(9, PIXELS(390), PIXELS(144));
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(8, 0x3000, 0);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldActor_SetSpritePriority(8, 2);
  Actor_SetSpriteRotationById(5, 0x4000);
  Actor_SetSingleChildFlags(((s32)Actor_Get(5)), 0);
  Func_02002994(5, 0);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1b4), s32, 0) = 0x3c;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)MsgAreAreYou);
  Actor_ShowEmote(8, 0x102, 40);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(30);
  Actor_SetPosition(5, PIXELS(376), PIXELS(164));
  Actor_SetSpriteRotationById(5, 0);
  Actor_SetSingleChildFlags(((s32)Actor_Get(5)), 1);
  FieldActor_Jump(5, 6, 50);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(5, -14, -2);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, 0, 0);
  Actor_FaceDirection(5, 0xf000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_SetAnimationAndWait(9, 4);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkByAndWait(9, 0, 0x10);
  Actor_FaceDirection(9, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0xa);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(5, 9);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkBy(9, 0, 0x78);
  FieldEvent_Wait(40);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkByAndWait(8, 0x18, 0);
  Actor_WalkByAndWait(8, 0, 0x14);
  Actor_FaceDirection(5, 0x3000, 0);
  FieldActor_WaitForMoveById(9);
  Actor_SetPosition(9, 0, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(8, 0x105, 30);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(20);
  FieldActor_Jump(8, 4, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 0xa);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Actor_SetAnimation(8, 2);
  t96 = ((s32)Actor_Get(5));
  if (t96 != 0) {
    Actor_SetDestination(8, FIELD_AT(t96, s16, 10),
                              FIELD_AT(t96, s16, 18));
  }
  FieldActor_WaitForMoveById(8);
  Actor_SetPosition(8, 0, 0);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1b4), s32, 0) = 8;
  Func_020028bc();
}

struct GlowState {
  s32 x; ///< 16.16 pixels
  s32 y; ///< 16.16 pixels
  s32 z; ///< 16.16 pixels
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 frame;
};

struct GlowSprite {
  u8 unk00[4];
  u16 attr0 : 13;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct GlowWork {
  u8 unk000[0x400];
  struct GlowSprite sprites[16];
  struct GlowState states[16];
  struct Actor *object;
  struct Actor *effect;
  s32 x;
  s32 y;
  s32 z;
  s16 stage;
  s16 timer;
  u16 tileTop;
  u16 tile;
  s16 height;
  s8 follow;
};

void Func_02002a14(struct GlowSprite *sprite, struct GlowState *state);

void Func_02001fd0(void) {
  struct GlowWork *work = Ram_HeapSlots->menuRuntime;
  struct Actor *object = work->object;
  struct Actor *effect = work->effect;
  s32 i, y;
  s32 scale;
  s32 frameStep;
  s32 mask;
  struct GlowState *state;
  struct GlowSprite *sprite;

  switch (work->stage) {
  case 0:
    if (work->timer == 0) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 1:
    work->height += 4;
    if (work->height > 31) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 2:
    if (work->timer == 90) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 3:
    if (work->timer == 0) {
      object->motionFlags = 3;
      object->velocityY = 0x180000;
      work->follow = 0;
      Sound_Play(134);
    }
    work->height += 8;
    if (work->height > 79) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 4: {
    s16 *timer = &work->timer;

    if (work->timer == 10) {
      object->z = 0;
      object->x = 0;
    }
    if (*timer == 30) {
      work->stage++;
      work->timer = -1;
    }
    break;
  }
  case 5:
    work->height -= 8;
    if (work->height <= 0) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 6: {
    s16 *timer;

    scale = effect->scaleX;
    if (scale > 0)
      scale -= 0x51e;
    else
      scale = 0;
    effect->scaleX = scale;
    effect->scaleY = scale;
    /* FAKEMATCH(dead): an empty once-only block after the scale's stores, which
     * the timer's address follows; as no block, 2 of 449 instructions differ in
     * tla-en. */
    do {
    } while (0);
    timer = &work->timer;
    if (work->timer == 10)
      Sound_Play(310);
    if (*timer == 90) {
      work->stage++;
      work->timer = -1;
    }
    break;
  }
  case 7: {
    s16 *timer = &work->timer;

    if (work->timer == 0) {
      Sound_Play(144);
      MapGrid_SetShake(0x30000, 0x30000, 0x10000);
    }
    if (*timer == 30) {
      MapGrid_SetShake(-1, -1, 0xe666);
      if (work->timer == 30) {
        work->stage = 999;
        work->timer = -1;
      }
    }
    break;
  }
  }
  object->angle += 0x1000;
  if (work->height > 0) {
    for (i = 0, y = 0; i < 16 && y < work->height - 8; i++, y += 16) {
      state = &work->states[i];
      state->x = work->x;
      state->y = work->y + (y << 16) + 0x80000;
      state->z = work->z;
      sprite = &work->sprites[i];
      state->frame++;
      frameStep = state->frame >> 2;
      mask = 3;
      sprite->tile = work->tileTop + (frameStep & mask) * 8;
      Func_02002a14(sprite, state);
    }
    if (i <= 15) {
      state = &work->states[i];
      state->x = work->x;
      state->y = work->y + (work->height << 16);
      state->z = work->z;
      state->frame++;
      state->y += Value2((s32 (*)())Iwram_MulQ16, 0xfffe0000,
                         Math_Sin(state->frame << 12)) +
                  0xfffe0000;
      sprite = &work->sprites[i];
      sprite->tile = work->tile + ((state->frame >> 2) & 3) * 8;
      Func_02002a14(sprite, state);
      if (work->follow != 0)
        object->y = state->y;
    }
  }
  work->timer++;
}
