// Draft, differ 2 of 1176, skeleton gap 1 (tla-en, 2026-11-01)
// Left: one instruction order, in the actor 23 setup: the native issues
// "strb r2, [r3]" (motionFlags) before "adds r1, r7, #0" (the address of
// priorityFlags); the draft swaps those two. sched2 ranks the copy 40 and the
// store 39, and only a load-after-store cost of 2 would tie them, which the
// slow-loads read-modify-write exception (thumb_load_stored_back_p) refuses
// because the byte 35 store writes back to the address it reads. So no C shape
// tried moves it: named fields, raw offsets, pointer locals, a u8/u16/s32 zero
// local, s8 access, statement swap, direct calls, void prototypes. Whether the
// exception is wrong here is a compiler question (K3), for Pascal.
// Devices tried and taken out, none reaches 0: volatile on either byte, an
// inline wrapper on either (no change); a one-pass block or empty volatile asm
// between them (22 to 38: a barrier gives every earlier read of r5 one more
// forward dependence, so sites with movs r0 before adds r1, r5 swap); a
// non-volatile asm with memory clobber redefining the actor fixes the order
// but counts 4 bytes for literal pools, moving the third pool one instruction
// and leaving differ 16.
// What closed the rest (163 to 2): the owner slot is read through a base
// pointer so its offset splits; each read run has its own local (the if-block
// and the post-loop run are single-block locals that take r5 from local-alloc,
// which pushes the counter to r6 and the final zero to r6); quarterTurn is a
// single-block constant in r5, and owner, in r6, stays single-block too, so
// the two high constants land r8 and r10. Func_0200578c, Func_020056f4,
// Func_02005794 and Func_02005734 return void (with s32 the argument loads
// around them swap). The pooled zero in r10 is the HImode mask of the first
// angle store, so byte 85's stores are plain 0.
// Func_02005714's later calls go through an inline that calls it directly, as
// Func_020058fc's do in overlay 6a5's scenes.
#include "types.h"
#include "actor.h"
#include "call.h"
#include "system/main_routine_work.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_MoveWithinMapBounds();
s32 Event_AskYesNoWithActorAnimation();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_Wait();
s32 Func_02000610();
s32 Func_02000640();
s32 Func_0200558c();
s32 Func_020055c4();
s32 Func_0200560c();
s32 Func_0200567c();
s32 Func_020056dc();
void Func_020056f4();
s32 Func_02005704();
s32 Func_02005714();
s32 Func_0200571c();
s32 Func_02005724();
void Func_02005734();
s32 Func_0200573c();
s32 Func_02005744();
s32 Func_0200575c();
s32 Func_02005764();
s32 Func_0200576c();
s32 Func_02005784();
void Func_0200578c();
void Func_02005794();
s32 Func_020057dc();
s32 Motion_SetModeAndWaitAnimation();
s32 Actor_ArmCallback();
s32 Actor_SetMovementTypeById();
s32 Actor_Jump();
s32 Actor_SetPositionAndReset();
s32 Actor_SetSpeedParameters();
s32 Actor_StartRepeatedMotion();
s32 Actor_GetById();
s32 Actor_SetPartAttribute();
s32 Task_RemoveFar();
s32 FieldMessage_ShowAtActor();
s32 wait();
extern u8 Func_020004a4[];
extern u8 MsgWhatCouldHe[];
extern u8 gData_020061d4[];

static __inline__ void W_Func_02005714(s32 a0, s32 a1, s32 a2) {
  Func_02005714(a0, a1, a2);
}

