// Matches: differ 0 (tla-en, 2026-11-08)
// tla-ja, tla-de, tla-es, tla-fr and tla-it also differ 0 against their own
// overlay objects. 2026-11-08 (rules): the scratch copy of actor and its last
// store measured 0 without them and are gone, as are the cast-of-casts and the
// field read-modify-writes through scratch. Of call.h's 227 forwarding calls,
// 116 match as direct calls and are written so (taken greedily in file order);
// the 120 left, and W_Func_02005714, each carry a wrapper tag with its own
// figure measured against this form. lastArg holds one constant (inline, 4
// differ); owner and owner2 are each built in two steps (one expression 109
// and 112; owner reused for owner2, 138). What closed the rest (2026-11-08,
// third round): the flag clears and sets written as FIELD_AT(..., u8, 90) &=
// 254 and |= 1 (34 to 12); the CSE'd 254 then outranks the store's zero for r8,
// and the last or's result gets no hard register, so reload gives it the slot
// below the spilled actor 32 and deletes its store: that is the native's four
// empty bytes at sp, so the held struct went (12 to 8). Func_02005734 returns
// void (its far call reaches Func_080d4548, void in field/camera/bounds.c),
// which orders both camera calls' shifts as the native does (34 to 30). The
// call through W_Func_02005714 is a wrapper kept from earlier rounds, now
// tagged (direct 103, Call3 2: r0 set before r2). Before adoption: the FIELD_AT
// macro and the t names are the translator's and need real types and names.
#include "call.h"
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Camera_MoveWithinMapBounds();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_Wait();
s32 Func_020053e4();
s32 Func_020054e4();
s32 Func_0200560c();
s32 Func_0200567c();
s32 Func_02005684();
s32 Func_020056dc();
s32 Func_020056f4();
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
s32 Func_020057dc();
s32 Motion_SetModeAndWaitAnimation();
s32 Motion_SetVarCbAndRefresh();
s32 Actor_ArmCallback();
s32 Actor_SetMovementTypeById();
s32 Actor_Jump();
s32 Actor_SetSpritePriority();
s32 Actor_SetPositionAndReset();
s32 Actor_SetSpeedParameters();
s32 Actor_StartRepeatedMotion();
s32 Actor_GetById();
s32 Actor_SetMovementTypeAndWaitById();
s32 Actor_SetModeById();
s32 Actor_SetPartAttribute();
s32 Random16Far();
s32 wait();
extern u8 Func_0200008c[];
extern u8 Func_02001eb0[];
extern u8 Func_02001ee4[];
extern u8 MsgWhoAreThey[];
extern u8 gData_02005f08[];
extern u8 gData_02005f50[];
extern u8 gData_02005f98[];
extern u8 gData_02005fdc[];
extern u8 gData_02006004[];
extern u8 gData_0200602c[];
extern u8 gData_02006070[];
extern u8 gData_02006b64[];
extern u8 gData_02006b6c[];
extern u8 gData_02006b70[];
extern u8 gData_02006c20[];
extern u8 gMainRoutineWork[];

static __inline__ void W_Func_02005714(s32 a0, s32 a1, s32 a2) {
  Func_02005714(a0, a1, a2);
}

