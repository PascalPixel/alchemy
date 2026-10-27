// 26 October 2026, under -mthumb-slow-loads: differ 8 of 288: the loop's
// argument 1 and, after a byte store, the native issues a load one slot later
// after a store, where alchemy-gcc issues it at once; alchemy permute (5 to 6
// minutes) and an empty asm or one-pass block after any one statement do not
// move it.
// Draft of overlay 670's Func_02004a28, translated from its listing: differ 6
// of 288. Left: instruction order around
//   lsls r1, r1, #11
//   ldr r3, [r2, #12]
//   - movs r0, #1
//   adds r3, r3, r1
//   str r3, [r2, #12]
// (- the listing, + this draft). Tried: Call, Value and direct forms of each
// call, empty asms and one-pass blocks between statements.
#include "types.h"
#include "party_state.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void FieldEvent_WaitTicks();
void Func_02003640();
void Func_020036e0();
void Func_02004524();
void Func_02005b0c();
void Func_02005b4c();
void Func_02005b5c();
void Func_02005b64();
void Func_02005b7c();
void Func_02005b94();
void Func_02005b9c();
void Func_02005bc4();
void Func_02005c9c();
void GameFlag_SetBit();
void Motion_SetModeAndWaitAnimation();
void ObjectMotion_SetActionVariant();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
void Object_SetModeById();
void Object_SetPartAttribute();
void Object_SetPositionAndResetMotion();
void WaitTicks();
extern u8 MsgLookTheGreatGabomba[];
extern u8 gData_02007950[];