void Func_02003ff4(void) {
  s32 owner, i, ownerInLoop, ownerAfterLoop, quarterTurn;
  s32 threeSixteenthsTurn, actor, t2, t3;
  s32 t4, t5, t6, t7, t8, t9, t10, t17, t21, t25;
  s32 t31, t49, t62, t76, t87, t101, t104, t107, t156;
  s32 t158, t160, t168, t184, t186, t189, t195, t205;
  s32 t215, t228, t238, t250, t263, t271, t274, t284, t292;
  s32 t296;

  Func_0200560c();
  Call1(Func_02005784, 0);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    owner = (s32)&party->currentOwner;
  }
  quarterTurn = 0x4000;
  t2 = *(s32 *)owner;
  t3 = Value1(Actor_GetById, t2);
  ((struct Actor *)t3)->angle = quarterTurn;
  t4 = Value1(Actor_GetById, 7);
  ((struct Actor *)t4)->angle = quarterTurn;
  t5 = Value1(Actor_GetById, 5);
  ((struct Actor *)t5)->angle = quarterTurn;
  t6 = Value1(Actor_GetById, 6);
  ((struct Actor *)t6)->angle = quarterTurn;
  t7 = Value1(Actor_GetById, 19);
  threeSixteenthsTurn = 0x3000;
  ((struct Actor *)t7)->angle = threeSixteenthsTurn;
  t8 = Value1(Actor_GetById, 0);
  ((struct Actor *)t8)->angle = quarterTurn;
  t9 = Value1(Actor_GetById, 1);
  ((struct Actor *)t9)->angle = quarterTurn;
  t10 = Value1(Actor_GetById, 2);
  ((struct Actor *)t10)->angle = quarterTurn;
  Call1(wait, 1);
  Call2(Actor_SetModeById, 20, 8);
  Call2(Actor_SetModeById, 21, 8);
  Call2(Actor_SetModeById, 22, 7);
  Call3(Func_0200567c, 19, 0x1f20000, 0x1400000);
  Call3(Func_0200567c, 1, 0x21a0000, 0x1400000);
  t17 = *(s32 *)owner;
  Call3(Func_0200567c, t17, 0x1fa0000, 0x1180000);
  Call1(wait, 1);
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  t21 = Value0(Func_0200573c);
  ((struct Actor *)t21)->motionFlags = 0;
  Call4(Camera_MoveWithinMapBounds, 0x2000000, 0x600000, 0x1200000, 0);
  Func_0200558c();
  Call1(wait, 1);
  t25 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t25, s32, 428) = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1(FieldEvent_Wait, 20);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  t31 = *(s32 *)owner;
  Call3(Actor_ArmCallback, t31, 0, 0);
  Call3(Actor_ArmCallback, 5, 0, 0);
  Call3(Actor_ArmCallback, 19, 0, 0);
  Call3(Actor_ArmCallback, 0, 0x8000, 0);
  Call3(Actor_ArmCallback, 3, 0x8000, 0);
  Call3(Actor_ArmCallback, 1, 0x8000, 0);
  Call3(Actor_ArmCallback, 2, 0x8000, 40);
  Call2(Func_02005704, 6, 0xe000);
  Call1(Func_020056dc, (s32)MsgWhatCouldHe);
  Call2(Func_020056f4, 6, 0);
  Call3(Actor_ArmCallback, 3, 0x6000, 20);
  Call2(Motion_SetModeAndWaitAnimation, 3, 4);
  Call3(Actor_Jump, 19, 4, 10);
  Call2(Func_020056f4, 19, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call3(Actor_ArmCallback, 5, 0x2000, 0);
  Call3(Actor_ArmCallback, 7, quarterTurn, 0);
  t49 = *(s32 *)owner;
  Call3(Actor_ArmCallback, t49, quarterTurn, 0);
  Call3(Actor_ArmCallback, 0, 0x6000, 0);
  Call3(Actor_ArmCallback, 2, 0x6000, 0);
  Call2(Func_02005724, 0xcccc, 0x1999);
  Call4(Camera_MoveWithinMapBounds, 0x2000000, -1, 0x1400000, 1);
  Func_02005734();
  Call2(Func_02005704, 19, 0xd000);
  Call2(Actor_SetModeById, 19, 3);
  Call2(Func_020056f4, 19, 0);
  Call2(Actor_SetModeById, 7, 3);
  Call2(Actor_SetModeById, 5, 3);
  Call2(Actor_SetModeById, 6, 3);
  t62 = *(s32 *)owner;
  Call2(Actor_SetModeById, t62, 3);
  Call2(Actor_SetModeById, 0, 3);
  Call2(Actor_SetModeById, 3, 3);
  Call2(Actor_SetModeById, 2, 3);
  Call2(Motion_SetModeAndWaitAnimation, 1, 3);
  Call2(Func_0200571c, 19, 0x102);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_020056f4, 19, 0);
  Call2(Func_020056f4, 7, 0);
  Call2(Func_02005704, 19, 0xb000);
  Call2(Motion_SetModeAndWaitAnimation, 19, 3);
  Func_02005714(1, 0x103, 20);
  Call2(Func_020056f4, 1, 0);
  t76 = Value1(Actor_GetById, 28);
  Call2(Actor_SetPartAttribute, t76, 15);
  Call3(Func_0200567c, 28, 0x2000000, 0x1680000);
  Call1(wait, 1);
  Call2(Func_02005764, 0x10000, 0);
  Call2(Func_0200578c, 23, 24);
  Func_020056f4(28, 0);
  Func_02005794();
  Call3(Actor_ArmCallback, 2, quarterTurn, 0);
  Call3(Actor_Jump, 2, 4, 10);
  Call2(Func_020056f4, 2, 0);
  t87 = *(s32 *)owner;
  Call3(Actor_ArmCallback, t87, quarterTurn, 0);
  Call3(Actor_ArmCallback, 7, quarterTurn, 0);
  Call3(Actor_ArmCallback, 5, quarterTurn, 0);
  Call3(Actor_ArmCallback, 6, quarterTurn, 0);
  Call3(Actor_ArmCallback, 19, threeSixteenthsTurn, 0);
  Call3(Actor_ArmCallback, 1, quarterTurn, 0);
  Call3(Actor_ArmCallback, 3, quarterTurn, 0);
  Call2(Func_02005704, 0, quarterTurn);
  W_Func_02005714(0, 0x109, 20);
  Call2(Func_020056f4, 0, 0);
  Call2(Func_0200578c, 23, 24);
  Func_020056f4(28, 0);
  Func_02005794();
  t101 = *(s32 *)owner;
  Call3(Actor_ArmCallback, t101, 0, 0);
  Call3(Actor_ArmCallback, 0, 0x8000, 20);
  t104 = *(s32 *)owner;
  Call2(Actor_SetModeById, t104, 3);
  Call2(Motion_SetModeAndWaitAnimation, 0, 3);
  t107 = *(s32 *)owner;
  Call3(Actor_ArmCallback, t107, quarterTurn, 0);
  Call2(Func_02005704, 0, quarterTurn);
  Call2(Func_0200578c, 23, 24);
  Func_020056f4(28, 0);
  Func_02005794();
  actor = Value1(Actor_GetById, 5);
  ((struct Actor *)actor)->angle = 0x6000;
  Call2(Actor_SetModeById, 5, 22);
  Call1(FieldEvent_Wait, 10);
  Call2(Actor_StartRepeatedMotion, 5, 2);
  Call2(Func_020056f4, 5, 0);
  Call2(Func_0200578c, 23, 24);
  Func_020056f4(28, 0);
  Func_02005794();
  ((struct Actor *)actor)->angle = quarterTurn;
  Call2(Actor_SetModeById, 5, 1);
  W_Func_02005714(5, 0x100, 20);
  Call2(Func_020056f4, 5, 0);
  Call2(Func_0200578c, 23, 24);
  Func_020056f4(28, 0);
  Func_02005794();
  Call2(Func_020056f4, 1, 0);
  Call2(Func_0200578c, 23, 24);
  Func_020056f4(28, 0);
  Func_02005794();
  W_Func_02005714(1, 0x100, 20);
  Call2(Func_020056f4, 1, 0);
  Call2(Func_0200578c, 23, 24);
  Func_020056f4(28, 0);
  Func_02005794();
  Func_02005714(19, 0x100, 20);
  Call2(Func_020056f4, 19, 0);
  Call2(Func_0200578c, 23, 24);
  Func_020056f4(28, 0);
  Func_02005794();
  Call1(FieldEvent_Wait, 40);
  Call4(Camera_MoveWithinMapBounds, 0x2000000, -1, 0x1280000, 1);
  Call3(Actor_ArmCallback, 19, 0x5000, 20);
  Call3(Actor_ArmCallback, 19, threeSixteenthsTurn, 40);
  Call2(Func_02005704, 19, 0xd000);
  Call2(Func_020056f4, 19, 0);
  Call2(Func_02005704, 5, 0x2000);
  Call2(Func_020056f4, 0x4005, 0);
  Call2(Func_02005704, 0, 0x8000);
  Call2(Func_020056f4, 0x8000, 0);
  Call2(Func_02005704, 7, 0);
  Call2(Func_020056f4, 7, 0);
  Call2(Func_02005704, 2, 0x8000);
  Func_02005714(2, 0x109, 40);
  FieldMessage_ShowAtActor(2, 0);
  t156 = *(s32 *)owner;
  Call2(Func_02005704, t156, 0);
  t158 = *(s32 *)owner;
  Event_AskYesNoWithActorAnimation(t158, 0);
  t160 = *(s32 *)owner;
  Call1(Actor_WaitForAnimationChange, t160);
  Call2(Func_0200575c, 0x10004, 1);
  Call1(Func_0200576c, 40);
  Call1(Func_020057dc, 78);
  Call1(Func_020057dc, 141);
  Call3(Func_020055c4, 0x10000, 0x10000, 0x10000);
  Call1(FieldEvent_Wait, 20);
  t168 = *(s32 *)owner;
  Call1(Func_02000610, t168);
  Call1(Func_02000610, 5);
  Call1(Func_02000610, 6);
  Call1(Func_02000610, 7);
  Call1(Func_02000610, 19);
  Call1(Func_02000610, 0);
  Call1(Func_02000610, 1);
  Call1(Func_02000610, 2);
  Call1(Func_02000610, 3);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_02005724, 0x19999, 0x3333);
  Call4(Camera_MoveWithinMapBounds, 0x2000000, -1, 0x1200000, 1);
  Func_02005734();
  Call1(Task_RemoveFar, (s32)Func_020004a4);
  Call1(Func_020057dc, 140);
  t184 = Value1(Actor_GetById, 8);
  Call2(Actor_SetPartAttribute, t184, 12);
  t186 = Value1(Actor_GetById, 9);
  Call2(Actor_SetPartAttribute, t186, 12);
  Call2(Actor_SetSpritePriority, 23, 2);
  t189 = Value1(Actor_GetById, 23);
  Call2(Actor_SetPartAttribute, t189, 7);
  actor = Value1(Actor_GetById, 23);
  ((struct Actor *)actor)->scaleY = 0xffff0000;
  ((struct Actor *)actor)->scaleX = 0x1999;
  ((struct Actor *)actor)->motionFlags = 0;
  ((struct Actor *)actor)->priorityFlags |= 2;
  Call4(Actor_SetPositionAndResetMotion, actor, 0x2000000, 0x400000, 0xcc0000);
  i = 0;
