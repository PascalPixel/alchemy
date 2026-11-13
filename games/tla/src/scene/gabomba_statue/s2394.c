/// @file
/// Scene code of overlay 0x671, in ROM order, from s2394's offset up to the next object the overlay links.

// ---- games/tla/src/scene/gabomba_statue/s2394.c ----

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

extern u32 gData_02005944[];
extern const u8 gData_02005d7c[];
extern u32 gData_02005ccc[];
extern u32 gData_02005d24[];
extern u32 gData_02005d8c[];
void *Unit_Get(s32 owner);
void Func_0200533c(void);
void Func_02005364(void);
void Func_02001b38(void);
void Func_02001b68(void);
/* VIEW(s2394): routine Func_020053bc as void Func_020053bc(const u8 *data); the definition has void Func_020053bc(const u16 *entry). */
extern void Func_020053bc__s2394(const u8 *data) __asm__("Func_020053bc");
void Func_02005824(void);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);
void Func_0200582c(s32 index, s32 a, s32 b);
s32 Func_02003604(void);
s32 Func_020048a4(s32 x, s32 z, s32 value, s32 id);
void Func_02004f40(u32 *x_out, u32 *z_out);
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
  Func_020053bc__s2394(gData_02005d7c);
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
/* VIEW(s2394): routine Func_02004380 as s32 Func_02004380(s32 col, s32 row, s32 turn); the definition has u8 Func_02004380(s32 x, s32 z, s32 direction); tried as the definition's u8 Func_02004380(s32 x, s32 z, s32 direction), which the object already declares, it does not compile in tla-ja (make modern): games/tla/include/field/event_inline.h:170:48: note: expected 's32' {aka 'int'} but argument is of type 'u8 *' {aka 'unsigned char *'}. */
extern s32 Func_02004380__s2394(s32 col, s32 row, s32 turn) __asm__("Func_02004380");

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
  Iwram_ClearWordsEntry(state, sizeof(state->cells));
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
      piece = Func_02004380__s2394(col, row, 0);
      break;
    case 3:
    case 4:
    case 5:
      piece = Func_02004380__s2394(col, row, 1);
      break;
    case 6:
      Func_02004380__s2394(col, row, 0);
      piece = Func_02004380__s2394(col, row, 0);
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
  Iwram_ClearWordsEntry(Data_02006fc8, 32);
  Iwram_ClearWordsEntry(Data_02006fe8, 32);
  Iwram_ClearWordsEntry(Data_02007008, 32);
  Iwram_ClearWordsEntry(Data_02007028, 32);
  Iwram_ClearWordsEntry(Data_02007048, 32);
  Iwram_ClearWordsEntry(gData_02007068, 32);
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
#undef FIELD_AT

// ---- games/tla/src/scene/gabomba_statue/puzzle_turn.c ----



extern struct SceneState gSceneState;
extern const u8 gData_02005ad4[];
extern const u8 gData_02006ea8[];
void Func_02004288(s32 id, s32 value);
void Func_02005624(struct Actor *object, s32 mode);
u8 Func_02004380(s32 x, s32 z, s32 direction) {
  s32 i;

  {
    s32 column = x;

    /* FAKEMATCH(barrier): an empty asm on column that names the cell table, so
     * the table is loaded before x is added; without it, 12 of 13 instructions
     * differ in tla-en. */
    asm("" : "+l"(column) : "l"(gSceneState.cells));
    i = z * 11 + column;
  }
  gSceneState.cells[i].kind =
      gData_02006ea8[gSceneState.cells[i].kind * 2 + direction];
  return gSceneState.cells[i].kind;
}

void Func_020043a4(s32 value, s32 row) {
  struct Actor *object = Actor_Get(15);

  if (row <= 7)
    object->scaleY += -0x1f00;
  if (value == 1) {
    Func_02004288(gSceneState.firstId, 1);
    Func_02005624(object, 6);
    object->scaleY = 0x10000;
  }
}


