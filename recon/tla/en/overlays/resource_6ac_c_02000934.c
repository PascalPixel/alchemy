// Draft of overlay 6ac's Func_02000934, translated from its listing: differ
// 149 of 159. Left: its shape: 164 instructions against the listing's 159.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02003158();
s32 Func_02003178();
s32 Scheduler_AddOrUpdateCallback();
extern u8 Data_02003a7c[];
extern u8 Func_0200055c[];
extern u8 gData_02003a7e[];
extern u8 gData_02003a80[];
extern u8 gData_02003a82[];
extern u8 gIoWriteQueue[];

void Func_02000934(s32 p0) {
  s32 v_r5, v_r6, v_r1, v_r0, v_r4, v_r3, v_r2, t2, t4, t6;
  s32 t8, t9, t10, t11, t12;

  Func_02003158();
  v_r1 = (s32)Data_02003a7c;
  v_r6 = -1;
  v_r5 = p0;
  if (v_r5 != 8)
    goto L_0200894a;
  v_r6 = 20;
L_0200894a:
  if (v_r5 != 10)
    goto L_02008950;
  v_r6 = 22;
L_02008950:
  if (v_r5 != 12)
    goto L_02008956;
  v_r6 = 24;
L_02008956:
  if (v_r5 != 14)
    goto L_0200895c;
  v_r6 = 26;
L_0200895c:
  if (v_r6 != -1)
    goto L_0200898c;
  *(u16 *)v_r1 = 0;
  *(u16 *)(s32)gData_02003a7e = 0;
  *(u16 *)(s32)gData_02003a80 = v_r6;
  *(u16 *)(s32)gData_02003a82 = v_r6;
  goto L_02008a2c;
L_0200898c:
  *(u16 *)v_r1 = 15;
  *(u16 *)(s32)gData_02003a7e = 16;
  *(u16 *)(s32)gData_02003a80 = (((v_r6 - 20) + ((u32)(v_r6 - 20) >> 31)) >> 1);
  *(u16 *)(s32)gData_02003a82 = v_r6;
  v_r5 = (s32)gData_02003a80;
  if ((((v_r6 - 20) + ((u32)(v_r6 - 20) >> 31)) >> 1) != 0)
    goto L_020089d8;
  Call6((void (*)())Func_02003178, 0, 125, 3, 3, 17, 67);
  goto L_020089d8;
L_020089d8:
  t2 = *(s16 *)v_r5;
  if (t2 != 1)
    goto L_020089f4;
  Call6((void (*)())Func_02003178, 0, 125, 3, 3, 8, 72);
L_020089f4:
  t4 = *(s16 *)v_r5;
  if (t4 != 2)
    goto L_02008a10;
  Call6((void (*)())Func_02003178, 0, 125, 3, 3, 17, 72);
L_02008a10:
  t6 = *(s16 *)v_r5;
  if (t6 != 3)
    goto L_02008a2c;
  Call6((void (*)())Func_02003178, 0, 125, 3, 3, 8, 67);
L_02008a2c:
  t8 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t9 = *(u16 *)(s32)gIoWriteQueue;
  v_r3 = t9;
  v_r4 = (s32)gIoWriteQueue;
  v_r0 = 0x4000208;
  v_r5 = t8;
  if (v_r3 > 31)
    goto L_02008a68;
  *(u16 *)v_r4 = (v_r3 + 1);
  t10 = *(s16 *)(s32)Data_02003a7c;
  *(s32 *)(((((v_r3 << 1) + v_r3) << 2) + v_r4) + 4) =
      (t10 | ((16 - t10) << 8));
  *(s32 *)(((((v_r3 << 1) + v_r3) << 2) + v_r4) + 8) = 0x4000052;
  FIELD_AT(((((v_r3 << 1) + v_r3) << 2) + v_r4), s32, 12) = 0x20000;
L_02008a68:
  *(u16 *)v_r0 = v_r5;
  t11 = *(u16 *)v_r0;
  *(u16 *)v_r0 = v_r0;
  t12 = *(u16 *)v_r4;
  v_r2 = t12;
  v_r1 = t11;
  if (v_r2 > 31)
    goto L_02008a9a;
  *(u16 *)v_r4 = (v_r2 + 1);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r4) + 4) = 0x3f42;
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r4) + 8) = 0x4000050;
  FIELD_AT(((((v_r2 << 1) + v_r2) << 2) + v_r4), s32, 12) = 0x20000;
L_02008a9a:
  *(u16 *)v_r0 = v_r1;
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_0200055c, 0x480);
  return;
}
