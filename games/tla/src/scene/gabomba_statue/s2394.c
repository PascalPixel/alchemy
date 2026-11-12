/// @file
/// Scene code of overlay 0x671, in ROM order, from s2394's offset up to the next object the overlay links.

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"

void Func_020056b4(void);
void Func_0200564c(void);
s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_020056bc(void);

void Func_02002394(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;

  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_0200564c();
  System_Wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  i = 0;
  do {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
    velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
    Func_020000b8(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                  velocity[2], 0x1090001, &options);
    i++;
  } while (i <= 16);
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  System_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Func_020056bc();
}

#include "text/msg_ids.h"
#include "scene_tables.h"

struct Wanderer {
  u8 unk00[98];
  u8 timer;
};

extern u32 gData_02005af4[];
extern u32 gData_02005b1c[];
extern u32 gData_02005b60[];
extern u32 gData_02005ba4[];
extern u32 gData_02005be8[];
extern s32 gData_02005c74[];
extern s32 gData_02005ca0[];
void Func_02005724(s32 id, s32 target);
void Func_020057b4(void);
void Func_02000f6c(void);
s32 Func_02004d48(s32 a, s32 b);

static __inline__ void Func_0200578c_ArgsFirst(s32 id, s32 a, s32 b) {
  /* FAKEMATCH(pin): the second argument pinned to r1, so the id is set after
   * it and the third; as calls through a plain forwarding inline, 16 of 521
   * instructions differ in tla-en. */
  register s32 a_ asm("r1") = a;
  s32 b_ = b;

  Func_080d47b4(id, a_, b_);
}

void Func_0200253c(void) {
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  FieldEvent_SetMessageId(MsgGabombaFocusEnergies);
  Event_ShowMessage(24, 0);
  Func_02005724(23, gMainRoutineWork.currentOwner);
  Func_02005724(5, gMainRoutineWork.currentOwner);
  Func_02005724(6, gMainRoutineWork.currentOwner);
  Func_02005724(7, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetSpeed(23, 0xcccc, 0x6666);
  Actor_SetScript(23, gData_02005ba4);
  System_Wait(10);
  Actor_SetScript(gMainRoutineWork.currentOwner, gData_02005af4);
  Actor_SetScript(5, gData_02005b1c);
  Actor_SetScript(7, gData_02005b60);
  FieldActor_RefreshSelectorById(23);
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(0x13333, 0x2666);
  FieldCamera_MoveTo(PIXELS(434), -1, PIXELS(286), 1);
  Actor_SetSpeed(23, 0x13333, 0x9999);
  Actor_RunScriptAndWait(23, gData_02005be8);
  FieldEvent_Wait(40);
  Actor_FaceDirectionAndWait(23, FACING_NORTH + FACING_STEP);
  Event_ShowMessage(23, 0);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(434), -1, PIXELS(168), 1);
  Func_020057b4();
  FieldEvent_Wait(20);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 40);
  Actor_Get(7)->facing = FACING_SOUTH;
  Actor_Get(gMainRoutineWork.currentOwner)->facing = 0x4000;
  Actor_Get(6)->facing = FACING_SOUTH;
  Actor_Get(5)->facing = FACING_SOUTH;
  Actor_Get(7)->actionFlags &= 0xfe;
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_Get(6)->actionFlags &= 0xfe;
  Actor_Get(5)->actionFlags &= 0xfe;
  Actor_WalkTo(7, 400, 272);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 420, 272);
  Actor_WalkTo(6, 444, 272);
  Actor_WalkTo(5, 468, 272);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(80);
  System_Wait(80);
  FieldEvent_SetExitRequest(11);
}