void Func_020043ec(s32 mode) {
  struct SceneState *state;
  struct Actor *object;
  struct PuzzleCell *cell;
  s32 x, z;
  u32 row, col;
  s32 found;

  if (mode != 1)
    return;
  state = &gSceneState;
  object = Actor_Get(*(s16 *)((u8 *)Ram_HeapSlots->effectWork + 26));
  x = (object->x >> 20) - state->originX;
  z = (object->z >> 20) - state->originZ;
  {
    s32 kind = state->cells[z * 11 + x].kind;

    cell = &state->cells[z * 11 + x];
    cell->occupied = mode;
    MapGrid_CopyCells(1, kind, 1, 1, state->originX + x, state->originZ + z);
  }
  object->x = 0;
  object->z = 0;
  found = 0;
  row = 0;
  for (row = 0; row <= 7; row++) {
    for (col = 0; col <= 10; col++) {
      if (state->cells[row * 11 + col].occupied == 2) {
        found = 1;
        break;
      }
    }
    if (found)
      return;
  }
  state->statuesReady = found;
}

void Func_020044b4(s32 mode) { Actor_SetModeById(gSceneState.firstId, mode); }

void Func_020056b4(void);
void Func_020056bc(void);

void Func_020044d0(s32 value) {
  struct SceneState *state = &gSceneState;
  s32 id;

  for (id = state->firstId; id < state->endId; id++) {
    struct Actor *object = Actor_Get(id);

    Func_02004380((object->x >> 20) - state->originX,
                  (object->z >> 20) - state->originZ, value);
  }
}

/// Redraws side A once; with the statues ready, moves their offset down by
/// 0x333 a frame for twenty frames, then clears it.
void Func_02004530(void) {
  struct SceneState *state = &gSceneState;
  s32 i;

  if (state->redrawA == 0) {
    MapGrid_CopyCellsLayered(1, 13, 1, 1, state->originX + 3,
                             state->originZ - 2);
    if (state->statuesReady != 0) {
      Event_PlaySound(248);
      Func_020056b4();
      FieldEvent_ResolvePendingActions(0);
      Func_020044b4(3);
      i = 19;
      do {
        s32 *offset = &state->statueOffset;

        *offset -= 0x333;
        System_Wait(1);
        i--;
      } while (i >= 0);
      Func_020044d0(0);
      Func_020056bc();
      state->statueOffset = 0;
    }
    state->redrawA = 1;
  }
}

void Func_020045d8(void) {
  if (gSceneState.redrawA != 0) {
    MapGrid_CopyCellsLayered(1, 12, 1, 1, gSceneState.originX + 3,
                             gSceneState.originZ - 2);
    gSceneState.redrawA = 0;
  }
}

/// Side B's twin of Func_02004530: the offset moves up instead.
void Func_02004620(void) {
  struct SceneState *state = &gSceneState;
  s32 i;

  if (state->redrawB == 0) {
    MapGrid_CopyCellsLayered(0, 13, 1, 1, state->originX + 7,
                             state->originZ - 2);
    if (state->statuesReady != 0) {
      Event_PlaySound(248);
      Func_020056b4();
      FieldEvent_ResolvePendingActions(0);
      Func_020044b4(2);
      i = 19;
      do {
        s32 *offset = &state->statueOffset;

        *offset += 0x333;
        System_Wait(1);
        i--;
      } while (i >= 0);
      Func_020044d0(1);
      Func_020056bc();
      state->statueOffset = 0;
    }
    state->redrawB = 1;
  }
}

void Func_020046c8(void) {
  if (gSceneState.redrawB != 0) {
    MapGrid_CopyCellsLayered(0, 12, 1, 1, gSceneState.originX + 7,
                             gSceneState.originZ - 2);
    gSceneState.redrawB = 0;
  }
}

#include "affine.h"
#include "graphics/vram_tab.h"

void Func_020055ec(struct Sprite *oam, s32 depth);