void Func_02004a28(void) {
  s32 t1, t5, t7, t10, t12, t22, t43, v_r5, v_r6;

  t1 = Value1((s32 (*)())Object_GetById, 14);
  v_r6 = t1;
  Func_02004524();
  Call2((void (*)())ObjectMotion_SetActionVariant, 17, 1);
  Call1((void (*)())Func_02005c9c, 0x240);
  t5 = Value1((s32 (*)())Object_GetById, 12);
  Call2((void (*)())Object_SetPartAttribute, t5, 7);
  t7 = Value1((s32 (*)())Object_GetById, 13);
  Call2((void (*)())Object_SetPartAttribute, t7, 7);
  Call1((void (*)())WaitTicks, 8);
  t10 = Value1((s32 (*)())Object_GetById, 12);
  Call2((void (*)())Object_SetPartAttribute, t10, 0);
  t12 = Value1((s32 (*)())Object_GetById, 13);
  Call2((void (*)())Object_SetPartAttribute, t12, 0);
  Call1((void (*)())WaitTicks, 8);
  Call3((void (*)())Func_02005b94, 17, 0x100, 20);
  Call1((void (*)())Func_02005b4c, (s32)&MsgLookTheGreatGabomba);
  Call3((void (*)())Func_02005b5c, 17, 0, 20);
  Call1((void (*)())Func_02005c9c, 0x240);
  Call2((void (*)())Func_020036e0, 1, 0);
  Call1((void (*)())FieldEvent_WaitTicks, 20);
  Call1((void (*)())Func_02005c9c, 233);
  FIELD_AT(v_r6, s32, 28) = 0x1999;
  t22 = Value1((s32 (*)())Object_GetById, 14);
  Call4((void (*)())Object_SetPositionAndResetMotion, t22, 0x1080000, 0x180000,
        0x1340000);
  Call1((void (*)())WaitTicks, 1);
  Call2((void (*)())Object_SetModeById, 14, 3);
  v_r5 = 0;
L_0200cae8:
  FIELD_AT(v_r6, s32, 28) = (FIELD_AT(v_r6, s32, 28) + 0x400);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 8 of 288 instructions differ in
   * tla-en. */
  asm("");
  FIELD_AT(v_r6, s32, 12) = (FIELD_AT(v_r6, s32, 12) + 0x2000);
  v_r5 = (v_r5 + 1);
  Call1((void (*)())WaitTicks, 1);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 8 of 288 instructions differ in
   * tla-en. */
  asm("");
  if ((u32)v_r5 <= (u32)63)
    goto L_0200cae8;
  Call1((void (*)())Func_02005c9c, 0x12a);
  if (FIELD_AT(gData_02007950, s32, 0) == 0)
    goto L_0200cb48;
  FIELD_AT(FIELD_AT(gData_02007950, s32, 0), u8, 85) = 0;
  if (FIELD_AT(FIELD_AT(gData_02007950, s32, 0), s32, 12) <= 0x240000)
    goto L_0200cb48;
L_0200cb2a:
  v_r5 = (s32)&gData_02007950;
  FIELD_AT(FIELD_AT(v_r5, s32, 0), s32, 12) =
      (FIELD_AT(FIELD_AT(v_r5, s32, 0), s32, 12) + -0x8000);
  Call1((void (*)())WaitTicks, 1);
  if (FIELD_AT(FIELD_AT(v_r5, s32, 0), s32, 12) > 0x240000)
    goto L_0200cb2a;
L_0200cb48:
  Call1((void (*)())Func_02005c9c, 232);
  if (FIELD_AT(gData_02007950, s32, 0) == 0)
    goto L_0200cbb2;
  FIELD_AT(FIELD_AT(gData_02007950, s32, 0), u8, 85) = 0;
  v_r5 = 0;
L_0200cb5e:
  FIELD_AT(FIELD_AT(gData_02007950, s32, 0), s32, 12) =
      (FIELD_AT(FIELD_AT(gData_02007950, s32, 0), s32, 12) + 0x60000);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 8 of 288 instructions differ in
   * tla-en. */
  asm("");
  FIELD_AT(v_r6, s32, 28) = (FIELD_AT(v_r6, s32, 28) + -0x2000);
  v_r5 = (v_r5 + 1);
  FIELD_AT(v_r6, s32, 12) = (FIELD_AT(v_r6, s32, 12) + -0x8000);
  Call1((void (*)())WaitTicks, 1);
  if ((u32)v_r5 <= (u32)3)
    goto L_0200cb5e;
  Call4((void (*)())Object_SetPositionAndResetMotion,
        FIELD_AT(gData_02007950, s32, 0), 0, 0, 0);
  v_r5 = 0;
L_0200cb96:
  FIELD_AT(v_r6, s32, 28) = (FIELD_AT(v_r6, s32, 28) + -0x2000);
  FIELD_AT(v_r6, s32, 12) = (FIELD_AT(v_r6, s32, 12) + -0x8000);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 8 of 288 instructions differ in
   * tla-en. */
  asm("");
  v_r5 = (v_r5 + 1);
  Call1((void (*)())WaitTicks, 1);
  if ((u32)v_r5 <= (u32)3)
    goto L_0200cb96;
L_0200cbb2:
  Call3((void (*)())Func_02005b0c, 14, 0, 0);
  Call2((void (*)())Func_020036e0, 0, 0);
  Call2((void (*)())Func_02003640, 1, 1);
  Call1((void (*)())FieldEvent_WaitTicks, 10);
  Call2((void (*)())Func_02003640, 0, 1);
  Call1((void (*)())FieldEvent_WaitTicks, 40);
  Call1((void (*)())Func_02005c9c, 0x240);
  Call2((void (*)())Func_02003640, 1, 1);
  Call2((void (*)())Func_020036e0, 1, 0);
  Call1((void (*)())Func_02005c9c, 132);
  t43 = Value1((s32 (*)())Object_GetById, 14);
  Call4((void (*)())Object_SetPositionAndResetMotion, t43, 0x1080000, 0x180000,
        0x1340000);
  Call1((void (*)())WaitTicks, 1);
  v_r5 = 0;
L_0200cc1c:
  FIELD_AT(v_r6, s32, 28) = (FIELD_AT(v_r6, s32, 28) + 0x2000);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 8 of 288 instructions differ in
   * tla-en. */
  asm("");
  FIELD_AT(v_r6, s32, 12) = (FIELD_AT(v_r6, s32, 12) + 0x8000);
  v_r5 = (v_r5 + 1);
  Call1((void (*)())WaitTicks, 1);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 8 of 288 instructions differ in
   * tla-en. */
  asm("");
  if ((u32)v_r5 <= (u32)7)
    goto L_0200cc1c;
  Call2((void (*)())Func_02005b9c, 17, 0x102);
  Call1((void (*)())FieldEvent_WaitTicks, 40);
  Call2((void (*)())Func_02005b7c, 17, 0x3000);
  Call2((void (*)())Func_02005b64, 0x4011, 0);
  Value3((s32 (*)())Func_02005b94, 17, 0x108, 20);
  Call2((void (*)())Func_02005b64, 0x4011, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 17, 3);
  Call2((void (*)())Func_02005b64, 0x4011, 0);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 17, 0xcccc, 0x6666);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 17, 0x108, 0x140);
  Call3((void (*)())Func_02005b0c, 17, 0, 0);
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x203;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())GameFlag_SetBit, 0x8fb);
  Call1((void (*)())Func_02005bc4, 13);
}
