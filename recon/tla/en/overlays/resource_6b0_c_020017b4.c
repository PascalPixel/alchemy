// Draft of overlay 6b0's Func_020017b4, translated from its listing: differ
// 66 of 171. Left: instruction order and register choice.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Engine_ObjectCreate();
s32 Func_020032e0();
s32 Func_020032e8();
s32 Func_02003348();
s32 Func_020034b8();
s32 GameFlag_ClearBit();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Object_GetById();
s32 Object_SetPartAttribute();
s32 Object_SetSpritePriority();
s32 Random16Far();
s32 Vector_AddPolarOffsetFar();
extern u8 gData_02003594[];
extern u8 gFrameCount[];
extern u8 gPartyState[];

void Func_020017b4(s32 p0) {
  s32 sp_[1];
  s32 v_r5, v_r7, v_r6, v_r4, v_r1, v_r8, v_r0, v_r3, v_r2, t0;
  s32 t1, t2, t3, t4, t6, t7, t8, t9, t10, t11;
  s32 t12, t13, t14, t15, t16, t17, t18, t19, t20, t21;
  s32 t22, t23, t29, t30, t31, t33, t34, t36, t37, t38;
  s32 t39, t40, t41, t42, t43, t44, t45;

  v_r5 = ((s32)gPartyState + 0x214);
  t0 = *(s32 *)v_r5;
  t1 = Value1((s32 (*)())Object_GetById, t0);
  t2 = *(s32 *)v_r5;
  t3 = Value1((s32 (*)())Object_GetById, t2);
  t4 = FIELD_AT(t3, s16, 18);
  v_r6 = p0;
  v_r7 = t1;
  if (t4 <= 248)
    goto L_02009800;
  FIELD_AT(v_r6, s32, 108) = 0;
  Call1((void (*)())GameFlag_ClearBit, 0x200);
  t6 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(v_r6, s32, 8) = ((t6 & 0xfff00000) + 0x80000);
  t7 = FIELD_AT(v_r6, s32, 16);
  FIELD_AT(v_r6, s32, 16) = ((t7 & 0xfff00000) + 0x80000);
  goto L_02009920;
L_02009800:
  t8 = FIELD_AT(v_r7, s32, 8);
  t9 = FIELD_AT(v_r6, s32, 76);
  t10 = FIELD_AT(v_r6, s32, 8);
  FIELD_AT(sp_, s32, 8) = (t10 - (t8 - t9));
  *(s32 *)sp_ = (t10 - (t8 - t9));
  t11 = FIELD_AT(v_r7, s32, 12);
  FIELD_AT(sp_, s32, 12) = t11;
  t12 = FIELD_AT(v_r7, s32, 16);
  t13 = FIELD_AT(v_r6, s32, 52);
  t14 = FIELD_AT(v_r6, s32, 16);
  FIELD_AT(sp_, s32, 16) = (t14 + (t12 - t13));
  FIELD_AT(sp_, s32, 4) = (t14 + (t12 - t13));
  t15 = FIELD_AT(v_r7, s32, 8);
  v_r0 = t13;
  v_r8 = ((s32)sp_ + 8);
  v_r1 = t10;
  v_r4 = t14;
  if (t9 != t15)
    goto L_02009832;
  t16 = FIELD_AT(v_r7, s32, 16);
  if (v_r0 == t16)
    goto L_020098b2;
L_02009832:
  t17 = FIELD_AT(v_r6, s32, 12);
  t18 = Value4((s32 (*)())Engine_ObjectCreate, 0x2a0, v_r1, t17, v_r4);
  t19 = Value0((s32 (*)())Random16Far);
  t20 = FIELD_AT(t18, s32, 8);
  FIELD_AT(t18, s32, 8) = (t20 + (((15 & t19) - 8) << 16));
  t21 = Value0((s32 (*)())Random16Far);
  t22 = FIELD_AT(t18, s32, 12);
  FIELD_AT(t18, s32, 12) = (t22 + (((31 & t21) - 4) << 16));
  FIELD_AT(t18, u8, 85) = 0;
  t23 = FIELD_AT(v_r6, u8, 98);
  Call2((void (*)())Object_SetPartAttribute, t18, t23);
  Call2((void (*)())Object_SetSpritePriority, t18, 2);
  Call2((void (*)())Func_020032e0, t18, 1);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t18, 0);
  Call2((void (*)())Func_020032e8, t18, (s32)gData_02003594);
  t29 = FIELD_AT(v_r7, s32, 8);
  FIELD_AT(v_r6, s32, 76) = t29;
  t30 = FIELD_AT(v_r7, s32, 16);
  FIELD_AT(v_r6, s32, 52) = t30;
  t31 = *(s32 *)(s32)gFrameCount;
  if ((t31 & 3) != 0)
    goto L_020098b2;
  Call1((void (*)())Func_020034b8, 106);
L_020098b2:
  t33 = FIELD_AT(v_r8, s16, 10);
  if (t33 > 248)
    goto L_02009920;
  t34 = FIELD_AT(v_r7, u16, 6);
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x70000, (0x8000 - t34), v_r8);
  t36 = *(s32 *)v_r8;
  v_r5 = (v_r6 + 34);
  t37 = *(u8 *)v_r5;
  t38 = FIELD_AT(v_r8, s32, 8);
  t39 = Value3((s32 (*)())Func_02003348, t37, t36, t38);
  if (t39 == 255)
    goto L_02009920;
  t40 = *(s32 *)sp_;
  t41 = FIELD_AT(sp_, s32, 4);
  FIELD_AT(v_r6, s32, 8) = t40;
  FIELD_AT(v_r6, s32, 16) = t41;
  t42 = *(u8 *)v_r5;
  t43 = Value3((s32 (*)())Func_02003348, t42, t40, t41);
  v_r0 = t43;
  if (v_r0 != 52)
    goto L_02009908;
  t44 = FIELD_AT(0x3000000, s32, 108);
  v_r2 = 0;
  v_r1 = (t44 + 0x16a);
  v_r3 = 201;
  goto L_0200991c;
L_02009908:
  if (v_r0 != 51)
    goto L_02009920;
  t45 = FIELD_AT(0x3000000, s32, 108);
  v_r2 = 0;
  v_r1 = (t45 + 0x16a);
  v_r3 = 200;
L_0200991c:
  *(u16 *)v_r1 = v_r3;
  FIELD_AT(v_r6, s32, 108) = v_r2;
L_02009920:
  return;
}