void Func_02004710(void) {
  s32 affine = 0;
  s32 *scroll = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0xe4);
  struct SceneState *state = &gSceneState;
  struct Sprite *oam = state->oam;
  s32 scrollX = scroll[0] & 0xffff0000;
  s32 scrollZ = scroll[1] & 0xffff0000;
  u16 tile = gResourceTableEntries[state->tileEntry].offset >> 5;
  struct AffineTransform effect;
  s32 id;

  if (state->statueOffset != 0) {
    effect.angle = state->statueOffset;
    effect.scaleX = 0x100;
    effect.scaleY = 0x100;
    affine = AffineMatrix_BuildForEffect(&effect);
  }
  for (id = state->firstId; id < state->endId; id++) {
    struct Actor *piece = Actor_Get(id);
    s32 px = piece->x;
    if (px != 0) {
      s32 x = px - scrollX;
      s32 y = piece->y + 0x100000;
      s32 pz = piece->z;
      s32 z = pz - scrollZ;
      s32 sy = z - y;
      s32 depth = ((y + z) >> 16) + 58;
      s32 cellX = (px >> 20) - state->originX;
      s32 cellZ = (pz >> 20) - state->originZ;
      s32 kind = state->cells[cellZ * 11 + cellX].kind;
      u32 *p;
      x = ((x >> 16) - 8) & 0x1ff;
      sy = ((sy >> 16) - 8) & 255;
      p = (u32 *)oam;
      *p++ = 0;
      *p++ = sy | (x << 16) | 0x40002000;
      *p = (tile + (kind << 3)) | 0x800;
      if (state->statueOffset != 0) {
        oam->mode = 1;
        oam->affine = affine;
      }
      Func_020055ec(oam, depth);
      oam++;
    }
  }
}

#include "task.h"

void Func_020038ac(void);
void Func_02003974(void);
s32 Func_02003a64(s32 value);

s32 Func_020048a4(s32 x, s32 z, s32 value, s32 id) {
  struct SceneState *state = &gSceneState;
  s32 count;

  if (Event_GetFlag(0x109)) {
    Func_020038ac();
    Func_02003974();
    count = state->count;
    Task_AddFrame(Func_02004710, TASK_PASS_VBLANK);
  } else {
    Iwram_ClearWordsEntry(state, sizeof(struct SceneState));
    state->originX = x;
    state->originZ = z;
    Func_020038ac();
    count = Func_02003a64(value);
    Func_02004288(id, 0);
    Task_AddFrame(Func_02004710, TASK_PASS_VBLANK);
  }
  Actor_Get(id)->unk5c[0] = 2;
  return count;
}

#include "call.h"

extern const u8 gData_02006f38[], gData_02006ec0[];
s32 Math_Cosine(s32 angle);
void Func_02005634(struct Actor *object, const u8 *script);