void Func_020027cc(void) {
  s32 speed;

  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Event_PlaySound(78);
  speed = 0x4000;
  Actor_SetPositionAndFacing(gMainRoutineWork.currentOwner, PIXELS(348),
                             PIXELS(312), speed);
  Actor_SetPositionAndFacing(7, PIXELS(330), PIXELS(312), speed);
  {
    s32 slow = 0x3000;

    Actor_SetPositionAndFacing(23, PIXELS(432), PIXELS(312), slow);
  }
  Actor_SetPositionAndFacing(6, PIXELS(448), PIXELS(312), speed);
  Actor_SetPositionAndFacing(5, PIXELS(468), PIXELS(312),
                                                 speed);
  System_Wait(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x202;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Event_PlaySound(220);
  System_Wait(1);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(40);
  System_Wait(80);
  Func_02000f6c();
  System_Wait(1);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(10);
  System_Wait(20);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 256, 0);
  Func_0200578c_ArgsFirst(5, 256, 0);
  Func_0200578c_ArgsFirst(6, 256, 0);
  Func_0200578c_ArgsFirst(7, 256, 0);
  Func_0200578c_ArgsFirst(23, 256, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirection(23, FACING_NORTHWEST + FACING_STEP, 60);
  Event_PlaySound(130);
  Actor_SetSpriteColor(Actor_Get(9), 7);
  Actor_SetSpriteColor(Actor_Get(10), 7);
  System_Wait(8);
  Actor_SetSpriteColor(Actor_Get(10), 3);
  Actor_SetSpriteColor(Actor_Get(9), 0);
  System_Wait(8);
  ((struct Wanderer *)Actor_Get(gMainRoutineWork.currentOwner))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_Get(7))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_Get(5))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_Get(6))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_Get(23))->timer =
      (Random16Far() * 40 >> 16) + 10;
  Actor_SetScript(gMainRoutineWork.currentOwner, gData_02005c74);
  Actor_SetScript(7, gData_02005c74);
  Actor_SetScript(5, gData_02005ca0);
  Actor_SetScript(6, gData_02005ca0);
  Actor_SetScript(23, gData_02005ca0);
  Event_PlaySound(11);
  Func_02004d48(11, 0);
  FieldEvent_Wait(80);
  Event_SetFlag(0x8f7);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(12);
}

#include "field.h"

extern const u8 gData_02005944[];
extern const u8 gData_02005d7c[];
extern u32 gData_02005ccc[];
extern u32 gData_02005d24[];
extern u32 gData_02005d8c[];
void *Unit_Get(s32 owner);
void Func_0200533c(void);
void Func_02005364(void);
void Func_02001b38(void);
void Func_02001b68(void);
void Func_020053bc(const u8 *data);
void Func_02005824(void);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);
void Func_0200582c(s32 index, s32 a, s32 b);
s32 Func_02003604(void);
s32 Func_020048a4(s32 x, s32 z, s32 value, s32 id);
void Func_02004f40(s32 *x_out, s32 *z_out);
void Func_02005624(struct Actor *object, s32 mode);
s32 ArcTan2(s32 y, s32 x);
void Func_02005644(struct Actor *object);

extern u32 gData_02005c40[];

void Func_0200581c(void);

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
  actor->facing = ArcTan2(spark->z - actor->z, spark->x - actor->x);
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
void Func_020052a4();
void Func_020053ac();
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

struct Sprite {
  u8 b0, b1, b2, b3, b4;
  u8 mode : 2;
  u8 b5rest : 6;
  u8 b6;
  u8 b7a : 1;
  u8 affine : 5;
  u8 b7b : 2;
  u32 w8;
};

struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

struct SceneState {
  struct PuzzleCell cells[88];
  s32 count;
  s16 firstId;
  s16 endId;
  s16 tileEntry;
  s8 redrawA;
  s8 redrawB;
  s16 originX;
  s16 originZ;
  s16 statuesReady;
  u8 unk172[2];
  s32 statueOffset;
  struct Sprite oam[10];
};

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

