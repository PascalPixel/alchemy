// Draft of overlay 67c's Func_02000cc8, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Actor_GetById();
s32 Func_02003d7c();
s32 Func_02003eac();
s32 Func_02000ca8();
s32 wait();
s32 Func_02003d2c();
extern u8 gMainRoutineWork[];

void Func_02000cc8(s32 p0) {
  u8 frame[12];
  s32 v_r0, v_r1, v_r2, v_r3, v_r5, v_r6, v_sp;

  v_r3 = (s32)gMainRoutineWork + 0x1e0;
  v_r2 = FIELD_AT(v_r3, s16, 0);
  v_r6 = p0;
  v_r5 = 1;
  if (v_r2 != 164)
    goto L_02008d00;
  v_r0 = Actor_GetById(8, 0, v_r2, 164);
  v_r3 = FIELD_AT(v_r0, s32, 8);
  v_r3 = v_r3 >> 20;
  if (v_r6 != 77)
    goto L_02008cf6;
  if (v_r3 != 17)
    goto L_02008cf6;
  v_r5 = 0;
L_02008cf6:
  ;
  if (v_r6 != 78)
    goto L_02008d00;
  if (v_r3 != 19)
    goto L_02008d00;
  v_r5 = 0;
L_02008d00:
  ;
  v_r3 = (s32)gMainRoutineWork + 0x1e0;
  v_r2 = FIELD_AT(v_r3, s16, 0);
  if (v_r2 != 165)
    goto L_02008d3a;
  if (v_r6 != 77)
    goto L_02008d26;
  v_r0 = Actor_GetById(10, 0, v_r2, 165);
  v_r3 = FIELD_AT(v_r0, s32, 8);
  v_r3 = v_r3 >> 20;
  if (v_r3 != 9)
    goto L_02008d26;
  v_r5 = 0;
L_02008d26:
  ;
  if (v_r6 != 78)
    goto L_02008d3a;
  v_r0 = Actor_GetById(11);
  v_r3 = FIELD_AT(v_r0, s32, 8);
  v_r3 = v_r3 >> 20;
  if (v_r3 != 13)
    goto L_02008d3a;
  v_r5 = 0;
L_02008d3a:
  ;
  if (v_r5 == 0)
    goto L_02008dc6;
  v_r3 = (s32)gMainRoutineWork + 0x214;
  v_r0 = FIELD_AT(v_r3, s32, 0);
  v_r0 = Actor_GetById(v_r0);
  v_r5 = v_r0;
  Func_02003d7c();
  v_r0 = Func_02003eac(0);
  goto L_02008da8;
L_02008d5a:
  ;
  if (v_r0 >= 0)
    goto L_02008d6c;
  FIELD_AT(v_r5, u16, 6) = 0x4000;
  Call1((void (*)())Func_02000ca8, v_r6);
  goto L_02008dc6;
L_02008d6c:
  ;
  v_r3 = FIELD_AT(v_r5, s32, 16);
  v_r3 = v_r3 + 0x20000;
  FIELD_AT(v_r5, s32, 16) = v_r3;
  v_r2 = FIELD_AT(v_r5, s32, 8);
  v_r3 = 0xfffff & v_r2;
  v_r3 = v_r3 + -0x80000;
  if (v_r3 <= 0x10000)
    goto L_02008d8c;
L_02008d8c:
  ;
  if (v_r3 >= -0x10000)
    goto L_02008d94;
L_02008d94:
  ;
  v_r3 = v_r2 - v_r3;
  FIELD_AT(v_r5, s32, 8) = v_r3;
  v_r3 = FIELD_AT(v_r5, u16, 6);
  v_r3 = v_r3 + 0x1000;
  FIELD_AT(v_r5, u16, 6) = v_r3;
  v_r0 = wait(1, v_r1, 0x1000, v_r3);
L_02008da8:
  ;
  v_r3 = FIELD_AT(v_r5, s32, 8);
  FIELD_AT(v_sp, s32, 0) = v_r3;
  v_r3 = FIELD_AT(v_r5, s32, 12);
  FIELD_AT(v_sp, s32, 4) = v_r3;
  v_r3 = FIELD_AT(v_r5, s32, 16);
  v_r3 = v_r3 + 0x80000;
  FIELD_AT(v_sp, s32, 8) = v_r3;
  v_r0 = Func_02003d2c(v_r5, v_sp, 0x80000, v_r3);
  if (v_r0 <= 0)
    goto L_02008d5a;
L_02008dc6:
  ;
}