void Func_02004938(struct Actor *ball, s32 step) {
  s16 *colp = (s16 *)((u8 *)ball + 100);
  s32 col = *colp;
  s16 *rowp = (s16 *)((u8 *)ball + 102);
  s32 row = *rowp;
  u8 *headingp = (u8 *)ball + 98;
  s32 heading = *headingp;
  s32 angle = heading << 14;
  s32 cell = row * 11 + col;
  struct SceneState *board = &gSceneState;
  s32 piece = board->cells[cell].kind;
  s32 goal = board->cells[cell].occupied;
  s32 key = (heading << 8) + piece;
  s32 d = step - 8;
  s32 arc = step << 10;
  s32 s = Value2((s32 (*)())Iwram_MulQ16, 0x80000, Math_Sine(arc));
  s32 c = Value2((s32 (*)())Iwram_MulQ16, 0x80000, Math_Cosine(arc));
  s32 z;
  s32 x;

  x = ((board->originX + col) << 20) + 0x80000;
  z = ((board->originZ + row) << 20) + 0x80000;
  if (goal == 2) {
    heading = row = col = 0;
    if (((u8 *)ball)[99] == 1) {
      Sound_Play(204);
      Func_02005634(ball, gData_02006f38);
      ((u8 *)ball)[99] = goal;
    }
    goto done;
  }
  switch (key) {
  case 514:
  case 515:
    col--;
    x -= d << 16;
    break;
  case 2:
  case 3:
    col++;
    x += d << 16;
    break;
  case 257:
  case 259:
    row++;
    z += d << 16;
    break;
  case 769:
  case 771:
    row--;
    z -= d << 16;
    break;
  case 516:
  case 520:
    row++;
    heading = 1;
    x = x - s + 0x80000;
    z = z - c + 0x80000;
    angle = 0x8000 - arc;
    break;
  case 773:
  case 777:
    col--;
    heading = 2;
    x = x + c - 0x80000;
    z = z - s + 0x80000;
    angle = 0xc000 - arc;
    break;
  case 6:
  case 8:
    row--;
    heading = 3;
    x = x + s - 0x80000;
    z = z + c - 0x80000;
    angle = -arc;
    break;
  case 263:
  case 265:
    col++;
    heading = 0;
    x = x - c + 0x80000;
    z = z + s - 0x80000;
    angle = 0x4000 - arc;
    break;
  case 519:
  case 521:
    row--;
    heading = 3;
    x = x - s + 0x80000;
    z = z + c - 0x80000;
    angle = arc + 0x8000;
    break;
  case 262:
  case 264:
    col--;
    heading = 2;
    x = x + c - 0x80000;
    z = z + s - 0x80000;
    angle = arc + 0x4000;
    break;
  case 5:
  case 9:
    row++;
    heading = 1;
    x = x + s - 0x80000;
    z = z - c + 0x80000;
    angle = arc;
    break;
  case 772:
  case 776:
    col++;
    heading = 0;
    x = x - c + 0x80000;
    z = z - s + 0x80000;
    angle = arc + 0xc000;
    break;
  case 11:
    heading = row = col = 0;
    break;
  case 267:
    heading = row = col = 0;
    break;
  case 523:
    heading = row = col = 0;
    break;
  case 10:
  case 266:
  case 522:
  case 778:
    if (((u8 *)ball)[99] == 1) {
      ball->x = x;
      ball->y = 0;
      ball->z = z;
      ball->facing = angle;
      if (x >> 20 == 24 && z >> 20 == 14) {
        ((u8 *)ball)[99] = 7;
      } else {
        Sound_Play(204);
        Func_02005634(ball, gData_02006f38);
        ((u8 *)ball)[99] = 2;
      }
    }
    break;
  case 779:
    heading = row = col = 0;
    break;
  default:
    heading = row = col = 0;
    if (((u8 *)ball)[99] == 1) {
      Sound_Play(132);
      Func_02005634(ball, gData_02006ec0);
      ((u8 *)ball)[99] = 2;
    }
    break;
  }
done:
  if (step == 15) {
    *colp = col;
    *rowp = row;
    *headingp = heading;
  }
  if (((u8 *)ball)[99] == 1) {
    ball->y = 0;
    ball->x = x;
    ball->z = z;
    ball->facing = angle;
    ball->unk4c++;
  }
}

// ---- games/tla/src/scene/gabomba_statue/s4d48.c ----
// Scene code of overlay 0x671, in ROM order, from s4d48's offset up to the next object the overlay links.


struct Unk02004d48 {
  u8 unk00[8];
  s32 x;
  u8 unk0c[76 - 12];
  u32 unk4c;
  u8 unk50[98 - 80];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u16 unk66;
};

void Func_02005654();
void Func_02005624(struct Actor *object, s32 mode);
/* VIEW(s4d48): routine Func_02004938 as void Func_02004938(); the parts before it has another declaration; tried as the definition's void Func_02004938(struct Actor *ball, s32 step), which the object already declares, it does not compile in tla-ja: games/tla/src/scene/gabomba_statue/s2394.c:1848: too many arguments to function `Func_02004938'. */
extern void Func_02004938__s4d48() __asm__("Func_02004938");