void Func_02003974(void) {
  struct SceneState *state = &gSceneState;
  u32 row;
  u32 col;

  for (row = 0; row <= 7; row++)
    for (col = 0; col <= 10; col++) {
      struct PuzzleCell *cell = &state->cells[row * 11 + col];
      s32 occupied = cell->occupied;

      if (Event_GetFlag(0x8fb) && cell->occupied == 2) {
        cell->occupied = 1;
        occupied = 1;
      }
      if (occupied == 2)
        MapGrid_CopyCells(0, 11, 1, 1, state->originX + col,
                          state->originZ + row);
      else if (occupied == 1)
        MapGrid_CopyCells(1, state->cells[row * 11 + col].kind, 1, 1,
                          state->originX + col, state->originZ + row);
      else
        MapGrid_CopyCells(occupied, state->cells[row * 11 + col].kind, 1, 1,
                          state->originX + col, state->originZ + row);
    }
}

#include "iwram_call.h"

s32 Func_02003fc0(s32 x0, s32 z0, s32 x1, s32 z1, s32 a, s32 b, s32 c, s32 d);
void Func_02004270(s32 col, s32 row, s32 a, s32 b);
s32 Func_02004380(s32 col, s32 row, s32 turn);

s32 Func_02003a64(s32 value) {
  struct SceneState *state = &gSceneState;
  s32 row, col, n, straight, corner, cross, tries, left, mode;
  s32 piece, shape;
  struct PuzzleCell *cell;

  state->count = value - 1;
  /* FAKEMATCH(dead): the form kept; as no such test, 580 of 629 instructions
   * differ in tla-en. */
  {
    s32 mark = state->cells[0].occupied;
    if (mark & 0x100) {
      if (value == 1)
        goto draw_next;
      if (value == 2)
        goto count_next;
      if (value == 3)
        goto turn_next;
      if (value == 4)
        goto start_miss;
      if (value == 5)
        goto corner_miss;
      if (value == 6)
        goto straight_miss;
      if (value == 7)
        goto plain_miss;
    }
  }
retry:
  state->count += 1;
  gRandomState = state->count;
  Iwram_ClearWords(state, sizeof(state->cells));
  state->cells[3 * 11 + 5].kind = 10;
  state->cells[7 * 11 + 1].kind = 10;
  state->cells[7 * 11 + 9].kind = 10;
  state->cells[7 * 11 + 0].blocked = 1;
  state->cells[7 * 11 + 2].blocked = 1;
  state->cells[7 * 11 + 8].blocked = 1;
  state->cells[7 * 11 + 10].blocked = 1;
  row = 0;
draw_row:
  col = 0;
draw_col:
  /* FAKEMATCH(temporary): the form kept; as plain arguments, 231 of 629
   * instructions differ in tla-en. */
  {
    s32 drawn = state->cells[row * 11 + col].kind;
    s32 drawZ, drawX = state->originX;
    drawZ = state->originZ + row;
    Map_CopyCellsRect(0, drawn, 1, 1, drawX + col, drawZ);
  }
draw_next:
  if (++col <= 10)
    goto draw_col;
  if (++row <= 7)
    goto draw_row;
  n = 0;
  if (Random16Far() < 0x8000) {
    n |= Func_02003fc0(2, 7, 5, 7, 4, 0, 3, 2);
    n |= Func_02003fc0(8, 7, 6, 6, 4, 2, 2, 3);
    Func_02004270(5, 6, 9, 12);
    n |= Func_02003fc0(4, 6, 4, 3, 12, 2, 0, 2);
    n |= Func_02003fc0(5, 5, 5, 2, 14, 3, 1, 3);
  } else {
    n |= Func_02003fc0(2, 7, 4, 6, 4, 0, 0, 2);
    n |= Func_02003fc0(8, 7, 5, 7, 4, 2, 3, 3);
    Func_02004270(5, 6, 8, 12);
    n |= Func_02003fc0(5, 5, 4, 3, 12, 3, 0, 2);
    n |= Func_02003fc0(6, 6, 5, 2, 14, 0, 1, 3);
  }
  if (n != 0)
    goto retry;
  n = Func_02003fc0(0, 7, 5, 4, 19, 2, 3, 0);
  n |= Func_02003fc0(10, 7, 6, 3, 17, 0, 2, 1);
  if (n != 0)
    goto retry;
  straight = 0;
  corner = 0;
  cross = 0;
  row = 0;
count_row:
  col = 0;
count_col:
  piece = state->cells[row * 11 + col].kind;
  shape = state->cells[row * 11 + col].state;
  if (shape == 5)
    straight++;
  if (shape == 10)
    corner++;
  if (shape == 12 && (u32)(piece - 8) <= 1)
    cross++;
count_next:
  if (++col <= 10)
    goto count_col;
  if (++row <= 7)
    goto count_row;
  if (straight == 0)
    goto retry;
  if (corner == 0)
    goto retry;
  if (cross == 0)
    goto retry;
  mode = (Random16Far() * 2) >> 16;
  left = 7;
  tries = 1;
start_try:
  col = (Random16Far() * 11) >> 16;
  row = ((Random16Far() * 3) >> 16) + 5;
  if (Func_02003934(col, row) == 0) {
    piece = state->cells[row * 11 + col].kind;
    if ((u32)(piece - 1) <= 1) {
      shape = state->cells[row * 11 + col].state;
      if ((mode == 0 && (shape & 6) != 0) || (mode == 1 && (shape & 9) != 0)) {
        state->cells[row * 11 + col].kind = 10;
        state->cells[row * 11 + col].occupied = 1;
        left = 6;
        goto start_done;
      }
    }
  }
start_miss:
  if (++tries <= 88)
    goto start_try;
start_done:
  n = 3;
corner_next:
  tries = 1;
corner_try:
  col = (Random16Far() * 11) >> 16;
  row = (Random16Far() * 8) >> 16;
  if (Func_02003934(col, row) == 0) {
    piece = state->cells[row * 11 + col].kind;
    if (piece == 3 || piece == 8 || piece == 9) {
      cell = &state->cells[row * 11 + col];
      shape = cell->state;
      if (shape != 6 && shape != 9) {
        switch (piece) {
        case 3:
          cell->kind = ((Random16Far() * 2) >> 16) + 1;
          break;
        case 8:
          cell->kind = (((Random16Far() * 2) >> 16) << 1) + 4;
          break;
        case 9:
          cell->kind = (((Random16Far() * 2) >> 16) << 1) + 5;
          break;
        }
        state->cells[row * 11 + col].occupied = 1;
        left--;
        goto corner_done;
      }
    }
  }
corner_miss:
  if (++tries <= 88)
    goto corner_try;
corner_done:
  if (--n >= 0)
    goto corner_next;
  tries = 1;
straight_try:
  col = (Random16Far() * 11) >> 16;
  row = (Random16Far() * 8) >> 16;
  if (Func_02003934(col, row) == 0) {
    piece = state->cells[row * 11 + col].kind;
    if (piece == 4 || piece == 6 || piece == 5 || piece == 7) {
      state->cells[row * 11 + col].occupied = 1;
      left--;
      goto straight_done;
    }
  }
straight_miss:
  if (++tries <= 88)
    goto straight_try;
straight_done:
  if (left > 0) {
    n = left;
  plain_next:
    tries = 1;
  plain_try:
    col = (Random16Far() * 11) >> 16;
    row = (Random16Far() * 8) >> 16;
    if (Func_02003934(col, row) == 0) {
      piece = state->cells[row * 11 + col].kind;
      if ((u32)(piece - 1) <= 1) {
        state->cells[row * 11 + col].occupied = 1;
        goto plain_done;
      }
    }
  plain_miss:
    if (++tries <= 88)
      goto plain_try;
  plain_done:
    if (--n != 0)
      goto plain_next;
  }
  row = 0;
turn_row:
  col = 0;
turn_col:
  piece = state->cells[row * 11 + col].kind;
  shape = state->cells[row * 11 + col].occupied;
  if (shape != 0) {
    switch ((Random16Far() * 8) >> 16) {
    case 0:
    case 1:
    case 2:
      piece = Func_02004380(col, row, 0);
      break;
    case 3:
    case 4:
    case 5:
      piece = Func_02004380(col, row, 1);
      break;
    case 6:
      Func_02004380(col, row, 0);
      piece = Func_02004380(col, row, 0);
      break;
    }
  }
  /* FAKEMATCH(temporary): the form kept; as plain arguments, 12 of 629
   * instructions differ in tla-en. */
  {
    s32 drawZ, drawX = state->originX;
    drawZ = state->originZ + row;
    Map_CopyCellsRect(shape, piece, 1, 1, drawX + col, drawZ);
  }
turn_next:
  if (++col <= 10)
    goto turn_col;
  if (++row <= 7)
    goto turn_row;
  return state->count;
}

