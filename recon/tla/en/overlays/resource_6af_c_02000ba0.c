// Draft of overlay 6af's Func_02000ba0, translated from its listing: differ
// 219 of 152. Left: its shape: 131 instructions against the listing's 152.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
extern u8 Data_02001aa8[];
extern u8 Data_02001ae8[];
extern u8 Data_02001b08[];
extern u8 IwramCopyWords[];
extern u8 gData_02001aa4[];
extern u8 gData_02001ac8[];
extern u8 gIoWriteQueue[];

void Func_02000ba0(void) {
  s32 v_r2, v_r8, v_r7, v_r3, v_r1, v_r4, v_r6, v_r0, v_r12, t0;
  s32 t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
  s32 t11, t12;

  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = *(s16 *)(t0 + 0x166);
  v_r2 = t0;
  if (t1 == 0)
    goto L_02008bbc;
  goto L_02008cca;
L_02008bbc:
  t2 = *(u8 *)(v_r2 + 0xca4);
  v_r8 = ((t2 << 24) >> 24);
  if (v_r8 == 0)
    goto L_02008bd2;
  goto L_02008cca;
L_02008bd2:
  t3 = *(s32 *)(s32)gData_02001aa4;
  v_r7 = (s32)gData_02001aa4;
  if (t3 != -1)
    goto L_02008c14;
  *(s32 *)v_r7 = v_r8;
L_02008c14:
  t4 = *(s32 *)v_r7;
  v_r3 = t4;
  if (v_r3 >= 0)
    goto L_02008c1c;
  v_r3 = (v_r3 + 7);
L_02008c1c:
  t5 = (v_r3 >> 3);
  v_r3 = (v_r3 >> 3);
  v_r2 = t5;
  if (v_r2 >= 0)
    goto L_02008c26;
  v_r3 = (v_r2 + 3);
L_02008c26:
  v_r12 = (s32)Data_02001b08;
  v_r8 = (s32)Data_02001ae8;
  v_r7 = (s32)gData_02001ac8;
  v_r0 = 4;
  v_r6 = (s32)Data_02001aa8;
  v_r4 = 0;
  v_r1 = (v_r2 - ((v_r3 >> 2) << 2));
L_02008c3c:
  v_r2 = (v_r1 + v_r4);
  v_r3 = (v_r1 + v_r4);
  if (v_r2 >= 0)
    goto L_02008c46;
  v_r3 = (v_r2 + 3);
L_02008c46:
  t6 = *(u16 *)(v_r6 + v_r0);
  *(u16 *)(v_r7 + (((v_r2 - ((v_r3 >> 2) << 2)) << 1) + 4)) = t6;
  t7 = *(u16 *)(v_r8 + v_r0);
  *(u16 *)(v_r12 + (((v_r2 - ((v_r3 >> 2) << 2)) << 1) + 4)) = t7;
  v_r0 = (v_r0 + 2);
  v_r4 = (v_r4 + 1);
  if (v_r4 <= 3)
    goto L_02008c3c;
  t8 = *(u16 *)0x4000208;
  *(u16 *)0x4000208 = 0x4000208;
  t9 = *(u16 *)(s32)gIoWriteQueue;
  v_r2 = t9;
  v_r0 = (s32)gIoWriteQueue;
  v_r1 = 0x4000208;
  v_r4 = t8;
  if (v_r2 > 31)
    goto L_02008c94;
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 4) = v_r7;
  *(u16 *)v_r0 = (v_r2 + 1);
  *(s32 *)((((((v_r2 << 1) + v_r2) << 2) + v_r0) + 4) + 4) = 0x5000060;
  *(s32 *)(((((((v_r2 << 1) + v_r2) << 2) + v_r0) + 4) + 4) + 4) = 0x84000008;
L_02008c94:
  *(u16 *)v_r1 = v_r4;
  t10 = *(u16 *)v_r1;
  *(u16 *)v_r1 = v_r1;
  t11 = *(u16 *)v_r0;
  v_r2 = t11;
  v_r4 = t10;
  if (v_r2 > 31)
    goto L_02008cc0;
  *(u16 *)v_r0 = (v_r2 + 1);
  *(s32 *)(((((v_r2 << 1) + v_r2) << 2) + v_r0) + 4) = v_r12;
  *(s32 *)((((((v_r2 << 1) + v_r2) << 2) + v_r0) + 4) + 4) = 0x5000140;
  *(s32 *)(((((((v_r2 << 1) + v_r2) << 2) + v_r0) + 4) + 4) + 4) = 0x84000008;
L_02008cc0:
  *(u16 *)v_r1 = v_r4;
  t12 = *(s32 *)(s32)gData_02001aa4;
  *(s32 *)(s32)gData_02001aa4 = (t12 + 1);
L_02008cca:
  return;
}
