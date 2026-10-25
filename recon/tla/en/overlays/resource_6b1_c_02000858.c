// Draft of overlay 6b1's Func_02000858, translated from its listing: differ
// 376 of 260. Left: its shape: 274 instructions against the listing's 260.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02000c3c();
s32 Func_02000d5c();
s32 __umodsi3();
extern u8 gData_02001cdc[];
extern u8 gData_02001cde[];
extern u8 gData_02001ce0[];
extern u8 gData_02001d20[];
extern u8 gData_02001de0[];
extern u8 gData_02001de2[];
extern u8 gData_02001de4[];
extern u8 gFrameCount[];
extern u8 gResourceTableEntries[];

void Func_02000858(void) {
  s32 sp_[1];
  s32 v_r9, v_r10, v_r8, v_r7, v_r6, v_r3, v_r5, v_r1, v_r2, v_r11;
  s32 v_r12, v_r4, v_r0, t0, t1, t2, t3, t4, t5, t6;
  s32 t7, t8, t9, t10, t11, t12, t13, t14, t15, t16;
  s32 t17, t18, t19, t20, t21, t22, t23, t24, t25, t26;
  s32 t27, t28, t29, t30, t31, t32, t33, t34, t36;

  t0 = FIELD_AT(0x3000000, s32, 32);
  t1 = FIELD_AT(t0, s32, 228);
  t2 = FIELD_AT(t0, s32, 232);
  t3 = *(s16 *)(s32)gData_02001cdc;
  t4 = FIELD_AT(((t3 << 2) + (s32)gResourceTableEntries), u16, 2);
  FIELD_AT(sp_, s32, 4) = ((u32)t4 >> 5);
  t5 = *(s32 *)t0;
  t6 = FIELD_AT(t5, s32, 4);
  *(s32 *)sp_ = t6;
  t7 = *(s16 *)(s32)gData_02001de4;
  t8 = *(s32 *)(s32)gFrameCount;
  t9 = __umodsi3(((u32)t8 >> 2), 5, (s32)gResourceTableEntries);
  FIELD_AT(sp_, s32, 8) = (((t9 << 5) + 32) * t7);
  t10 = *(s16 *)(s32)gData_02001cde;
  v_r7 = (s32)gData_02001d20;
  v_r8 = (t2 & 0xffff0000);
  v_r10 = (t1 & 0xffff0000);
  v_r9 = 0;
  if (v_r9 < t10)
    goto L_020088ce;
  goto L_02008a5a;
L_020088ce:
  t11 = *(s32 *)((s32)gData_02001ce0 + (v_r9 << 2));
  v_r6 = t11;
  if (v_r6 != 0)
    goto L_020088dc;
  goto L_02008a4a;
L_020088dc:
  t12 = FIELD_AT(v_r6, s32, 8);
  v_r3 = t12;
  if (v_r3 != 0)
    goto L_020088e4;
  goto L_02008a4a;
L_020088e4:
  t13 = *(s32 *)sp_;
  t14 = FIELD_AT(v_r6, s32, 12);
  t15 = *(s32 *)sp_;
  t16 = FIELD_AT(v_r6, s32, 16);
  v_r5 = (v_r3 - v_r10);
  t17 = *(s16 *)(s32)gData_02001de0;
  v_r12 = t17;
  v_r11 = (((((t14 - t13) + 0x80000) + ((t16 - v_r8) - t15)) >> 16) + 58);
  v_r2 = ((((t16 - v_r8) - t15) - ((t14 - t13) + 0x80000)) >> 16);
  v_r1 = (v_r5 >> 16);
  v_r3 = (v_r6 + 100);
  if (v_r12 != 0)
    goto L_020089b2;
  t18 = *(s16 *)v_r3;
  v_r0 = t18;
  v_r5 = (v_r1 - 8);
  v_r4 = (v_r2 - 16);
  if ((u32)(v_r1 + 7) <= (u32)0x14e)
    goto L_02008936;
  goto L_02008a4a;
L_02008936:
  if (v_r4 > -16)
    goto L_02008940;
  goto L_02008a4a;
L_02008940:
  if (v_r4 <= 239)
    goto L_02008946;
  goto L_02008a4a;
L_02008946:
  *(s32 *)v_r7 = v_r12;
  v_r5 = (v_r5 & 0x1ff);
  *(s32 *)(v_r7 + 4) =
      ((((v_r4 & 255) | (v_r5 << 16)) | ((128 & v_r0) << 21)) | 0x40002000);
  t19 = FIELD_AT(sp_, s32, 4);
  t20 = FIELD_AT(sp_, s32, 8);
  FIELD_AT(v_r7, s32, 8) = (((t19 + t20) + ((v_r0 & 7) << 3)) | 0x800);
  t21 = *(u16 *)(s32)gData_02001de2;
  t22 = *(s16 *)(s32)gData_02001de2;
  v_r2 = t21;
  if (t22 != -1)
    goto L_020089a4;
  t23 = Func_02000d5c(v_r6);
  t24 = FIELD_AT(v_r7, u8, 9);
  v_r2 = -13;
  v_r3 = t24;
  v_r0 = (t23 & 3);
  goto L_02008a22;
L_020089a4:
  t25 = FIELD_AT(v_r7, u8, 9);
  t26 = (3 & v_r2);
  v_r1 = -13;
  v_r2 = t25;
  v_r3 = t26;
  goto L_02008a38;
L_020089b2:
  t27 = *(s16 *)v_r3;
  v_r0 = t27;
  v_r5 = (v_r1 - 8);
  v_r4 = (v_r2 - 64);
  if ((u32)(v_r1 + 23) > (u32)0x15e)
    goto L_02008a4a;
  if (v_r4 <= -0x40)
    goto L_02008a4a;
  if (v_r4 > 175)
    goto L_02008a4a;
  *(s32 *)v_r7 = 0;
  v_r5 = (v_r5 & 0x1ff);
  *(s32 *)(v_r7 + 4) = (((v_r4 & 255) | (v_r5 << 16)) | 0xc000a000);
  t28 = FIELD_AT(sp_, s32, 4);
  FIELD_AT(v_r7, s32, 8) = ((t28 + (v_r0 << 3)) | 0x800);
  t29 = *(u16 *)(s32)gData_02001de2;
  t30 = *(s16 *)(s32)gData_02001de2;
  v_r2 = t29;
  if (t30 != -1)
    goto L_02008a2c;
  t31 = Func_02000d5c(v_r6);
  t32 = FIELD_AT(v_r7, u8, 9);
  v_r2 = -13;
  v_r3 = t32;
  v_r0 = (t31 & 3);
L_02008a22:
  FIELD_AT(v_r7, u8, 9) = ((v_r3 & v_r2) | (v_r0 << 2));
  goto L_02008a40;
L_02008a2c:
  t33 = FIELD_AT(v_r7, u8, 9);
  t34 = (3 & v_r2);
  v_r1 = -13;
  v_r2 = t33;
  v_r3 = t34;
L_02008a38:
  FIELD_AT(v_r7, u8, 9) = ((v_r2 & v_r1) | (v_r3 << 2));
L_02008a40:
  Func_02000c3c(v_r7, v_r11);
  v_r7 = (v_r7 + 12);
L_02008a4a:
  t36 = *(s16 *)(s32)gData_02001cde;
  v_r9 = (v_r9 + 1);
  if (v_r9 >= t36)
    goto L_02008a5a;
  goto L_020088ce;
L_02008a5a:
  return;
}