s32 Func_02004d48(s32 first, s32 check) {
  struct Unk02004d48 *actor;
  s32 second = first + 1;
  s32 frame, tries, id;
  s32 done;
  s32 result;

  actor = ((struct Unk02004d48 *)Actor_Get(first));
  actor->unk64 = 0;
  actor->unk66 = 7;
  actor->unk62 = 2;
  actor->unk63 = 1;
  actor->unk4c = 0;
  /* FAKEMATCH(signature): Func_02005654, Actor_ResetMotion's veneer, called
   * without the actor it reads from r0; as Func_02005654(actor), 73 of 230
   * instructions differ in tla-en. */
  Func_02005654();
  Func_02005624(actor, 2);
  id = second;
  actor = ((struct Unk02004d48 *)Actor_Get(id++));
  actor->unk64 = 2;
  actor->unk66 = 7;
  actor->unk62 = 0;
  actor->unk63 = 1;
  actor->unk4c = 0;
  Func_02005654(actor);
  Func_02005624(actor, 2);
  actor = ((struct Unk02004d48 *)Actor_Get(id++));
  actor->unk64 = 8;
  actor->unk66 = 7;
  actor->unk62 = 2;
  actor->unk63 = 1;
  actor->unk4c = 0;
  Func_02005654(actor);
  Func_02005624(actor, 2);
  actor = ((struct Unk02004d48 *)Actor_Get(id));
  actor->unk64 = 10;
  actor->unk66 = 7;
  actor->unk62 = 0;
  actor->unk63 = 1;
  actor->unk4c = 0;
  Func_02005654(actor);
  Func_02005624(actor, 2);
  for (tries = 0; tries <= 127; tries++) {
    done = 1;
    for (frame = 0; frame <= 15; frame++) {
      for (id = first; id < first + 4; id++) {
        actor = ((struct Unk02004d48 *)Actor_Get(id));
        if (actor->unk63 != 0 && actor->unk63 != 7)
          done = 0;
        Func_02004938__s4d48(actor, frame, id);
      }
      System_Wait(1);
    }
    if (done)
      break;
  }
  System_Wait(20);
  result = 3;
  if (check) {
    u32 a = 0;
    u32 b = 0;
    {
      struct Unk02004d48 *r = ((struct Unk02004d48 *)Actor_Get(first));
      if (r->x != 0)
        a = r->unk4c;
    }
    {
      struct Unk02004d48 *r = ((struct Unk02004d48 *)Actor_Get(second));
      if (r->x != 0) {
        u32 value = r->unk4c;
        if (a == 0 || a > value)
          a = value;
      }
    }
    {
      struct Unk02004d48 *r = ((struct Unk02004d48 *)Actor_Get(first + 2));
      if (r->x != 0)
        b = r->unk4c;
    }
    {
      struct Unk02004d48 *r = ((struct Unk02004d48 *)Actor_Get(first + 3));
      if (r->x != 0) {
        u32 value = r->unk4c;
        if (b == 0 || b > value)
          b = value;
      }
    }
    if (a != 0) {
      result = 1;
      if (b != 0) {
        result = 0;
        if (a != b) {
          if (a < b) {
            Event_SetFlag(0x9eb);
            result = 1;
          } else {
            Event_SetFlag(0x9eb);
            result = 2;
          }
        }
      }
    } else {
      if (b != 0)
        return 2;
      return 3;
    }
  }
  return result;
}




void Func_02004f40(u32 *x_out, u32 *z_out) {
  u32 z;
  u32 x;

  for (z = 0; z <= 7; z++) {
    for (x = 0; x <= 10; x++) {
      if (gSceneState.cells[z * 11 + x].kind == 10 && !(x == 5 && z == 3) &&
          !(z == 7 && (x == 1 || x == 9))) {
        *x_out = x;
        *z_out = z;
        return;
      }
    }
  }
}



struct ClockWork {
  u8 *tiles;  ///< the digit sheet
  u8 *digits; ///< the five digit pictures, as loaded
  u8 old[8];
  u8 now[8];
  u8 digit[8];
  struct Sprite oam[8];
  s32 tileEntry;
  s32 timer;
  u8 ringing;
  u8 mode;
};

extern u8 gData_020023c4[];
s32 Math_Cosine(s32 angle);

