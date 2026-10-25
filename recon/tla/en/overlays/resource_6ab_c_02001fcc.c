// Draft of overlay 6ab's Func_02001fcc, translated from its listing: differ
// 134 of 108. Left: its shape: 106 instructions against the listing's 108.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_020043bc();
s32 Func_02004454();
s32 Func_02004464();
s32 Object_GetById();
s32 Party_CountActiveOwners();
s32 Unit_Get();
extern u8 gPartyState[];

void Func_02001fcc(void) {
  s32 sp_[1];
  s32 v_r6, v_r8, v_r7, v_r5, v_r10, v_r0, t0, t1, t3, t4;
  s32 t5, t6, t7, t10, t11, t12, t13, t14, t15, t16;

  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = Value0((s32 (*)())Func_02004464);
  Call1((void (*)())Object_GetById, t1);
  t3 = Value0((s32 (*)())Party_CountActiveOwners);
  v_r5 = 0;
  v_r7 = t3;
  v_r8 = t1;
  v_r6 = t0;
  if (v_r5 >= v_r7)
    goto L_0200a00e;
L_02009ff2:
  t4 = *(u8 *)((s32)gPartyState + (v_r5 + 0x218));
  t5 = Value1((s32 (*)())Unit_Get, t4);
  t6 = FIELD_AT(t5, u16, 56);
  *(u16 *)((s32)sp_ + (v_r5 << 1)) = t6;
  t7 = (v_r5 << 1);
  v_r5 = (v_r5 + 1);
  if (v_r5 < v_r7)
    goto L_02009ff2;
L_0200a00e:
  Call2((void (*)())Func_02004454, -10, 0);
  FIELD_AT(v_r6, u16, 364) = 0;
  FIELD_AT(v_r6, u16, 366) = 0;
  Call2((void (*)())Func_020043bc, v_r8, 0x102);
  v_r5 = 0;
  if (v_r5 >= v_r7)
    goto L_0200a0a8;
L_0200a03a:
  t10 = *(u8 *)((s32)gPartyState + (v_r5 + 0x218));
  t11 = Value1((s32 (*)())Unit_Get, t10);
  t12 = FIELD_AT(t11, s16, 56);
  v_r0 = t11;
  v_r8 = (v_r5 + 0x218);
  v_r10 = (s32)gPartyState;
  if (t12 <= 0)
    goto L_0200a062;
  t13 = FIELD_AT(v_r6, u16, 366);
  FIELD_AT(v_r6, u16, 366) = (t13 + 1);
  goto L_0200a0a2;
L_0200a062:
  t14 = *(s16 *)((s32)sp_ + (v_r5 << 1));
  if (t14 == 0)
    goto L_0200a0a2;
  t15 = FIELD_AT(v_r6, u16, 364);
  FIELD_AT(v_r6, u16, 364) = (t15 + 1);
  t16 = *(u8 *)(v_r10 + v_r8);
  *(u16 *)(v_r6 + (((t15 << 16) >> 15) + 0x170)) = t16;
  FIELD_AT(v_r6, u16, 362) = 0xffff;
  FIELD_AT(v_r0, u8, 305) = 0;
L_0200a0a2:
  v_r5 = (v_r5 + 1);
  if (v_r5 < v_r7)
    goto L_0200a03a;
L_0200a0a8:
  return;
}
