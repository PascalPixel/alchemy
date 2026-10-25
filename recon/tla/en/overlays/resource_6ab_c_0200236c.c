// Draft of overlay 6ab's Func_0200236c, translated from its listing: differ
// 58 of 138. Left: its shape: 130 instructions against the listing's 138.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_ApplyChildValues();
s32 FieldEvent_WaitFrames();
s32 Func_02001fcc();
s32 Func_020041cc();
s32 Func_0200422c();
s32 Func_020042dc();
s32 Func_020043c4();
s32 Func_020043fc();
s32 Func_0200444c();
s32 GameFlag_ClearBit();
s32 GameFlag_SetBit();
s32 Object_GetById();
s32 WaitFrames();
extern u8 Func_02002120[];
extern u8 gData_020023c4[];
extern u8 gPartyState[];

void Func_0200236c(void) {
  s32 v_r6, v_r5, v_r1, v_r7, v_r3, t0, t1, t2, t3, t8;
  s32 t9, t15, t16, t17, t18, t19, t20, t21, t22, t23;
  s32 t24, t25, t26, t27, t28, t29, t31, t32;

  t0 = FIELD_AT(0x3000000, s32, 108);
  v_r6 = (t0 + 0x1b2);
  t1 = *(u16 *)v_r6;
  v_r1 = 0x1b2;
  v_r5 = t0;
  if (t1 != 0)
    goto L_0200a398;
  t2 = *(s32 *)(v_r5 + 0x1ac);
  t3 = *(s32 *)(v_r5 + (v_r1 + 2));
  Call2((void (*)())Func_020043fc, t2, t3);
  *(u16 *)v_r6 = 1;
L_0200a398:
  Call1((void (*)())GameFlag_ClearBit, 0x106);
  *(u16 *)(v_r5 + 0x166) = 0;
  *(u16 *)(v_r5 + 0x15a) = 0;
  *(u16 *)(v_r5 + 0x15e) = 0;
  *(u16 *)(v_r5 + 0x15c) = 0;
  *(u16 *)(v_r5 + 0x164) = 0;
  Call4((void (*)())Func_02001fcc, -10, 0, 0, (v_r5 + 0x164));
  Call2((void (*)())Func_020043c4, 0x70000, 0xe000);
  t8 = *(s32 *)((s32)gPartyState + 0x214);
  t9 = Value1((s32 (*)())Object_GetById, t8);
  Call1((void (*)())GameFlag_SetBit, 0x106);
  Func_020042dc();
  Call1((void (*)())Func_0200444c, 0);
  Call1((void (*)())GameFlag_ClearBit, 0x106);
  FIELD_AT(t9, s32, 108) = (s32)Func_02002120;
  FIELD_AT(t9, u16, 6) = 0x4000;
  Call2((void (*)())Func_020041cc, t9, 49);
  v_r7 = ((s32)gData_020023c4 + 0x188);
  t15 = FIELD_AT(v_r7, s32, 108);
  t16 = *(s16 *)(t15 + 100);
  v_r6 = t9;
  if (t16 != 0)
    goto L_0200a484;
L_0200a424:
  t17 = FIELD_AT(v_r6, s32, 8);
  t18 = FIELD_AT(v_r6, s32, 16);
  t19 = Value3((s32 (*)())Func_0200422c, 0, t17, t18);
  t20 = FIELD_AT(v_r7, s32, 108);
  v_r1 = t20;
  if (t19 == 7)
    goto L_0200a444;
  t21 = FIELD_AT(v_r1, s32, 112);
  t22 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(v_r6, s32, 8) = (t22 + t21);
  t23 = FIELD_AT(v_r6, s32, 16);
  t24 = FIELD_AT(v_r1, s32, 120);
  v_r3 = (t23 + t24);
  goto L_0200a46c;
L_0200a444:
  t25 = FIELD_AT(v_r1, s32, 112);
  if (t25 == 0)
    goto L_0200a458;
  t26 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(v_r6, s32, 8) = ((t26 & 0xfff00000) + 0x80000);
L_0200a458:
  t27 = FIELD_AT(v_r7, s32, 108);
  t28 = FIELD_AT(t27, s32, 120);
  if (t28 == 0)
    goto L_0200a46e;
  t29 = FIELD_AT(v_r6, s32, 16);
  v_r3 = ((t29 & 0xfff00000) + 0x80000);
L_0200a46c:
  FIELD_AT(v_r6, s32, 16) = v_r3;
L_0200a46e:
  FIELD_AT(v_r7, u16, 4) = 0;
  Call1((void (*)())WaitFrames, 1);
  t31 = FIELD_AT(v_r7, s32, 108);
  t32 = *(s16 *)(t31 + 100);
  if (t32 == 0)
    goto L_0200a424;
L_0200a484:
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  FIELD_AT(v_r6, s32, 108) = 0;
  Call2((void (*)())Animation_ApplyChildValues, v_r6, 0);
  FIELD_AT(v_r7, u16, 4) = 0;
  return;
}
