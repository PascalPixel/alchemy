// Draft of overlay 6ae's Func_02003370, translated from its listing: differ
// 165 of 207. Left: its shape: 192 instructions against the listing's 207.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_0200556c();
s32 Func_02005574();
s32 Func_020055c4();
s32 Func_0200575c();
s32 Func_0200576c();
s32 Func_020057dc();
s32 Math_Sine();
s32 ObjectMotion_EnableActionAndSetCallback();
s32 Object_GetById();
s32 Object_SetPartAttribute();
s32 Random16Far();
s32 SceneActor_ParkRecord();
s32 Scheduler_RemoveCallbackFar();
extern u8 Data_02006b68[];
extern u8 Data_02006ccc[];
extern u8 Func_020000e4[];
extern u8 gData_02005b54[];
extern u8 gData_02006180[];
extern u8 gFrameCount[];

void Func_02003370(void) {
  s32 v_r3, v_r7, v_r6, v_r5, v_r0, v_r1, t0, t1, t9, t10;
  s32 t13, t14, t16, t17, t18, t20, t22, t23, t24, t25;
  s32 t26, t27, t28, t31, t32, t33, t34, t35, t36, t37;
  s32 t38;

  t0 = Value1((s32 (*)())Object_GetById, 8);
  t1 = *(s32 *)(s32)Data_02006ccc;
  v_r5 = (s32)Data_02006ccc;
  v_r6 = t0;
  v_r7 = 0;
  v_r3 = t1;
  if ((u32)v_r3 <= (u32)26)
    goto L_0200b38c;
  goto L_0200b506;
L_0200b38c:
  switch (v_r3) {
  case 0:
    goto L_0200b400;
  case 1:
    goto L_0200b506;
  case 2:
    goto L_0200b506;
  case 3:
    goto L_0200b506;
  case 4:
    goto L_0200b506;
  case 5:
    goto L_0200b506;
  case 6:
    goto L_0200b506;
  case 7:
    goto L_0200b506;
  case 8:
    goto L_0200b41a;
  case 9:
    goto L_0200b506;
  case 10:
    goto L_0200b506;
  case 11:
    goto L_0200b506;
  case 12:
    goto L_0200b506;
  case 13:
    goto L_0200b506;
  case 14:
    goto L_0200b506;
  case 15:
    goto L_0200b506;
  case 16:
    goto L_0200b42c;
  case 17:
    goto L_0200b506;
  case 18:
    goto L_0200b506;
  case 19:
    goto L_0200b506;
  case 20:
    goto L_0200b506;
  case 21:
    goto L_0200b506;
  case 22:
    goto L_0200b506;
  case 23:
    goto L_0200b506;
  case 24:
    goto L_0200b43e;
  case 25:
    goto L_0200b466;
  case 26:
    goto L_0200b4a8;
  }
L_0200b400:
  Call1((void (*)())Func_020057dc, 220);
  Call3((void (*)())Func_020055c4, 0x60000, 0x60000, 0x10000);
  v_r0 = 0x2063ff;
  goto L_0200b41e;
L_0200b41a:
  v_r0 = 0x10000;
L_0200b41e:
  Call2((void (*)())Func_0200575c, v_r0, 1);
  Call4((void (*)())Func_0200576c, 8, 0, 0, 0);
  goto L_0200b506;
L_0200b42c:
  Call3((void (*)())Func_020055c4, 0x10000, 0x10000, 0x10000);
  goto L_0200b506;
L_0200b43e:
  FIELD_AT(v_r6, s32, 8) = 0x2000000;
  FIELD_AT(v_r6, s32, 12) = 0xfe980000;
  FIELD_AT(v_r6, s32, 16) = 0xcc0000;
  FIELD_AT(v_r6, s32, 24) = 0x10000;
  FIELD_AT(v_r6, s32, 28) = 0x10000;
  Call1((void (*)())SceneActor_ParkRecord, v_r6);
  Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 8,
        (s32)gData_02005b54);
  goto L_0200b506;