L_0200c66c:
  if (i != 20)
    goto L_0200c72a;
  Call3(Func_020055c4, 0x20000, 0x20000, 0x10000);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    ownerInLoop = (s32)&party->currentOwner;
  }
  t195 = *(s32 *)ownerInLoop;
  W_Func_02005714(t195, 0x100, 0);
  W_Func_02005714(5, 0x100, 0);
  W_Func_02005714(6, 0x100, 0);
  W_Func_02005714(7, 0x100, 0);
  W_Func_02005714(19, 0x100, 0);
  W_Func_02005714(0, 0x100, 0);
  W_Func_02005714(1, 0x100, 0);
  W_Func_02005714(2, 0x100, 0);
  W_Func_02005714(3, 0x100, 0);
  t205 = *(s32 *)ownerInLoop;
  Call1(Func_02000640, t205);
  Call1(Func_02000640, 5);
  Call1(Func_02000640, 6);
  Call1(Func_02000640, 7);
  Call1(Func_02000640, 19);
  Call1(Func_02000640, 0);
  Call1(Func_02000640, 1);
  Call1(Func_02000640, 2);
  Call1(Func_02000640, 3);
L_0200c72a:
  t215 = ((struct Actor *)actor)->scaleX;
  ((struct Actor *)actor)->scaleX = (t215 + 0x400);
  Call1(wait, 1);
  i = (i + 1);
  if ((u32)i <= (u32)39)
    goto L_0200c66c;
  Call1(Func_020057dc, 63);
  Call2(Actor_SetMovementTypeById, 23, (s32)gData_020061d4);
  Call1(FieldEvent_Wait, 80);
  Call3(Actor_Jump, 1, 4, 10);
  Call3(Actor_Jump, 1, 6, 10);
  Call2(Func_02005704, 1, 0x8000);
  Call2(Func_020056f4, 1, 0);
  Call2(Func_020056f4, 7, 0);
  Call2(Func_02005704, 6, 0xa000);
  Call2(Func_020056f4, 6, 0);
  Call2(Func_020056f4, 19, 0);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    ownerAfterLoop = (s32)&party->currentOwner;
  }
  t228 = *(s32 *)ownerAfterLoop;
  Call3(Actor_ArmCallback, t228, 0, 0);
  Call3(Actor_ArmCallback, 5, 0, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call3(Actor_ArmCallback, 19, 0, 0);
  Call3(Actor_ArmCallback, 0, 0x8000, 0);
  Call3(Actor_ArmCallback, 1, 0x8000, 0);
  Call3(Actor_ArmCallback, 2, 0x8000, 0);
  Call3(Actor_ArmCallback, 3, 0x8000, 40);
  t238 = *(s32 *)ownerAfterLoop;
  Call3(Actor_ArmCallback, t238, 0xc000, 0);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call3(Actor_ArmCallback, 19, 0xc000, 0);
  Call3(Actor_ArmCallback, 0, 0xc000, 0);
  Call3(Actor_ArmCallback, 1, 0xc000, 0);
  Call3(Actor_ArmCallback, 2, 0xc000, 0);
  Call3(Actor_ArmCallback, 3, 0xc000, 20);
  Call2(Func_02005704, 3, 0x8000);
  Call2(Func_020056f4, 3, 0);
  t250 = *(s32 *)ownerAfterLoop;
  Call3(Actor_ArmCallback, t250, 0, 0);
  Call3(Actor_ArmCallback, 5, 0, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call3(Actor_ArmCallback, 19, 0, 0);
  Call3(Actor_ArmCallback, 0, 0x8000, 0);
  Call3(Actor_ArmCallback, 1, 0x8000, 0);
  Call3(Actor_ArmCallback, 2, 0x8000, 0);
  Call3(Actor_ArmCallback, 3, 0x8000, 20);
  Call2(Actor_SetModeById, 7, 3);
  Call2(Actor_SetModeById, 5, 3);
  Call2(Actor_SetModeById, 6, 3);
  t263 = *(s32 *)ownerAfterLoop;
  Call2(Actor_SetModeById, t263, 3);
  Call2(Actor_SetModeById, 0, 3);
  Call2(Actor_SetModeById, 3, 3);
  Call2(Actor_SetModeById, 2, 3);
  Call2(Motion_SetModeAndWaitAnimation, 1, 3);
  Call4(Camera_MoveWithinMapBounds, 0x2000000, -1, 0x1380000, 1);
  Call3(Func_0200567c, 8, 0, 0);
  t271 = Value1(Actor_GetById, 9);
  Call2(Actor_SetPartAttribute, t271, 7);
  Call2(Actor_SetSpritePriority, 9, 1);
  t274 = Value1(Actor_GetById, 9);
  ((struct Actor *)t274)->priorityFlags &= 253;
  Call1(Func_020057dc, 140);
  Call3(Func_020055c4, 0x30000, 0x30000, 0x10000);
  Call2(Func_0200575c, 0x203210, 0);
  Call1(Func_0200576c, 40);
  Call3(Actor_ArmCallback, 7, 0x4000, 0);
  Call3(Actor_ArmCallback, 5, 0x2000, 0);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Call3(Actor_ArmCallback, 19, 0xb000, 0);
  t284 = *(s32 *)ownerAfterLoop;
  Call3(Actor_ArmCallback, t284, 0x4000, 0);
  Call3(Actor_ArmCallback, 3, 0x4000, 0);
  Call3(Actor_ArmCallback, 6, 0x8000, 0);
  Call3(Actor_ArmCallback, 0, 0, 0);
  Call3(Actor_ArmCallback, 1, 0xc000, 80);
  Call3(Actor_Jump, 1, 4, 0);
  Call3(Actor_SetSpeedParameters, 1, 0x33333, 0x19999);
  t292 = Value1(Actor_GetById, 1);
  FIELD_AT(t292, u8, 90) &= 254;
  Call3(Actor_SetPositionAndReset, 1, 0x21e, 0x148);
  Call1(FieldEvent_Wait, 1);
  t296 = Value1(Actor_GetById, 1);
  FIELD_AT(t296, u8, 90) |= 1;
  Call2(Func_020056f4, 1, 0);
  Call3(Actor_ArmCallback, 0, 0x4000, 0);
  Call3(Actor_ArmCallback, 3, 0x2000, 0);
  Call3(Actor_ArmCallback, 2, 0x4000, 40);
  Call3(Actor_ArmCallback, 3, 0x6000, 0);
  Call3(Actor_ArmCallback, 2, 0xa000, 0);
  Call3(Actor_ArmCallback, 0, 0xe000, 20);
  Func_02005714(0, 0x103, 0);
  Call2(Func_02005704, 0, 0x4000);
  Call3(Actor_Jump, 0, 4, 10);
  Call2(Func_020056f4, 0x1000, 0);
  Call2(Actor_SetModeById, 1, 4);
  Call2(Func_020056f4, 1, 0);
  Call3(Actor_ArmCallback, 3, 0x4000, 0);
  Func_02005714(3, 0x100, 20);
  Call2(Func_020056f4, 3, 0);
  Call2(Func_0200571c, 7, 0x102);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_020056f4, 7, 0);
  Call3(Actor_Jump, 5, 4, 10);
  Call2(Func_020056f4, 5, 0);
  Call2(Func_020056f4, 0x8002, 0);
  Call2(Func_02005704, 19, 0xd000);
  Call3(Actor_Jump, 19, 2, 10);
  Call3(Actor_Jump, 19, 4, 10);
  Call2(Func_020056f4, 19, 0);
  Call1(Func_020057dc, 0x12a);
  Call1(Func_020057dc, 78);
  Call1(wait, 1);
  Call1(Func_020057dc, 140);
  Call2(Func_0200575c, 0x7fff, 0);
  Call1(Func_0200576c, 20);
  Call1(wait, 240);
  *(u16 *)0x4000000 = 0;
  Call2(Func_0200575c, 0, 0);
  Call1(Func_0200576c, 8);
  Call1(wait, 8);
  Call1(Func_02005744, 4);
  return;
}