/// Converts the remaining frames into minutes, seconds and tenths
/// (`MM:SS:T`).
void Func_02004f84(s32 time, u8 *text) {
  if (time < 0)
    time = 0;
  *text = time / 36000 + '0';
  time %= 36000;
  *++text = time / 3600 + '0';
  time %= 3600;
  *++text = ':';
  *++text = time / 600 + '0';
  time %= 600;
  *++text = time / 60 + '0';
  time %= 60;
  *++text = ':';
  *++text = time / 6 + '0';
  text[1] = 0;
}

s32 Func_02005020(s32 time) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);
  s32 second = time % 60;
  s32 volume = 512;
  u8 *ringing = &clock->ringing;

  if (second != 0) {
    *ringing = 1;
  } else if (*ringing) {
    if (clock->timer != 0)
      Event_PlaySound(236);
    *ringing = second;
  }
  if (second <= 19) {
    s32 cosine = Math_Cosine((second << 15) / 10);
    s32 (*mul)(s32 a, s32 b);
    s32 scale = 128;

    mul = Iwram_MulQ16;
    volume = mul(scale << 2, cosine);
  }
  if (volume <= 16)
    volume = 16;
  return volume;
}


void Func_020055ec(struct Sprite *oam, s32 depth);

/// Draws the clock's five-digit count: copies the pictures of the digits of
/// timer + 50 out of the digit sheet and loads them, builds a plain and a
/// scaled affine matrix, and fills five OAM entries, giving the scaled matrix
/// to the middle digit and to each digit where timer + 60 and timer differ.
void Func_02005090(void) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);
  u8 *digit = clock->digit;
  u8 *old = clock->tiles;
  u8 *now = clock->digits;
  u32 tile = gResourceTableEntries[clock->tileEntry].offset >> 5;
  struct Sprite *oam = clock->oam;
  s32 *timer = &clock->timer;
  struct AffineTransform effect;
  s32 plain, scaled, i;
  s32 scale;
  u32 *p;

  Func_02004f84(*timer + 50, digit);
  Func_02004f84(*timer + 60, clock->old);
  Func_02004f84(*timer, clock->now);
  // old and now walk the sheet and the pictures here, the digit lists below.
  for (i = 0; i < 5; i++) {
    Iwram_CopyWordsEntry(now, old + (*digit << 6) - 0xc00, 64);
    now += 64;
    digit++;
  }
  Vram_Load(clock->tileEntry, 320, clock->digits);
  effect.angle = 0;
  effect.scaleX = 0x200;
  effect.scaleY = 0x200;
  plain = AffineMatrix_BuildForEffect(&effect);
  scale = Func_02005020(*timer);
  effect.angle = 0;
  effect.scaleX = scale;
  effect.scaleY = scale;
  scaled = AffineMatrix_BuildForEffect(&effect);
  old = clock->old;
  now = clock->now;
  for (i = 0; i <= 4; i++, tile += 2, oam++) {
    p = (u32 *)oam;
    *p++ = 0;
    *p++ = (i << 20) | 0x8000;
    *p = tile | 0x400;
    oam->mode = 3;
    if (*old++ != *now++ || i == 2)
      oam->affine = scaled;
    else
      oam->affine = plain;
    Func_020055ec(oam, 236);
  }
}


void Func_02005210(void);
void Func_0200525c(void);
s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

void Func_02005210(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);

  if (*(s16 *)(event + 358) == 0 && *(s16 *)(event + 346) == 0 &&
      *(s16 *)(event + 350) == 0 && clock->timer > 0)
    clock->timer--;
  Func_02005090();
}

void Func_0200525c(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);

  if (*(s16 *)(event + 358) == 0 && *(s16 *)(event + 346) == 0 &&
      *(s16 *)(event + 350) == 0 && clock->timer > 0)
    clock->timer--;
}

#include "map.h"

void *Heap_AllocEwram(s32 size);
void *Resource_GetTableEntry(s32 index);
void Func_020055cc(const u8 *data, void *buffer);
s32 Resource_FindFreeEntry(void);
extern char ResourceId_ClockDigits;