L_0200b466:
  t9 = *(s32 *)v_r5;
  *(s32 *)v_r5 = (t9 - 1);
  t10 = FIELD_AT(v_r6, s32, 12);
  if (t10 <= 0)
    goto L_0200b488;
  Call2((void (*)())Func_0200575c, 0x203210, 0);
  Call4((void (*)())Func_0200576c, 16, 0, 0, 0);
  t13 = *(s32 *)v_r5;
  *(s32 *)v_r5 = (t13 + 1);
  goto L_0200b506;
L_0200b488:
  t14 = *(s32 *)(s32)gFrameCount;
  if ((t14 & 7) != 0)
    goto L_0200b49a;
  Call1((void (*)())Func_020057dc, 246);
L_0200b49a:
  t16 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(v_r6, s32, 12) = (t16 + 0x24000);
  v_r7 = 1;
  goto L_0200b506;
L_0200b4a8:
  t17 = *(s32 *)v_r5;
  *(s32 *)v_r5 = (t17 - 1);
  t18 = FIELD_AT(v_r6, s32, 12);
  if (t18 <= 0x140000)
    goto L_0200b4e8;
  *(s32 *)v_r5 = 0x3e7;
  Call1((void (*)())Scheduler_RemoveCallbackFar, (s32)Func_02003370);
  goto L_0200b506;
L_0200b4e8:
  t20 = *(s32 *)(s32)gFrameCount;
  if ((t20 & 7) != 0)
    goto L_0200b4fa;
  Call1((void (*)())Func_020057dc, 246);
L_0200b4fa:
  t22 = FIELD_AT(v_r6, s32, 12);
  FIELD_AT(v_r6, s32, 12) = (t22 + 0x24000);
  v_r7 = 1;
L_0200b506:
  if (v_r7 == 0)
    goto L_0200b5c0;
  t23 = Value0((s32 (*)())Random16Far);
  t24 = FIELD_AT(v_r6, s32, 12);
  t25 = FIELD_AT(v_r6, s32, 8);
  t26 = FIELD_AT(v_r6, s32, 16);
  t27 = Value4(
      (s32 (*)())Func_02005574, 0x2a0, t25,
      ((t24 - (((u32)(((t23 << 2) + t23) << 4) >> 16) << 16)) + 0xfff80000),
      t26);
  v_r0 = t27;
  v_r7 = t27;
  if (v_r0 == 0)
    goto L_0200b5c0;
  t28 = FIELD_AT(v_r7, s32, 80);
  Call2((void (*)())Func_0200556c, v_r0, (s32)gData_02006180);
  Call2((void (*)())Object_SetPartAttribute, v_r7, 1);
  *(u8 *)(v_r7 + 85) = 0;
  t31 = Value0((s32 (*)())Random16Far);
  *(u16 *)(v_r7 + 100) = (0xffff000 & t31);
  *(u16 *)(v_r7 + 102) = 0;
  t32 = Value0((s32 (*)())Random16Far);
  *(u8 *)(v_r7 + 98) = ((u32)t32 >> 13);
  FIELD_AT(v_r7, s32, 108) = (s32)Func_020000e4;
  t33 = Value0((s32 (*)())Random16Far);
  t34 = Value2((s32 (*)())Math_Sine, ((u32)((t33 << 16) - t33) >> 20), 0);
  FIELD_AT(v_r7, s32, 48) = (((t34 << 1) + t34) << 3);
  t35 = FIELD_AT(v_r6, s16, 50);
  FIELD_AT(v_r7, s32, 48) = t35;
  v_r3 = 0;
  v_r1 = t28;
  goto L_0200b5b0;
L_0200b5b0:
  t36 = FIELD_AT(v_r1, u8, 9);
  FIELD_AT(v_r1, u8, 26) = v_r3;
  FIELD_AT(v_r1, u8, 9) = ((-13 & t36) | 4);
L_0200b5c0:
  t37 = *(s32 *)(s32)Data_02006ccc;
  *(s32 *)(s32)Data_02006ccc = (t37 + 1);
  t38 = *(s32 *)(s32)Data_02006b68;
  if ((u32)(t37 + 1) <= (u32)t38)
    goto L_0200b5d6;
  Call1((void (*)())Scheduler_RemoveCallbackFar, (s32)Func_02003370);
L_0200b5d6:
  return;
}