void Func_02001f10(void) {
  s32 lastArg, spill;
  s32 owner, actor, owner2, t1;
  s32 t2, t16, t34;
  s32 t85, t86, t87;
  s32 t158, t162;
  s32 t166, t170;
  s32 t244, t248;

  actor = Actor_GetById(17);
  t1 = Actor_GetById(30);
  t2 = Actor_GetById(31);
  spill = (Actor_GetById(32));
  Func_0200560c();
  Func_02005784(0);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetModeById, 17, 2);
  FIELD_AT(actor, u8, 85) = 0;
  FIELD_AT(actor, s32, 8) -= 0x40000;
  FIELD_AT(actor, s32, 16) += 0x100000;
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 4 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetModeById, 28, 2);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetModeById, 29, 3);
  /* FAKEMATCH(wrapper): Camera_MoveWithinMapBounds called through call.h's
   * Call4; as a direct call, 34 of 1038 instructions differ in tla-en. */
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  wait(1);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetModeById, 30, 7);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetModeById, 31, 7);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetModeById, 32, 6);
  owner = (s32)gMainRoutineWork;
  owner = (owner + 0x214);
  t16 = *(s32 *)owner;
  lastArg = 0xc000;
  Func_02005684(t16, 0x1f80000, 0x1400000, lastArg);
  wait(1);
  /* FAKEMATCH(wrapper): Func_02005764 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_02005764, 0, 0);
  Func_0200575c(0, 0);
  Func_0200576c(1);
  Actor_SetPartAttribute((Actor_GetById(30)), 15);
  Actor_SetPartAttribute((Actor_GetById(31)), 15);
  Actor_SetPartAttribute((Actor_GetById(32)), 15);
  wait(1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetSpritePriority, 21, 1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetSpritePriority, 20, 1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetSpritePriority, 19, 1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetSpritePriority, 26, 1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetSpritePriority, 25, 1);
  t34 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t34, s32, 428) = 0x204;
  FIELD_AT(t34, s32, 436) = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020057dc(0x186);
  /* FAKEMATCH(wrapper): Func_0200575c called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_0200575c, 0x10003, 1);
  /* FAKEMATCH(wrapper): Func_0200575c called through call.h's Call2; as a
   * direct call, 12 of 1038 instructions differ in tla-en. */
  Call2(Func_0200575c, 0x10000, 2);
  Func_0200576c(16);
  wait(16);
  FieldEvent_Wait(80);
  *(s32 *)gData_02006b64 = 0;
  *(s32 *)gData_02006b6c = 4;
  FIELD_AT((Actor_GetById(17)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT((Actor_GetById(27)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT((Actor_GetById(28)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT((Actor_GetById(29)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT(t1, s32, 108) = (s32)Func_02001ee4;
  FIELD_AT(t2, s32, 108) = (s32)Func_02001ee4;
  FIELD_AT((spill), s32, 108) = (s32)Func_02001ee4;
  /* FAKEMATCH(wrapper): Func_020057dc called through call.h's Call1; as a
   * direct call, 31 of 1038 instructions differ in tla-en. */
  Call1(Func_020057dc, 0x253);
  wait(10);
  /* FAKEMATCH(wrapper): Func_020057dc called through call.h's Call1; as a
   * direct call, 27 of 1038 instructions differ in tla-en. */
  Call1(Func_020057dc, 0x253);
  wait(10);
  /* FAKEMATCH(wrapper): Func_020057dc called through call.h's Call1; as a
   * direct call, 23 of 1038 instructions differ in tla-en. */
  Call1(Func_020057dc, 0x253);
  wait(10);
  /* FAKEMATCH(wrapper): Func_020057dc called through call.h's Call1; as a
   * direct call, 15 of 1038 instructions differ in tla-en. */
  Call1(Func_020057dc, 0x253);
  wait(10);
  Func_020057dc(0x253);
  wait(40);
  *(s32 *)gData_02006b6c = 3;
  wait(30);
  *(s32 *)gData_02006b6c = 2;
  wait(20);
  /* FAKEMATCH(wrapper): Func_0200567c called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Func_0200567c, 17, 0, 0);
  /* FAKEMATCH(wrapper): Func_0200567c called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Func_0200567c, 27, 0, 0);
  /* FAKEMATCH(wrapper): Func_0200567c called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Func_0200567c, 28, 0, 0);
  Func_0200567c(29, 0, 0);
  FIELD_AT((Actor_GetById(17)), s32, 108) = 0;
  FIELD_AT((Actor_GetById(27)), s32, 108) = 0;
  FIELD_AT((Actor_GetById(28)), s32, 108) = 0;
  FIELD_AT((Actor_GetById(29)), s32, 108) = 0;
  FIELD_AT(t1, s32, 108) = 0;
  FIELD_AT(t2, s32, 108) = 0;
  FIELD_AT((spill), s32, 108) = 0;
  wait(1);
  Func_020057dc(103);
  Actor_SetPartAttribute((Actor_GetById(30)), 9);
  Actor_SetPartAttribute((Actor_GetById(31)), 9);
  Actor_SetPartAttribute((Actor_GetById(32)), 9);
  FieldEvent_Wait(20);
  Actor_SetPartAttribute((Actor_GetById(30)), 0);
  Actor_SetPartAttribute((Actor_GetById(31)), 0);
  Actor_SetPartAttribute((Actor_GetById(32)), 0);
  FieldEvent_Wait(20);
  t85 = Random16Far();
  FIELD_AT(t1, u8, 98) = (((u32)(((t85 << 2) + t85) << 4) >> 16) + 40);
  t86 = Random16Far();
  FIELD_AT(t2, u8, 98) = (((u32)(((t86 << 2) + t86) << 4) >> 16) + 40);
  t87 = Random16Far();
  FIELD_AT((spill), u8, 98) = (((u32)(((t87 << 2) + t87) << 4) >> 16) + 40);
  FIELD_AT(t1, s32, 108) = (s32)Func_0200008c;
  FIELD_AT(t2, s32, 108) = (s32)Func_0200008c;
  FIELD_AT((spill), s32, 108) = (s32)Func_0200008c;
  FieldEvent_Wait(20);
  Func_0200575c(0x10000, 1);
  Func_0200576c(40);
  wait(80);
  Func_020056dc((s32)MsgWhoAreThey);
  /* FAKEMATCH(wrapper): Func_02005704 called through call.h's Call2; as a
   * direct call, 24 of 1038 instructions differ in tla-en. */
  Call2(Func_02005704, 26, 0x8000);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 26, 0);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 19, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 25 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 22, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 26 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, (*(s32 *)owner), 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 30 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 23, 0x4000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 15 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 24, 0x4000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 23 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 21, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 24 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 20, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 10 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 26, 0x4000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 25, 0x4000, 0);
  /* FAKEMATCH(wrapper): Func_02005724 called through call.h's Call2; as a
   * direct call, 59 of 1038 instructions differ in tla-en. */
  Call2(Func_02005724, 0x19999, 0x3333);
  FIELD_AT((Func_0200573c()), u8, 85) = 0;
  Camera_MoveWithinMapBounds(0x2000000, 0x500000, 0x1540000, 1);
  Func_02005734();
  Func_02005714(19, 0x109, 20);
  Func_020056f4(19, 0);
  Func_020057dc(8);
  /* FAKEMATCH(wrapper): Func_0200571c called through call.h's Call2; as a
   * direct call, 8 of 1038 instructions differ in tla-en. */
  Call2(Func_0200571c, 24, 0x102);
  FieldEvent_Wait(20);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 24, 0);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 25, 0);
  /* FAKEMATCH(wrapper): Motion_SetModeAndWaitAnimation called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Motion_SetModeAndWaitAnimation, 19, 3);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 19, 0);
  /* FAKEMATCH(wrapper): Func_0200571c called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_0200571c, 26, 0x102);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 0x101a, 0);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetModeById, 19, 4);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 19, 0);
  Func_02005714(23, 0x103, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 51 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetSpeedParameters, 23, 0x19999, 0xcccc);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetPositionAndReset, 23, 0x200, 0x148);
  /* FAKEMATCH(wrapper): Func_02005704 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_02005704, 23, 0x2000);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 0x1017, 0);
  /* FAKEMATCH(wrapper): Func_02005704 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_02005704, 20, 0x8000);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 0xa014, 0);
  Func_02005714(19, 0x100, 20);
  Actor_SetSpritePriority(21, 2);
  FIELD_AT((Actor_GetById(20)), u8, 98) = 0;
  FIELD_AT((Actor_GetById(21)), u8, 98) = 0;
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 38 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetSpeedParameters, 21, 0xcccc, 0x6666);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetSpeedParameters, 20, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(21, (s32)gData_02005f08);
  Actor_SetMovementTypeById(20, (s32)gData_02005f50);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 4 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetSpeedParameters, 19, 0x26666, 0x13333);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetPositionAndReset, 19, 0x21c, 0x154);
  /* FAKEMATCH(wrapper): Actor_Jump called through call.h's Call3; as a direct
   * call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_Jump, 19, 4, 10);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 19, 0);
  actor = Actor_GetById(21);
L_0200a47a:
  wait(1);
  if (FIELD_AT(actor, u8, 98) == 0)
    goto L_0200a47a;
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 20, 0x2000, 0);
  /* FAKEMATCH(wrapper): Func_02005704 called through call.h's Call2; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Call2(Func_02005704, 21, 0x2000);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 21, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetSpritePriority, 20, 2);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_SetSpritePriority, 22, 2);
  /* FAKEMATCH(wrapper): this call through W_Func_02005714; as Func_02005714(20,
   * 0x100, 0), 103 of 1038 instructions differ in tla-en (as Call3, 2). */
  W_Func_02005714(20, 0x100, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 55 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetSpeedParameters, 20, 0x19999, 0xcccc);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetPositionAndReset, 20, 0x1ea, 0x128);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 21, 0xe000, 20);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 139 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 20, 0xa000, 20);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 20, 0xe000, 20);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 27 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetSpeedParameters, 20, 0x3333, 0x1999);
  t158 = Actor_GetById(20);
  FIELD_AT(t158, u8, 90) &= 254;
  Actor_SetPositionAndReset(20, 0x1e8, 0x12c);
  FieldEvent_Wait(1);
  t162 = Actor_GetById(20);
  FIELD_AT(t162, u8, 90) |= 1;
  FieldEvent_Wait(20);
  Motion_SetVarCbAndRefresh(20, 2);
  t166 = Actor_GetById(20);
  FIELD_AT(t166, u8, 90) &= 254;
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 53 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetPositionAndReset, 20, 0x1e6, 0x134);
  FieldEvent_Wait(1);
  t170 = Actor_GetById(20);
  FIELD_AT(t170, u8, 90) |= 1;
  /* FAKEMATCH(wrapper): Func_02005724 called through call.h's Call2; as a
   * direct call, 57 of 1038 instructions differ in tla-en. */
  Call2(Func_02005724, 0xcccc, 0x1999);
  Camera_MoveWithinMapBounds(0x2000000, 0x600000, 0x1400000, 1);
  Func_02005734();
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 21, 0, 40);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 21, 0xe000, 20);
  /* FAKEMATCH(wrapper): Func_02005704 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_02005704, 21, 0);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 0x8015, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 128 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 22, 0xc000, 0);
  owner2 = (s32)gMainRoutineWork;
  owner2 = (owner2 + 0x214);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 118 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, (*(s32 *)owner2), 0xa000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 111 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 23, 0xa000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 100 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 24, 0xa000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 107 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 26, 0xa000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 88 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 25, 0xa000, 0);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 100 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 0x8014, 0);
  actor = Actor_GetById(32);
  FIELD_AT(actor, s32, 108) = 0;
  wait(1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  /* FAKEMATCH(wrapper): Motion_SetVarCbAndRefresh called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Motion_SetVarCbAndRefresh, 32, 2);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 32, 0);
  Func_02005714(23, 0x100, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 23, 0xe000, 40);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 25, 0x8000, 0);
  Func_02005714(25, 0x101, 20);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 25, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 23, 0, 40);
  /* FAKEMATCH(wrapper): Func_02005704 called through call.h's Call2; as a
   * direct call, 179 of 1038 instructions differ in tla-en. */
  Call2(Func_02005704, 23, 0xc000);
  /* FAKEMATCH(wrapper): Actor_StartRepeatedMotion called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Actor_StartRepeatedMotion, 32, 3);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 32, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 162 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 24, 0xc000, 40);
  /* FAKEMATCH(wrapper): Func_02005704 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_02005704, 24, 0x6000);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 0x4018, 0);
  /* FAKEMATCH(wrapper): Func_02005704 called through call.h's Call2; as a
   * direct call, 91 of 1038 instructions differ in tla-en. */
  Call2(Func_02005704, 21, 0x2000);
  /* FAKEMATCH(wrapper): Func_0200571c called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_0200571c, 21, 0x102);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 21, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 159 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, (*(s32 *)owner2), 0xc000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 160 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 24, 0xc000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 155 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 26, 0xc000, 0);
  Actor_ArmCallback(25, 0xc000, 0);
  actor = Actor_GetById(30);
  FIELD_AT(actor, s32, 108) = 0;
  wait(1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  actor = Actor_GetById(31);
  FIELD_AT(actor, s32, 108) = 0;
  wait(1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  FieldEvent_Wait(10);
  /* FAKEMATCH(wrapper): Motion_SetVarCbAndRefresh called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Motion_SetVarCbAndRefresh, 31, 1);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 0x201f, 0);
  /* FAKEMATCH(wrapper): Motion_SetVarCbAndRefresh called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Motion_SetVarCbAndRefresh, 30, 2);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 30, 0);
  /* FAKEMATCH(wrapper): Actor_Jump called through call.h's Call3; as a direct
   * call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_Jump, 20, 2, 10);
  /* FAKEMATCH(wrapper): Actor_Jump called through call.h's Call3; as a direct
   * call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_Jump, 20, 4, 10);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 0x8014, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 85 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetSpeedParameters, 20, 0x26666, 0x13333);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 85 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetSpeedParameters, (*(s32 *)owner2), 0x26666, 0x13333);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 83 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetSpeedParameters, 23, 0x26666, 0x13333);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 31 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetSpeedParameters, 24, 0x26666, 0x13333);
  Actor_SetMovementTypeById(20, (s32)gData_02005f98);
  Actor_SetMovementTypeById((*(s32 *)owner2), (s32)gData_02005fdc);
  Actor_SetMovementTypeById(23, (s32)gData_02006004);
  Actor_SetMovementTypeAndWaitById(24, (s32)gData_0200602c);
  FieldEvent_Wait(20);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 18 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetSpeedParameters, 22, 0x19999, 0xcccc);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetPositionAndReset, 22, 0x210, 0x140);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 22, 0x2000, 0);
  /* FAKEMATCH(wrapper): Func_02005704 called through call.h's Call2; as a
   * direct call, 10 of 1038 instructions differ in tla-en. */
  Call2(Func_02005704, 25, 0xa000);
  /* FAKEMATCH(wrapper): Func_020056f4 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_020056f4, 0x5016, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetSpeedParameters, 25, 0x19999, 0xcccc);
  Actor_SetMovementTypeById(25, (s32)gData_02006070);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetPositionAndReset, 22, 0x1e6, 0x134);
  /* FAKEMATCH(wrapper): Func_02005704 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_02005704, 22, 0xa000);
  Actor_SetSpeedParameters(21, 0x26666, 0x13333);
  t244 = Actor_GetById(21);
  FIELD_AT(t244, u8, 90) &= 254;
  Actor_SetPositionAndReset(21, 0x1da, 0x13c);
  FieldEvent_Wait(1);
  t248 = Actor_GetById(21);
  FIELD_AT(t248, u8, 90) |= 1;
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_ArmCallback, 21, 0xe000, 0);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call3(Actor_SetPositionAndReset, 22, 0x1ce, 0x118);
  /* FAKEMATCH(wrapper): Func_02005704 called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Func_02005704, 22, 0);
  Actor_SetSpritePriority(22, 1);
  Func_020053e4((Actor_GetById(22)), (s32)gData_02006c20);
  Func_020053e4((Actor_GetById(25)), (s32)gData_02006b70);
  FieldEvent_Wait(40);
  /* FAKEMATCH(wrapper): Motion_SetModeAndWaitAnimation called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Call2(Motion_SetModeAndWaitAnimation, 19, 4);
  Func_020056f4(19, 0);
  Func_020057dc(78);
  Func_0200575c(0, 0);
  Func_0200576c(120);
  wait(120);
  Func_020054e4(Actor_GetById(22));
  Func_020054e4(Actor_GetById(25));
  FieldEvent_Wait(80);
  Func_02005744(2);
  return;
}
