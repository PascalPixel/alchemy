// Draft of overlay 6a9's Func_0200087c, translated from its listing: differ
// 175 of 255. Left: its shape: 264 instructions against the listing's 255.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_0200015c();
s32 Func_020003b4();
s32 Func_020003f0();
s32 Func_0200053c();
s32 Func_0200108c();
s32 Func_020010a4();
s32 Func_020010cc();
s32 Func_0200115c();
s32 Func_02001164();
s32 GameFlag_SetBit();
s32 Map_GetTerrainHeight();
s32 Math_Cosine();
s32 Math_Sine();
s32 OverlayObject_PrepareObject();
s32 OverlayObject_WaitUntilIdle();
s32 Random16Far();
s32 WaitTicks();
extern u8 Func_020007ec[];
extern u8 Func_02000848[];

void Func_0200087c(void) {
  s32 sp_[1];
  s32 spo_[4];
  s32 v_r7, v_r2, v_r1, v_r5, v_r11, v_r9, v_r0, v_r8, v_r6, v_r10;
  s32 t0, t1, t2, t3, t4, t7, t8, t9, t10, t12;
  s32 t13, t14, t15, t16, t17, t18, t27, t28, t29, t30;
  s32 t31, t32, t33, t34, t35, t36, t37, t38, t40, t41;
  s32 t42, t43, t45, t46, t47, t48, t49, t51, t52, t54;
  s32 t55, t56, t57, t58;

  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = *(s32 *)0x30000e0;
  t2 = FIELD_AT(t0, s32, 3260);
  v_r1 = t1;
  v_r2 = t0;
  v_r7 = t2;
  if (v_r1 == 0)
    goto L_020088c4;
  t3 = FIELD_AT(v_r2, s16, 358);
  if (t3 == 0)
    goto L_020088c4;
  t4 = FIELD_AT(v_r1, s16, 28);
  if (t4 == 147)
    goto L_020088cc;
  Call1((void (*)())Func_0200053c, -1);
  goto L_020088cc;
L_020088c4:
  Call1((void (*)())Func_0200053c, -1);
L_020088cc:
  t7 = FIELD_AT(v_r7, s32, 8);
  t8 = FIELD_AT(v_r7, s32, 16);
  t9 = Value3((s32 (*)())Map_GetTerrainHeight, 0, t7, t8);
  t10 = FIELD_AT(v_r7, s32, 12);
  if (t9 != t10)
    goto L_020088de;
  goto L_02008a60;
L_020088de:
  FIELD_AT(v_r7, u8, 34) = 2;
  FIELD_AT(v_r7, u8, 85) = 3;
  FIELD_AT(v_r7, s32, 68) = 0;
  Call1((void (*)())WaitTicks, 1);
  t12 = FIELD_AT(v_r7, s32, 8);
  t13 = FIELD_AT(v_r7, s32, 16);
  t14 = Value3((s32 (*)())Func_020010a4, 2, t12, t13);
  v_r8 = 0;
  v_r0 = t14;
  v_r9 = (v_r7 + 85);
  v_r11 = (v_r7 + 34);
  v_r5 = 2;
  if (v_r0 == v_r8)
    goto L_0200897a;
  t15 = FIELD_AT(v_r7, u8, 35);
  FIELD_AT(v_r7, u8, 35) = (t15 | 2);
  *(u16 *)0x4000052 = 0x1000;
  t16 = FIELD_AT(v_r7, s32, 16);
  *(u16 *)0x4000050 = 0x3f10;
  t17 = FIELD_AT(v_r7, s32, 8);
  t18 = Value4((s32 (*)())OverlayObject_PrepareObject, t17, 0,
               (t16 + ((v_r0 + 2) << 20)), 0x138);
  Call1((void (*)())Func_02001164, 204);
  Call1((void (*)())OverlayObject_WaitUntilIdle, v_r7);
  Call1((void (*)())OverlayObject_WaitUntilIdle, v_r7);
  v_r6 = 0x4000050;
  v_r5 = t18;
  goto L_02008960;
L_02008960:
  FIELD_AT(v_r7, s32, 16) = v_r8;
  FIELD_AT(v_r7, s32, 12) = v_r8;
  FIELD_AT(v_r7, s32, 8) = v_r8;
  Call1((void (*)())Func_0200108c, v_r5);
  Call1((void (*)())WaitTicks, 1);
  *(u16 *)v_r6 = v_r8;
  goto L_02008a1e;
L_0200897a:
  Call1((void (*)())OverlayObject_WaitUntilIdle, v_r7);
  Call1((void (*)())OverlayObject_WaitUntilIdle, v_r7);
  Call1((void (*)())Func_02001164, 188);
  FIELD_AT(sp_, s32, 4) = 7;
  *(s32 *)sp_ = v_r5;
  FIELD_AT(sp_, s32, 36) = (s32)Func_020007ec;
  FIELD_AT(sp_, s32, 8) = 0xcccc;
  FIELD_AT(sp_, s32, 12) = 0xcccc;
  v_r8 = (s32)sp_;
  v_r10 = 0;
L_020089a8:
  v_r5 = (v_r10 << 12);
  t27 = Value1((s32 (*)())Math_Cosine, v_r5);
  FIELD_AT(sp_, s32, 40) = (t27 << 1);
  FIELD_AT(sp_, s32, 44) = 0;
  t28 = Value2((s32 (*)())Math_Sine, v_r5, 0);
  t29 = FIELD_AT(sp_, s32, 40);
  FIELD_AT(sp_, s32, 40) = (t29 + (t29 >> 1));
  FIELD_AT(sp_, s32, 48) = (t28 << 1);
  t30 = Value0((s32 (*)())Random16Far);
  t31 = FIELD_AT(sp_, s32, 40);
  FIELD_AT(sp_, s32, 40) =
      ((t31 + ((u32)(((t30 << 1) + t30) << 14) >> 16)) - 0x6000);
  t32 = Value0((s32 (*)())Random16Far);
  t33 = FIELD_AT(sp_, s32, 48);
  t34 = FIELD_AT(sp_, s32, 44);
  v_r5 = ((t33 + ((u32)(((t32 << 1) + t32) << 13) >> 16)) - 0x3000);
  FIELD_AT(sp_, s32, 48) = v_r5;
  t35 = FIELD_AT(v_r7, s32, 8);
  t36 = FIELD_AT(v_r7, s32, 16);
  t37 = FIELD_AT(v_r7, s32, 12);
  t38 = FIELD_AT(sp_, s32, 40);
  Func_0200015c(t35, t37, t36, t38, t34, v_r5, 0x10b0000, v_r8);
  v_r10 = (v_r10 + 1);
  if ((u32)v_r10 <= (u32)16)
    goto L_020089a8;
L_02008a1e:
  *(u8 *)v_r9 = 0;
  *(u8 *)v_r11 = 0;
  t40 = FIELD_AT(v_r7, s32, 8);
  FIELD_AT(sp_, s32, 40) = t40;
  t41 = FIELD_AT(v_r7, s32, 12);
  FIELD_AT(sp_, s32, 44) = (t41 - 0x100000);
  t42 = FIELD_AT(v_r7, s32, 16);
  FIELD_AT(sp_, s32, 48) = t42;
  t43 = Value2((s32 (*)())Func_0200115c, ((s32)sp_ + 40), 1);
  v_r0 = t43;
  if (v_r0 == 0)
    goto L_02008a60;
  FIELD_AT(v_r7, s32, 108) = (s32)Func_02000848;
  FIELD_AT(v_r7, u8, 98) = 1;
  FIELD_AT(v_r0, u8, 98) = 1;
  Call1((void (*)())GameFlag_SetBit, 0x212);
L_02008a60:
  t45 = FIELD_AT(v_r7, s32, 8);
  t46 = FIELD_AT(v_r7, s32, 16);
  t47 = Value3((s32 (*)())Map_GetTerrainHeight, 0, t45, t46);
  t48 = FIELD_AT(v_r7, s32, 8);
  t49 = FIELD_AT(v_r7, s32, 16);
  v_r5 = ((t47 >> 19) + 2);
  Call4((void (*)())Func_020010cc, t48, t49, 0, v_r5);
  t51 = FIELD_AT(v_r7, s32, 8);
  t52 = FIELD_AT(v_r7, s32, 16);
  Call4((void (*)())Func_020010cc, t51, t52, 2, v_r5);
  t54 = FIELD_AT(v_r7, s32, 8);
  t55 = FIELD_AT(v_r7, s32, 16);
  t56 = Value3((s32 (*)())Func_020003b4, 0, (t54 >> 20), (t55 >> 20));
  t57 = FIELD_AT(v_r7, s32, 8);
  t58 = FIELD_AT(v_r7, s32, 16);
  FIELD_AT(v_r7, s32, 76) = t56;
  Call4((void (*)())Func_020003f0, 0, (t57 >> 20), (t58 >> 20), (128 | t56));
  return;
}