extern u8 Data_02006fc8[], Data_02006fe8[], Data_02007008[], Data_02007028[],
    Data_02007048[], gData_02007068[];
extern s8 gData_02005ac0[];
extern u8 gData_02005ac8[];
s32 abs(s32);
s32 Func_02004224(s32 from, s32 to, s32 kind);
// Lays a random path of `length` cells across the Gabomba Statue grid (gSceneState,
// 11 by 8 cells of four bytes) from (x0, z0) to (endX, endZ), backtracking through
// per-step stacks (Data_02006fc8 to gData_02007068: direction, x, z, kind, tried
// directions and flags) when a cell has no free neighbour; each step joins the
// cells (Func_02004224 picks the piece, Func_02004270 writes it). Returns 0, or -1
// when the parity of the distance rules a path out or the search gives up.
s32 Func_02003fc0(s32 x0, s32 z0, s32 endX, s32 endZ, s32 length, s32 direction,
                  s32 last, s32 bit) {
  u8 *cells, *kinds;
  s32 piece;
  s32 spare, step, dir, used, dz, index, one;
  u32 x, z, nextX, nextZ, tries, attempts;
  u8 tried;

  length--;
  dz = endZ - z0;
  spare = length - abs(endX - x0) - abs(dz);
  Iwram_ClearWords(Data_02006fc8, 32);
  Iwram_ClearWords(Data_02006fe8, 32);
  Iwram_ClearWords(Data_02007008, 32);
  Iwram_ClearWords(Data_02007028, 32);
  Iwram_ClearWords(Data_02007048, 32);
  Iwram_ClearWords(gData_02007068, 32);
  if (spare & 1)
    return -1;
  x = x0;
  cells = (u8 *)((struct PuzzleCell *)&gSceneState);
  one = 1;
  step = 0;
  attempts = 0;
  z = z0;
  while (1) {
    if (++attempts > 1280)
      return -1;
    {
      s32 offset = (z * 11 + x) * 4;
      Data_02007028[step] = cells[offset];
      gData_02007068[step] = ((struct PuzzleCell *)(cells + offset))->state;
    }
    Data_02006fe8[step] = x;
    Data_02007008[step] = z;
    Data_02006fc8[step] = direction;
    {
      u8 *joins = gData_02005ac8 + 1;
      kinds = Data_02007028;
      Data_02007048[step] = Data_02007048[step] | joins[kinds[step]] |
                            1 << ((direction + 2) & 3);
    }
    tries = 0;
    while (1) {
      u32 value;
      if (++tries > 63)
        goto back;
      value = Random16Far();
      dir = direction;
      if (value > 0x7fff)
        dir = (Random16Far() * 4) >> 16;
      tried = Data_02007048[step];
      if ((tried >> dir) & one)
        continue;
      {
        s8 *steps = gData_02005ac0 + 1;
        index = dir * 2;
        nextX = x + steps[index];
        index += 1;
        nextZ = z + steps[index];
      }
      if (nextX > 10 || nextZ > 7)
        goto blocked;
      if (step == length - 1 && (nextX != endX || nextZ != endZ))
        goto blocked;
      {
        u8 *cell = cells + (nextZ * 11 + nextX) * 4;
        if (cell[1] != 0)
          goto blocked;
        used = (cell[2] >> bit) & one;
        if (used != 0)
          goto blocked;
        if (cell[0] == 10)
          goto blocked;
      }
      tried |= one << dir;
      Data_02007048[step] = tried;
      kinds = Data_02007028;
      piece = Func_02004224(direction, dir, kinds[step]);
      {
        s32 offset = (z * 11 + x) * 4;
        Func_02004270(x, z, piece, (cells + offset)[2] | one << bit);
      }
      step++;
      x = nextX;
      z = nextZ;
      direction = dir;
      if (step == length)
        goto done;
      Data_02007048[step] = 0;
      goto top;
    blocked:
      Data_02007048[step] |= 1 << dir;
      if (Data_02007048[step] != 0x0f)
        continue;
    back:
    Data_02007048[step] = 0;
    if (step == 0)
      return -1;
    step--;
    x = Data_02006fe8[step];
    z = Data_02007008[step];
    direction = Data_02006fc8[step];
    Func_02004270(x, z, Data_02007028[step], gData_02007068[step]);
    goto top;
    }
    top:;
  }
done:
  {
    s32 offset = (nextZ * 11 + nextX) * 4;
    piece = Func_02004224(dir, last, cells[offset]);
    Func_02004270(nextX, nextZ, piece, (cells + offset)[2] | 1 << bit);
  }
  return 0;
}

