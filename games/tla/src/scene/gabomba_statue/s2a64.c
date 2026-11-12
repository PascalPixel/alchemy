/// @file
/// Scene code of overlay 0x671, in ROM order, from s2a64's offset up to the next object the overlay links.

#include "types.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_inline.h"
#include "field.h"

extern const u8 gData_02005944[];
extern const u8 gData_02005d7c[];
extern u32 gData_02005ccc[];
extern u32 gData_02005d24[];
extern u32 gData_02005d8c[];
u8 *Unit_Get(s32 owner);
void Func_0200533c(void);
void Func_02005364(void);
void Func_02005724(s32 id, s32 target);
void Func_02001b38(void);
void Func_02001b68(void);
void Func_02002394(void);
void Func_020053bc(const u8 *data);
void Func_02005824(void);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);
void Func_0200582c(s32 index, s32 a, s32 b);
s32 Func_02003604(void);
void Func_020056b4(void);
s32 Func_020048a4(s32 x, s32 z, s32 value, s32 id);
void Func_0200564c(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020057b4(void);
void Func_02004f40(s32 *x_out, s32 *z_out);
void Func_02005624(struct Actor *object, s32 mode);
s32 ArcTan2(s32 y, s32 x);
void Func_02005644(struct Actor *object);

extern u32 gData_02005c40[];

void Event_FinishScreenTransition(void);
void Func_0200581c(void);

static __inline__ void Func_0200578c_ArgsFirst(s32 id, s32 a, s32 b) {
  /* FAKEMATCH(pin): the second argument pinned to r1, so the id is set after
   * it and the third; as calls through a plain forwarding inline, 16 of 521
   * instructions differ in tla-en. */
  register s32 a_ asm("r1") = a;
  s32 b_ = b;

  Func_080d47b4(id, a_, b_);
}

void Func_02002a64(void) {
  s32 speed;
  s32 cue;

  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  {
    /* FAKEMATCH(temporary): the leader's id read into a local before the speed
     * is set; as Actor_SetPositionAndFacing(gMainRoutineWork.currentOwner,
     * ...), 2 of 521 instructions differ in tla-en. */
    s32 owner = gMainRoutineWork.currentOwner;

    speed = 0xc000;
    Actor_SetPositionAndFacing(owner, 174 << 17, 156 << 17, speed);
  }
  Actor_SetPositionAndFacing(7, 165 << 17, 156 << 17, speed);
  {
    /* FAKEMATCH(temporary): actor 23's speed in a local; as the constant in the
     * call, 4 of 521 instructions differ in tla-en. */
    s32 slow = 0xb000;

    Actor_SetPositionAndFacing(23, 216 << 17, 156 << 17, slow);
  }
  Actor_SetPositionAndFacing(6, 224 << 17, 156 << 17, speed);
  Actor_SetPositionAndFacing(5, 234 << 17, 156 << 17, speed);
  System_Wait(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Func_0200578c_ArgsFirst(23, 256, 20);
  FieldEvent_SetMessageId(MsgGabombaUnderstandsChannel);
  Event_ShowMessage(23, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  /* FAKEMATCH(temporary): the else branch's 0x6007 set here in a local, which
   * gives the event work counter r3 and r2 as the native has them; as
   * Event_ShowMessage(0x6007, 0), 20 of 521 instructions differ in
   * tla-en. */
  cue = 0x6007;
  FieldCamera_MoveTo(196 << 17, -1, 146 << 17, 1);
  Func_020057b4();
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirectionAndWait(6, 0x8000);
  Actor_SetAnimation(6, 3);
  Event_ShowMessage(0x4006, 0);
  ((void *)Event_OpenMessage(0x6005, 0));
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Func_0200578c_ArgsFirst(7, 256, 20);
    Actor_SetSpeed(7, 0x20000, 0x10000);
    Actor_WalkToAndWait(7, 340, 296);
    Actor_WalkToAndWait(7, 354, 296);
    Actor_WalkToAndWait(7, 364, 308);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xe000, 0);
    Actor_FaceDirectionAndWait(7, 0x6000);
    Event_ShowMessage(0x6007, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Actor_SetSpeed(7, 0x20000, 0x10000);
    Actor_WalkToAndWait(7, 340, 296);
    Actor_WalkToAndWait(7, 354, 296);
    Actor_WalkToAndWait(7, 364, 308);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xe000, 0);
    Actor_FaceDirectionAndWait(7, 0x6000);
    Actor_SetAnimation(7, 3);
    Event_ShowMessage(cue, 0);
  }
  Actor_FaceDirectionAndWait(23, 0x8000);
  Actor_SetAnimationAndWait(23, 4);
  Event_ShowMessage(0x6017, 0);
  Actor_FaceDirection(7, 0, 0);
  Func_0200578c_ArgsFirst(7, 257, 40);
  Event_ShowMessage(0x6017, 0);
  Actor_SetAnimationAndWait(7, 3);
  Func_0200578c_ArgsFirst(23, 265, 20);
  Event_ShowMessage(0x6017, 0);
  Event_ShowMessage(0x4007, 0);
  Func_0200578c_ArgsFirst(7, 256, 40);
  Event_ShowMessage(0x4007, 0);
  Actor_SetAnimation(23, 3);
  Event_ShowMessage(0x4017, 0);
  Func_0200578c_ArgsFirst(7, 265, 10);
  Event_ShowMessage(0x4007, 0);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_WalkToAndWait(6, 440, 296);
  Actor_FaceDirectionAndWait(6, 0x8000);
  Actor_SetAnimationAndWait(6, 3);
  Event_ShowMessage(0x4006, 0);
  Actor_FaceDirection(7, 0xe000, 0);
  Func_0200578c_ArgsFirst(7, 261, 0);
  Actor_FaceDirection(6, 0x2000, 0);
  Actor_FaceDirection(5, 0xa000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 80);
  Func_0200578c_ArgsFirst(7, 261, 40);
  Actor_FaceDirectionAndWait(7, 0);
  Event_ShowMessage(0x4007, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirectionAndWait(6, 0x8000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(23, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(6, 3);
  Actor_SetAnimationAndWait(7, 3);
  Actor_FaceDirectionAndWait(23, 0xb000);
  Event_ShowMessage(23, 0);
  Func_0200578c_ArgsFirst(23, 257, 0);
  Actor_FaceDirection(23, 0xd000, 20);
  Actor_FaceDirectionAndWait(23, 0xb000);
  Event_ShowMessage(23, 0);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(196 << 17, -1, 176 << 16, 1);
  Func_020057b4();
  Event_ShowMessage(23, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(23, 0);
  Event_ShowMessage(7, 0);
  Event_ShowMessage(0x2005, 0);
  FieldCamera_MoveTo(196 << 17, -1, 146 << 17, 1);
  Func_020057b4();
  Actor_SetAnimation(23, 3);
  Event_ShowMessage(23, 0);
  Actor_FaceDirectionAndWait(23, 0x8000);
  Event_ShowMessage(23, 0);
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetSpeed(6, 0x19999, 0xcccc);
  Actor_SetSpeed(23, 0x19999, 0xcccc);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetScript(7, gData_02005c40);
  Actor_SetScript(23, gData_02005c40);
  Actor_SetScript(6, gData_02005c40);
  Actor_RunScriptAndWait(5, gData_02005c40);
  Event_SetFlag(0x8f6);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200581c();
  FieldEvent_SetExitRequest(13);
}

#include "scene_tables.h"

void Func_02002f70(void) {
  s32 index = Func_02003604();
  s32 x;
  s32 z;
  struct Actor *spark;
  struct Actor *actor;
  s32 fast;
  s32 slow;

  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_020048a4(19, 11, gData_02005d8c[index], 16);
  Actor_SetAnimation(15, 5);
  Actor_SetPosition(11, 0, 0);
  Actor_SetPosition(12, 0, 0);
  Actor_SetPosition(13, 0, 0);
  Actor_SetPosition(14, 0, 0);
  Actor_SetPosition(16, 0, 0);
  Actor_SetPosition(17, 0, 0);
  Actor_SetPosition(18, 0, 0);
  Actor_SetPosition(19, 0, 0);
  Actor_SetPosition(20, 0, 0);
  Actor_SetPosition(21, 0, 0);
  Actor_SetPosition(22, 0, 0);
  System_Wait(1);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(182), 0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
  fast = 0x4000;
  Actor_SetPositionAndFacing(5, PIXELS(472), PIXELS(216), fast);
  slow = 0x3000;
  Actor_SetPositionAndFacing(24, PIXELS(472), PIXELS(200), slow);
  System_Wait(1);
  Func_0200564c();
  System_Wait(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x202;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_SetSpeed(0x13333, 0x2666);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(312), 1);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(24, 0x10000, 0x8000);
  Actor_SetScript(5, gData_02005ccc);
  Actor_RunScriptAndWait(24, gData_02005d24);
  FieldEvent_SetMessageId(MsgGabombaSawEnergyFlow);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(24, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_WalkToAndWait(5, 440, 296);
  Actor_FaceDirectionAndWait(5, FACING_EAST);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  FieldCamera_MoveTo(PIXELS(414), -1, PIXELS(272), 1);
  Func_020057b4();
  Event_ShowMessage(5, 0);
  Func_02004f40(&x, &z);
  x = (x << 20) + (156 << 17);
  z = (z << 20) + (184 << 16);
  spark = (FieldActor_Create(434, x, 0, z));
  spark->motionFlags |= 4;
  Actor_SetSingleChildFlags(spark, 0);
  Func_02005624(spark, 1);
  System_Wait(1);
  actor = Actor_Get(5);
  actor->angle = ArcTan2(spark->z - actor->z, spark->x - actor->x);
  FieldCamera_MoveTo(spark->x, -1, spark->z, 1);
  Func_020057b4();
  Event_ShowMessage(5, 0);
  Func_02005644(spark);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(304), 1);
  Func_020057b4();
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  Event_ShowMessage(5, 0);
  Actor_SetAttachedEffect(24, 258);
  FieldEvent_Wait(20);
  Event_ShowMessage(24, 0);
  Actor_SetPositionAndFacing(gMainRoutineWork.currentOwner, PIXELS(472),
                             PIXELS(204), fast);
  Actor_SetPositionAndFacing(7, PIXELS(440), PIXELS(204), fast);
  Actor_SetPositionAndFacing(23, PIXELS(456), PIXELS(204), slow);
  Actor_SetPositionAndFacing(6, PIXELS(488), PIXELS(204),
                                                 fast);
  System_Wait(1);
  Event_ShowMessage(23, 0);
  Actor_FaceDirection(24, FACING_NORTH + FACING_STEP, 0);
  FieldCamera_MoveTo(PIXELS(440), -1, PIXELS(264), 1);
  Func_020057b4();
  FieldActor_Jump(24, 4, 10);
  Event_ShowMessage(0x4018, 0);
  Actor_SetAnimationAndWait(7, 4);
  Event_ShowMessage(0x8007, 0);
  Actor_ShowEmote(24, 257, 40);
  Event_ShowMessage(0x8006, 0);
  Actor_SetAnimation(23, 4);
  Event_ShowMessage(0x9017, 0);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(60);
  Event_ClearFlag(265);
  System_Wait(60);
  FieldEvent_SetExitRequest(18);
}

void Func_02003338(void) {
  Func_0200533c();
  Func_02005724(23, gMainRoutineWork.currentOwner);
  Actor_SetSpriteColor(Actor_Get(23), 15);
  System_Wait(10);
  FieldEvent_SetMessageId(MsgGabombaChannelTiming);
  Event_ShowMessage(23, 0);
  Actor_SetPosition(23, 0, 0);
  System_Wait(1);
  Event_SetFlag(0x9ec);
  Func_02005364();
}

#include "scene_ids.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Func_02001c10(void);
void Func_020020f0(s32 value);
void Func_020034ec(void);
void Func_020034f4(void);
void Func_0200359c(void);
void Func_020035f0(void);
void Func_02003634(void);
void Func_02003868(void);

/// Prepares the current room.
s32 Func_02003398(void) {
  s16 scene;

  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 521;
  if (gMainRoutineWork.scene != (s32)&SceneId_Ov671Room1) {
    if (Event_GetFlag(515))
      Func_020020f0(0);
    else if (Event_GetFlag(0x8cd))
      Func_020020f0(-1);
    else
      Func_020020f0(1);
    Func_02001c10();
  }
  Actor_Get(FieldParty_GetLeaderActor())->priorityFlags |= 0x20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov671Room1) {
    Func_020034ec();
  } else if (scene == (s32)&SceneId_Ov671Room2) {
    MapGrid_CopyCellAttributes(15, 68, 8, 3, 15, 4);
    Func_020034f4();
  } else if (scene == (s32)&SceneId_Ov671Room3) {
    Func_0200359c();
  } else if (scene == (s32)&SceneId_Ov671Room5) {
    Func_020035f0();
  } else if (scene == (s32)&SceneId_Ov671Room6) {
    Func_02003634();
  } else if (scene == (s32)&SceneId_Ov671Room7) {
    Func_02003868();
  }
  return 0;
}

s32 Func_02003494(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov671Room7 &&
      gMainRoutineWork.entrance == 1 && !Event_GetFlag(0x109)) {
    Event_ClearFlag(0x8cd);
    Event_ClearFlag(0x8ce);
    Event_ClearFlag(0x8cf);
  }
  return 0;
}

void Func_020034ec(void) { Func_02001b38(); }

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Exit_Set(s32 index, s32 flag, s32 a, s32 b) {
  Func_0200583c(index, flag, a, b);
}

void Func_020034f4(void) {
  if ((u16)(gMainRoutineWork.entrance - 3) <= 1) {
    Actor_SetScript(27, gData_02005944);
    Actor_SetScript(28, gData_02005944);
    MapGrid_CopyCellAttributes(33, 24, 3, 3, 27, 29);
  }
  Func_020053bc(gData_02005d7c);
  if (!Event_GetFlag(0x8ce) || !Event_GetFlag(0x8cf))
    Actor_Get(26)->y = PIXELS(16);
  Func_02005824();
  Exit_Set(0, 513, 8, 9);
  Exit_Set(1, 514, 10, 11);
  Func_02001b68();
}

void Func_0200359c(void) {
  Func_02005824();
  Func_0200583c(0, 513, 31, 32);
  if (!Event_GetFlag(0x109)) {
    s32 entrance = (u16)gMainRoutineWork.entrance;

    if ((u16)(entrance - 7) <= 1)
      Func_02002394();
    if (gMainRoutineWork.entrance == 7)
      Event_SetFlag(0x204);
  }
}

void Func_020035f0(void) {
  Func_02005824();
  ((s32 (*)(s32, s32, s32))Func_0200582c)(0, 8, 9);
}

s32 Func_02003604(void) {
  u8 *state = Unit_Get(gMainRoutineWork.currentOwner);
  s32 sum = 0;
  u32 i;

  for (i = 0; i < 14; i++)
    sum += state[i];
  return sum & 15;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Event_StartScreenTransition();
void Func_0200253c();
void Func_020027cc();
void Func_020052a4();
void Func_020053ac();
void Func_020056bc();
void Func_02001ac0();
void Func_02002074();

void Func_02003634(void) {
  s32 t1, t4, t6, t8, t10, t12, t14, t16, t18, t20, t22, t24, t26, t27, t29,
      t30, t42, t43, t46, t50, t53, t59, t60, p_r3, v_r5;
  const u8 *table;

  t1 = ((s32)Actor_Get(10));
  Actor_SetSpriteColor(t1, 3);
  Task_AddFrame((s32)Func_02002074, 0x480);
  t4 = ((s32)Actor_Get(11));
  Actor_SetSpriteColor(t4, 11);
  t6 = ((s32)Actor_Get(12));
  Actor_SetSpriteColor(t6, 11);
  t8 = ((s32)Actor_Get(13));
  Actor_SetSpriteColor(t8, 10);
  t10 = ((s32)Actor_Get(14));
  Actor_SetSpriteColor(t10, 10);
  t12 = ((s32)Actor_Get(16));
  Actor_SetSingleChildFlags(t12, 0);
  t14 = ((s32)Actor_Get(17));
  Actor_SetSingleChildFlags(t14, 0);
  t16 = ((s32)Actor_Get(18));
  Actor_SetSingleChildFlags(t16, 0);
  t18 = ((s32)Actor_Get(19));
  Actor_SetSingleChildFlags(t18, 0);
  t20 = ((s32)Actor_Get(20));
  Actor_SetSingleChildFlags(t20, 0);
  t22 = ((s32)Actor_Get(21));
  Actor_SetSingleChildFlags(t22, 0);
  t24 = ((s32)Actor_Get(22));
  Actor_SetSingleChildFlags(t24, 0);
  t26 = Event_GetFlag(0x8fb);
  if (t26 == 0)
    goto L_0200b6f2;
  t27 = Event_GetFlag(0x8fd);
  if (t27 != 0)
    goto L_0200b6f2;
  Func_02002f70();
  goto L_0200b856;
L_0200b6f2:
  t29 = Event_GetFlag(0x8fc);
  if (t29 == 0)
    goto L_0200b762;
  t30 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  v_r5 = t30;
  Actor_SetPosition(11, 0, 0);
  Actor_SetPosition(12, 0, 0);
  Actor_SetPosition(13, 0, 0);
  Actor_SetPosition(14, 0, 0);
  Func_020053ac();
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(FIELD_AT(v_r5, s32, 8), -1, FIELD_AT(v_r5, s32, 16), 0);
  Func_0200564c();
  System_Wait(1);
  Func_020056bc();
L_0200b762:
  t42 = Event_GetFlag(0x8f6);
  if (t42 == 0)
    goto L_0200b78e;
  t43 = Event_GetFlag(0x8fd);
  if (t43 != 0)
    goto L_0200b78e;
  Func_020052a4(0);
  Task_AddFrame((s32)Func_02001ac0, 0x480);
L_0200b78e:
  t46 = Func_02003604();
  /* FAKEMATCH(temporary): the table's address in a local set after the call,
   * which makes it the load's base register; as ((u8 *)gData_02005d8c)'s address
   * added in place, 2 of 211 instructions differ in tla-en. */
  table = ((u8 *)gData_02005d8c);
  Func_020048a4(19, 11, *(s32 *)(table + (t46 << 2)), 16);
  Actor_SetAnimation(15, 5);
  if (FIELD_AT(&gMainRoutineWork, s16, 482) != 2)
    goto L_0200b7c0;
  Func_020027cc();
  goto L_0200b856;
L_0200b7c0:
  t50 = Event_GetFlag(0x8f6);
  if (t50 != 0)
    goto L_0200b7e4;
  if (FIELD_AT(&gMainRoutineWork, s16, 482) != 1) {
    p_r3 = FIELD_AT(&gMainRoutineWork, s16, 482);
    goto L_0200b7dc;
  }
  Func_0200253c();
  goto L_0200b7e4;
L_0200b7dc:
  if (p_r3 != 3)
    goto L_0200b7e4;
  Func_02002a64();
L_0200b7e4:
  t53 = Event_GetFlag(0x8fc);
  if (t53 == 0)
    goto L_0200b856;
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x209;
  Event_ClearFlag(0x8fc);
  t59 = Event_GetFlag(0x9eb);
  if (t59 == 0)
    goto L_0200b84a;
  t60 = Event_GetFlag(0x9ec);
  if (t60 != 0)
    goto L_0200b84a;
  Func_02003338();
L_0200b84a:
  Event_ClearFlag(0x12f);
  Func_020056bc();
L_0200b856:
}

#include "heap_release.h"
#include "resource.h"

extern struct SceneState gSceneState;
extern const u8 gField_CachedTilesA[];
void Func_020055cc(const u8 *data, void *buffer);

void Func_02003868(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (gMainRoutineWork.entrance == 1) {
    Event_SetFlag(0xa27);
    Event_ClearFlag(0xa28);
  }
}

struct SceneState {
  u8 unk000[0x168];
  u16 tileEntry;
};

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void *Mem_Alloc(s32 size) { return Heap_Alloc(size); }

void Func_020038ac(void) {
  struct SceneState *state = &gSceneState;
  void *buffer = Mem_Alloc(0xc00);
  s32 handle;

  Func_020055cc(gField_CachedTilesA, buffer);
  handle = Resource_FindFreeEntry();
  VramCache_Load(handle, 0xc00, buffer);
  state->tileEntry = handle;
  Heap_Release(buffer);
}

s32 Func_020038f4(u32 x, u32 z) {
  u8 *cells = ((u8 *)&gSceneState);

  if (x > 10 || z > 7)
    return 0;
  {
    u8 *cell = cells + (z * 11 + x) * 4;
    s32 code;

    if (cell[3] != 0 || cell[1] != 0)
      return 1;
    code = cell[0];
    code ^= 10;
    return 1 - ((u32)(-code | code) >> 31);
  }
}
s32 Func_02003934(s32 x, s32 z) {
  s32 found;

  if (Func_020038f4(x, z - 1) || Func_020038f4(x, z + 1) ||
      Func_020038f4(x - 1, z))
    return 1;
  found = Func_020038f4(x + 1, z);
  return (u32)(-found | found) >> 31;
}
