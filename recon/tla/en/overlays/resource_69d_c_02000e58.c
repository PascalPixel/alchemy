// Draft of overlay 69d's Func_02000e58, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02001ee0();
s32 Func_02001d80();
extern u8 gData_0200275c[];
extern u8 gResourceTableEntries[];
extern u8 gData_020027a0[];
extern u8 gData_0200275e[];
extern u8 gData_02002760[];
extern u8 gData_02002860[];
extern u8 gData_02002862[];

void Func_02000e58(void) {
  u8 frame[8];
  s32 s0, s4, v_ip, v_r0, v_r1, v_r10, v_r11, v_r2, v_r3, v_r4, v_r5, v_r6, v_r7, v_r8, v_r9;

  v_r7 = v_r11;
  v_r6 = v_r10;
  v_r5 = v_r9;
  v_r7 = v_r8;
  v_r1 = FIELD_AT(0x3000000, s32, 32);
  v_r2 = v_r1 + 228;
  v_r0 = FIELD_AT(v_r2, s32, 0);
  v_r2 = FIELD_AT(v_r2, s32, 4);
  v_r0 = v_r0 & -0x10000;
  v_r2 = v_r2 & -0x10000;
  v_r10 = v_r0;
  v_r3 = FIELD_AT((s32)gData_0200275c, s16, 0);
  v_r8 = v_r2;
  v_r3 = v_r3 << 2;
  v_r3 = v_r3 + (s32)gResourceTableEntries;
  v_r3 = FIELD_AT(v_r3, u16, 2);
  v_r3 = (u32)v_r3 >> 5;
  /* XXX stack argument */;
  v_r6 = (s32)gData_020027a0;
  v_r3 = FIELD_AT(v_r1, s32, 0);
  v_r3 = FIELD_AT(v_r3, s32, 4);
  v_r9 = 0;
  /* XXX stack argument */;
  v_r3 = FIELD_AT((s32)gData_0200275e, s16, 0);
  if (v_r9 < v_r3)
    goto L_02008eaa;
  goto L_02008fde;
L_02008eaa:
  ;
  v_r3 = v_r9 << 2;
  v_r5 = FIELD_AT((s32)gData_02002760 + v_r3, s32, 0);
  if (v_r5 != 0)
    goto L_02008eb8;
  goto L_02008fce;
L_02008eb8:
  ;
  v_r3 = FIELD_AT(v_r5, s32, 8);
  if (v_r3 != 0)
    goto L_02008ec0;
  goto L_02008fce;
L_02008ec0:
  ;
  v_r0 = v_r3 - v_r10;
  v_r2 = s0;
  v_r3 = FIELD_AT(v_r5, s32, 12);
  v_r3 = v_r3 - v_r2;
  v_r2 = FIELD_AT(v_r5, s32, 16);
  v_r3 = v_r3 + 0x80000;
  v_r2 = v_r2 - v_r8;
  v_r1 = s0;
  v_r2 = v_r2 - v_r1;
  v_r4 = v_r2 - v_r3;
  v_r3 = v_r3 + v_r2;
  v_r3 = v_r3 >> 16;
  v_r3 = v_r3 + 58;
  v_r11 = v_r3;
  v_r2 = FIELD_AT((s32)gData_02002860, s16, 0);
  v_ip = v_r2;
  v_r1 = v_r0 >> 16;
  v_r3 = v_r5 + 100;
  v_r2 = v_r4 >> 16;
  if (v_ip != 0)
    goto L_02008f36;
  v_r7 = FIELD_AT(v_r3, s16, 0);
  v_r3 = v_r1 + 7;
  v_r4 = v_r2;
  v_r0 = v_r1 - 8;
  v_r4 = v_r4 - 16;
  if ((u32)v_r3 > (u32)0x14e)
    goto L_02008fce;
  if (v_r4 <= -16)
    goto L_02008fce;
  if (v_r4 > 239)
    goto L_02008fce;
  v_r0 = v_r0 & 0x1ff;
  v_r4 = v_r4 & 255;
  /* XXX stmia r1!, {r3} */
  v_r3 = v_r0 << 16;
  v_r4 = v_r4 | v_r3;
  goto L_02008f72;
L_02008f36:
  ;
  v_r7 = FIELD_AT(v_r3, s16, 0);
  v_r4 = v_r2;
  v_r3 = v_r6 + 23;
  v_r0 = v_r6 - 8;
  v_r4 = v_r4 - 64;
  if ((u32)v_r3 > (u32)0x15e)
    goto L_02008fce;
  if (v_r4 <= -64)
    goto L_02008fce;
  if (v_r4 > 175)
    goto L_02008fce;
  v_r0 = v_r0 & 0x1ff;
  v_r4 = v_r4 & 255;
  /* XXX stmia r1!, {r3} */
  v_r3 = v_r0 << 16;
  v_r4 = v_r4 | v_r3;
L_02008f72:
  ;
  v_r4 = v_r4 | v_r3;
  /* XXX stmia r1!, {r4} */
  v_r0 = s4;
  v_r3 = v_r7 << 3;
  v_r3 = v_r0 + v_r3;
  v_r3 = v_r3 | 0x800;
  FIELD_AT(v_r6, s32, 0) = v_r3;
  v_r2 = FIELD_AT((s32)gData_02002862, u16, 0);
  v_r3 = FIELD_AT((s32)gData_02002862, s16, 0);
  if (v_r3 != -1)
    goto L_02008fb0;
  v_r0 = Func_02001ee0(v_r5, 0, v_r2, v_r3);
  v_r0 = v_r0 & 3;
  v_r3 = FIELD_AT(v_r6, u8, 9);
  v_r0 = v_r0 << 2;
  v_r3 = v_r3 & -13;
  v_r3 = v_r3 | v_r0;
  FIELD_AT(v_r6, u8, 9) = v_r3;
  goto L_02008fc4;
L_02008fb0:
  ;
  v_r3 = 3 & v_r2;
  v_r2 = FIELD_AT(v_r6, u8, 9);
  v_r3 = v_r3 << 2;
  v_r2 = v_r2 & -13;
  v_r2 = v_r2 | v_r3;
  FIELD_AT(v_r6, u8, 9) = v_r2;
L_02008fc4:
  ;
  Call4((void (*)())Func_02001d80, v_r6, v_r11, v_r2, v_r3);
  v_r6 = v_r6 + 12;
L_02008fce:
  ;
  v_r3 = FIELD_AT((s32)gData_0200275e, s16, 0);
  v_r9 = v_r9 + 1;
  if (v_r9 >= v_r3)
    goto L_02008fde;
  goto L_02008eaa;
L_02008fde:
  ;
  v_r8 = v_r3;
  v_r9 = v_r5;
  v_r10 = v_r6;
  v_r11 = v_r7;
}