extern const u8 gData_02005ad4[];
extern const u8 gData_02006ea8[];
void Func_02004288(s32 id, s32 value);
s32 Func_02004224(s32 row, s32 column, s32 direction) {
  s32 value = gData_02005ad4[row * 4 + column];

  if (direction == 2 && value == 1)
    value = 3;
  if (direction == 1 && value == 2)
    value = 3;
  if (direction == 4 && value == 6)
    value = 8;
  if (direction == 6 && value == 4)
    value = 8;
  if (direction == 7 && value == 5)
    value = 9;
  if (direction == 5 && value == 7)
    value = 9;
  return value;
}

void Func_02004270(s32 x, s32 z, s32 kind, s32 state) {
  s32 i;

  {
    s32 column = x;

    /* FAKEMATCH(barrier): an empty asm on column that names the cell table, so
     * the table is loaded before x is added; without it, 9 of 10 instructions
     * differ in tla-en. */
    asm("" : "+l"(column) : "l"(gSceneState.cells));
    i = z * 11 + column;
  }
  gSceneState.cells[i].kind = kind;
  gSceneState.cells[i].state = state;
}

// Shows or hides the statues: gives each occupied cell's actor, numbered from
// id, its world position and draws its tile, or clears the actor and restores
// the cell's kind tile.
void Func_02004288(s32 id, s32 show) {
  struct SceneState *state = &gSceneState;
  s32 row;
  s32 column;

  state->statuesReady = show;
  state->firstId = id;
  row = 0;
nextRow:
  column = 0;
nextColumn: {
  struct PuzzleCell *cell = &state->cells[row * 11 + column];

  if (cell->occupied != 0) {
    struct Actor *object = Actor_Get(id);
    s32 mapX;
    s32 mapZ;
    s32 kind;

    if (show != 0) {
      object->x = ((state->originX + column) << 20) + 0x80000;
      object->z = ((state->originZ + row) << 20) + 0x80000;
      {
        s32 x = state->originX;
        s32 z = state->originZ;

        /* FAKEMATCH(temporary): the origins read into x and z before the column
         * and row are added; as plain arguments, 12 of 119 instructions differ
         * in tla-en. */
        x += column;
        z += row;
        MapGrid_CopyCells(0, 11, 1, 1, x, z);
      }
      cell->occupied = 2;
    } else {
      object->x = 0;
      object->z = 0;
      // A byte store of the cell's last byte: sched2 orders it behind the next
      // load's constant only as a char access.
      ((u8 *)cell)[3] = 1;
      /* FAKEMATCH(temporary): kind and the origins read before the column and row
       * are added; as plain arguments, 37 of 119 instructions differ in tla-en. */
      kind = cell->kind;
      mapX = state->originX;
      mapZ = state->originZ;
      mapX += column;
      mapZ += row;
      MapGrid_CopyCells(1, kind, 1, 1, mapX, mapZ);
    }
    id++;
  }
}
  column++;
  if (column <= 10)
    goto nextColumn;
  row++;
  if (row <= 7)
    goto nextRow;
  state->endId = id;
}