void Func_020052a4(s32 mode) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);
  u8 *tiles;

  if (!Event_GetFlag(0x109)) {
    Iwram_ClearWordsEntry(clock, sizeof(struct ClockWork));
    clock->timer = 3600;
    clock->ringing = 1;
  }
  tiles = Heap_AllocEwram(0x400);
  Func_020055cc(Resource_GetTableEntry((s32)&ResourceId_ClockDigits), tiles);
  clock->tiles = tiles;
  clock->digits = tiles + 0x2c0;
  clock->tileEntry = Resource_FindFreeEntry();
  Vram_Load(clock->tileEntry, 320, NULL);
  clock->mode = mode;
  if (mode)
    Task_AddFrame(Func_02005210, TASK_PASS_VBLANK);
  else
    Task_AddFrame(Func_0200525c, TASK_PASS_VBLANK);
}

void Func_0200533c(void) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);

  if (clock->mode)
    FieldTask_Remove(Func_02005210);
  else
    FieldTask_Remove(Func_0200525c);
}

void Func_02005364(void) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);

  if (clock->mode)
    Task_AddFrame(Func_02005210, TASK_PASS_VBLANK);
  else
    Task_AddFrame(Func_0200525c, TASK_PASS_VBLANK);
  clock->timer = 3600;
}

s32 Func_020053a0(void) {
  return ((struct ClockWork *)(gData_020023c4 + 0x288))->timer;
}

void Func_020053ac(void) {
  ((struct ClockWork *)(gData_020023c4 + 0x288))->timer = 3600;
}


void Func_020054ac(s32 id, u32 mode, s32 flag, s32 layer);

struct EffectWork {
  u8 unk00[26];
  s16 target;
};

// Layer switch table: three words (id, mode, flag), terminated by 0xffff.
void Func_020053bc(const u16 *entry) {
  s32 value;

  while ((value = *entry) != 0xffff) {
    u16 id = *entry++;
    struct Actor *object = Actor_Get(id);
    s16 mode = entry[0];
    s16 flag = entry[1];

    object->collisionFlags &= 0xfe;
    Actor_SetSingleChildFlags(object, 0);
    Func_020054ac(id, (u16)mode, (u16)flag, object->terrainId);
    entry += 2;
  }
}

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

// Finds the current target's table row and shrinks it.
void Func_02005428(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  while (*list != 0xffff) {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_Get(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        Event_SetFlag(flag);
        Func_020054ac(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  }
}

s32 Func_02005844(struct Actor *object);

void Func_020054ac(s32 id, u32 mode, s32 flag, s32 layer) {
  struct Actor *object = Actor_Get(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_02005844(object);
  if (Event_GetFlag(flag)) {
    /* FAKEMATCH(temporary): the flags read into a local before the cell store
     * and a local holding 2, so the flags are ORed into the register that holds
     * 2; as one priorityFlags |= 2, 8 of 75 instructions differ in tla-en. */
    u8 flags = object->priorityFlags;
    s32 bits;

    cell->collisionCode = 0;
    bits = 2;
    bits |= flags;
    object->priorityFlags = bits;
    switch (mode) {
    case 0:
      Func_02005624(object, 2);
      break;
    case 1:
      Func_02005624(object, 4);
      break;
    case 2:
      Func_02005624(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (mode) {
    case 0:
      Func_02005624(object, 1);
      break;
    case 1:
      Func_02005624(object, 3);
      break;
    case 2:
      Func_02005624(object, 5);
      break;
    }
  }
}

#include "objcmd.h"

// clang-format off
u32 gData_0200585c[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02005898[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_020058d4[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02005910[] = {
    0x20007, 512, 0x20003, 0x20004, 516, 515,
    0x20101, 0x10201, 0x20102, 0x10202, 0xf800, 0,
};
// clang-format on

u16 gData_02005940[] = {
    259,
    2,
};

u32 gData_02005944[] = {
    38,
};
