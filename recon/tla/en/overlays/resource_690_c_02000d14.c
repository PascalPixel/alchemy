// Draft of overlay 690's Func_02000d14, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gData_02002c5c[];
extern u8 gData_02002c60[];
extern u8 IwramCopyWords[];
extern u8 gData_02002c80[];
extern u8 gIoWriteQueue[];

void Func_02000d14(void) {
  s32 v_r0, v_r1, v_r2, v_r3, v_r4, v_r5, v_r6, v_r7, v_r8;

  v_r7 = v_r8;
  v_r2 = FIELD_AT(0x3000000, s32, 108);
  v_r3 = v_r2 + 0x166;
  v_r3 = FIELD_AT(v_r3, s16, 0);
  if (v_r3 != 0)
    goto L_02008de4;
  v_r3 = v_r2 + 0xca4;
  v_r6 = 0;
  v_r6 = FIELD_AT(v_r3 + v_r6, s8, 0);
  if (v_r6 != 0)
    goto L_02008de4;
  v_r7 = (s32)gData_02002c5c;
  v_r3 = FIELD_AT(v_r7, s32, 0);
  if (v_r3 != -1)
    goto L_02008d6a;
  v_r5 = (s32)gData_02002c60;
  v_r8 = (s32)IwramCopyWords;
  FIELD_AT(v_r7, s32, 0) = v_r6;
L_02008d6a:
  ;
  v_r3 = FIELD_AT(v_r7, s32, 0);
  if (v_r3 >= 0)
    goto L_02008d72;
  v_r3 = v_r3 + 7;
L_02008d72:
  ;
  v_r2 = v_r3 >> 3;
  if (v_r2 >= 0)
    goto L_02008d7c;
  v_r3 = v_r2 + 3;
L_02008d7c:
  ;
  v_r1 = v_r3 >> 2;
  v_r6 = (s32)gData_02002c80;
  v_r5 = (s32)gData_02002c60;
  v_r3 = v_r1 << 2;
  v_r1 = v_r2 - v_r3;
L_02008d8a:
  ;
  v_r2 = v_r1 + v_r0;
  if (v_r2 >= 0)
    goto L_02008d94;
  v_r3 = v_r2 + 3;
L_02008d94:
  ;
  v_r3 = v_r3 >> 2;
  v_r3 = v_r3 << 2;
  v_r3 = v_r2 - v_r3;
  v_r2 = FIELD_AT(v_r5 + v_r4, u16, 0);
  v_r3 = v_r3 << 1;
  v_r3 = v_r3 + 24;
  v_r0 = v_r0 + 1;
  FIELD_AT(v_r6 + v_r3, u16, 0) = v_r2;
  v_r4 = v_r4 + 2;
  if (v_r0 <= 3)
    goto L_02008d8a;
  v_r3 = FIELD_AT(0x4000208, u16, 0);
  v_r4 = v_r3;
  FIELD_AT(0x4000208, u16, 0) = 0x4000208;
  v_r2 = FIELD_AT((s32)gIoWriteQueue, u16, 0);
  if (v_r2 > 31)
    goto L_02008dda;
  v_r3 = v_r2 << 1;
  v_r3 = v_r3 + v_r2;
  v_r3 = v_r3 << 2;
  v_r3 = v_r3 + (s32)gIoWriteQueue;
  v_r3 = v_r3 + 4;
  v_r2 = v_r2 + 1;
  /* XXX stmia r3!, {r6} */
  FIELD_AT((s32)gIoWriteQueue, u16, 0) = v_r2;
  /* XXX stmia r3!, {r2} */
  FIELD_AT(v_r3, s32, 0) = -0x7bfffff8;
L_02008dda:
  ;
  FIELD_AT(v_r0, u16, 0) = v_r4;
  v_r3 = FIELD_AT((s32)gData_02002c5c, s32, 0);
  v_r3 = v_r3 + 1;
  FIELD_AT((s32)gData_02002c5c, s32, 0) = v_r3;
L_02008de4:
  ;
  v_r8 = v_r3;
}
