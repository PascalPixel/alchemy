// Draft of overlay 6b0's Func_0200088c, translated from its listing: differ
// 97 of 130. Left: its shape: 127 instructions against the listing's 130.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_SetStateFlags();
s32 ArcTan2();
s32 Engine_ObjectCreate();
s32 Func_020006f8();
s32 Func_020032e0();
s32 Func_020032e8();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Object_SetPartAttribute();
s32 Object_SetSpritePriority();
s32 Random16Far();
s32 Vector_AddPolarOffsetFar();
extern u8 IwramFillWords[];
extern u8 gData_02003684[];

void Func_0200088c(s32 p0, s32 p1, s32 p2) {
  s32 sp_[1];
  s32 v_r10, v_r3, v_r7, v_r1, v_r8, v_r9, v_r6, v_r0, t0, t1;
  s32 t2, t3, t4, t5, t6, t7, t8, t9, t10, t11;
  s32 t12, t13, t14, t15, t16, t17, t19, t24, t25, t26;

  FIELD_AT(sp_, s32, 4) = ((p2 << 16) >> 16);
  t0 = FIELD_AT(p0, s32, 4);
  t1 = *(s32 *)p0;
  t2 = FIELD_AT(p0, s32, 8);
  t3 = Engine_ObjectCreate(0x2a0, t1, t0, t2);
  t4 = *(s32 *)p1;
  t5 = *(s32 *)p0;
  t6 = FIELD_AT(p0, s32, 8);
  t7 = FIELD_AT(p1, s32, 8);
  t8 = ArcTan2((t6 - t7), (t5 - t4));
  *(s32 *)sp_ = (t3 + 100);
  FIELD_AT(t3, u16, 100) = t8;
  t9 = Random16Far();
  t10 = Random16Far();
  t11 = FIELD_AT(sp_, u8, 4);
  FIELD_AT(t3, u16, 102) = (((15 & t10) * ((2 & t9) - 1)) << 8);
  FIELD_AT(t3, u8, 98) = t11;
  FIELD_AT(t3, u8, 85) = 0;
  t12 = *(s32 *)p1;
  v_r6 = p0;
  v_r9 = p1;
  v_r8 = ((t6 - t7) >> 16);
  v_r1 = p1;
  v_r7 = t3;
  v_r3 = t12;
  v_r10 = ((t5 - t4) >> 16);
  goto L_02008918;
L_02008918:
  FIELD_AT(v_r7, s32, 68) = v_r3;
  t13 = FIELD_AT(v_r1, s32, 8);
  FIELD_AT(v_r7, s32, 72) = t13;
  t14 = ((s32 (*)())((s32)IwramFillWords + 0x74))(
      ((v_r10 * v_r10) + (v_r8 * v_r8)));
  FIELD_AT(v_r7, s32, 76) = (t14 << 16);
  v_r0 = (t14 << 16);
  if (v_r0 >= 0)
    goto L_0200893c;
  v_r0 = (v_r0 + 15);
L_0200893c:
  FIELD_AT(v_r7, s32, 48) = (v_r0 >> 4);
  t15 = FIELD_AT(v_r6, s32, 4);
  FIELD_AT(v_r7, s32, 52) = t15;
  t16 = FIELD_AT(v_r6, s32, 4);
  t17 = FIELD_AT(v_r9, s32, 4);
  v_r0 = (t16 - t17);
  if (v_r0 >= 0)
    goto L_02008952;
  v_r0 = (v_r0 + 15);
L_02008952:
  FIELD_AT(v_r7, s32, 20) = (v_r0 >> 4);
  Animation_SetStateFlags(v_r7, 1);
  t19 = FIELD_AT(sp_, s32, 4);
  Object_SetPartAttribute(v_r7, ((u32)(t19 << 16) >> 16));
  Object_SetSpritePriority(v_r7, 2);
  Func_020032e0(v_r7, 7);
  ObjectDispatch_SetSingleChildFlags(v_r7, 0);
  t24 = *(s32 *)sp_;
  t25 = FIELD_AT(v_r7, s32, 76);
  t26 = *(s16 *)t24;
  Vector_AddPolarOffsetFar(t25, t26, (v_r7 + 8));
  Func_020032e8(v_r7, (s32)gData_02003684);
  Func_020006f8(v_r7);
  return;
}
