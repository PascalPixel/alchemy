// Under -mthumb-slow-loads (2026-10-26): differ 6 of 175; left: in three places
// the object's x loads one row before the constant added to it.
// Draft of overlay 6a0's Func_020005e8, translated from its listing: differ 6
// of 175. Left: instruction order around
//   movs r3, #0
//   strb r3, [r2, #0]
//   + ldr r3, [r5, #12]
//   ldr r2, [pc, #260]
//   - ldr r3, [r5, #12]
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
s32 Func_020005a0();
void Func_02002b2c();
void Func_02002b54();
void Func_02002ba4();
s32 GameFlag_Test();
void ObjectMotion_EnableActionAndResetMotion();
s32 Object_GetById();
void WaitTicks();

void Func_020005e8(s32 a0) {
  s32 t1, t6, t7, t9, t16, t17, t19, t20, v_r5, v_r6, v_r7;

  v_r6 = a0;
  t1 = Value1((s32 (*)())GameFlag_Test, 0xa1a);
  if (t1 != 0)
    goto L_020086a2;
  v_r5 = 72;
  Call6((void (*)())Func_02002b2c, 80, 46, 1, 7, v_r5, 9);
  Call6((void (*)())Func_02002b2c, 80, 53, 8, 5, v_r5, 16);
  Call6((void (*)())Func_02002b2c, 78, 47, 1, 1, 8, 74);
  Call4((void (*)())Func_02002b54, 0x800000, 0xc00000, 0, 4);
  if (v_r6 != 1)
    goto L_02008682;
  v_r7 = 0;
L_0200864c:
  v_r6 = 0;
L_0200864e:
  t6 = Value1((s32 (*)())Object_GetById, (v_r6 + 10));
  v_r5 = t6;
  t7 = Value0((s32 (*)())Func_020005a0);
  if (t7 == 0)
    goto L_02008670;
  FIELD_AT(v_r5, u8, 85) = 0;
  FIELD_AT(v_r5, s32, 12) = (FIELD_AT(v_r5, s32, 12) + -0x20000);
L_02008670:
  v_r6 = (v_r6 + 1);
  if (v_r6 <= 5)
    goto L_0200864e;
  v_r7 = (v_r7 + 1);
  Call1((void (*)())WaitTicks, 1);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 8 of 175 instructions differ in
   * tla-en. */
  asm("");
  if (v_r7 <= 15)
    goto L_0200864c;
L_02008682:
  v_r6 = 0;
L_02008684:
  t9 = Value1((s32 (*)())Object_GetById, (v_r6 + 10));
  v_r5 = t9;
  FIELD_AT(v_r5, u8, 85) = 3;
  v_r6 = (v_r6 + 1);
  FIELD_AT(v_r5, s32, 12) = FIELD_AT(v_r5, s32, 20);
  if (v_r6 <= 5)
    goto L_02008684;
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 10 of 175 instructions differ in
   * tla-en. */
  asm("");
  goto L_0200876a;
L_020086a2:
  v_r5 = 72;
  Call6((void (*)())Func_02002b2c, 72, 46, 1, 7, v_r5, 9);
  Call6((void (*)())Func_02002b2c, 72, 53, 8, 5, v_r5, 16);
  Call6((void (*)())Func_02002b2c, 70, 47, 1, 1, 8, 74);
  Call4((void (*)())Func_02002b54, 0x800000, 0xc00000, 0, 6);
  Call1((void (*)())ObjectMotion_EnableActionAndResetMotion,
        gPartyState.currentOwner);
  Call3((void (*)())Func_02002ba4, 20, 0, 0);
  if (v_r6 != 1)
    goto L_02008742;
  v_r7 = 0;
L_0200870a:
  v_r6 = 0;
L_0200870c:
  t16 = Value1((s32 (*)())Object_GetById, (v_r6 + 10));
  v_r5 = t16;
  t17 = Value0((s32 (*)())Func_020005a0);
  if (t17 == 0)
    goto L_02008730;
  FIELD_AT(v_r5, u8, 85) = 0;
  FIELD_AT(v_r5, s32, 12) = (FIELD_AT(v_r5, s32, 12) + 0x20000);
L_02008730:
  v_r6 = (v_r6 + 1);
  if (v_r6 <= 5)
    goto L_0200870c;
  v_r7 = (v_r7 + 1);
  Call1((void (*)())WaitTicks, 1);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 8 of 175 instructions differ in
   * tla-en. */
  asm("");
  if (v_r7 <= 15)
    goto L_0200870a;
L_02008742:
  v_r6 = 0;
L_02008744:
  t19 = Value1((s32 (*)())Object_GetById, (v_r6 + 10));
  v_r5 = t19;
  t20 = Value0((s32 (*)())Func_020005a0);
  if (t20 == 0)
    goto L_02008764;
  FIELD_AT(v_r5, u8, 85) = 4;
  FIELD_AT(v_r5, s32, 12) = 0x208000;
L_02008764:
  v_r6 = (v_r6 + 1);
  if (v_r6 <= 5)
    goto L_02008744;
L_0200876a:
}
