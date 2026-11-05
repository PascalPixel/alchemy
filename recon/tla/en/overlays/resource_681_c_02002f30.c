// Draft of overlay 681's Func_02002f30, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 GameFlag_Test();
s32 Func_02004e04();


void Func_02002f30(void) {
  u8 frame[8];
  s32 v_r0, v_r1, v_r10, v_r2, v_r3, v_r5, v_r6, v_r7, v_r8, v_r9;

  v_r7 = v_r10;
  v_r6 = v_r9;
  v_r5 = v_r8;
  v_r0 = GameFlag_Test(0x9bc);
  v_r5 = v_r0;
  v_r0 = GameFlag_Test(0x9be);
  v_r8 = v_r0;
  v_r2 = v_r8 << 24;
  v_r2 = (u32)v_r2 >> 24;
  v_r8 = v_r2;
  v_r0 = GameFlag_Test(0x9bd);
  v_r6 = v_r0;
  v_r0 = GameFlag_Test(0x9bf);
  v_r5 = v_r5 << 24;
  v_r5 = (u32)v_r5 >> 24;
  v_r5 = v_r5 << 24;
  v_r5 = v_r5 >> 24;
  v_r10 = v_r5;
  v_r3 = v_r8 << 24;
  v_r6 = v_r6 + v_r10;
  v_r7 = v_r3 >> 24;
  v_r6 = v_r6 << 24;
  v_r6 = (u32)v_r6 >> 24;
  v_r0 = v_r7 + v_r0;
  v_r0 = v_r0 << 24;
  v_r6 = v_r6 << 24;
  v_r0 = (u32)v_r0 >> 24;
  v_r9 = v_r3;
  v_r3 = v_r6 >> 24;
  v_r8 = v_r0;
  v_r5 = v_r0 << 24;
  if (v_r3 == 0)
    goto L_0200ae8e;
  if (v_r3 != 2)
    goto L_0200ade8;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02004e04, 74, 115, 7, 12);
  v_r3 = v_r5 >> 24;
  if (v_r3 != 0)
    goto L_0200adbc;
  goto L_0200af22;
L_0200adbc:
  ;
  if (v_r3 != 2)
    goto L_0200adcc;
  /* XXX stack argument */;
  /* XXX stack argument */;
  goto L_0200ae6e;
L_0200adcc:
  ;
  if (v_r7 == 0)
    goto L_0200addc;
  /* XXX stack argument */;
  /* XXX stack argument */;
  goto L_0200ae6e;
L_0200addc:
  ;
  /* XXX stack argument */;
  /* XXX stack argument */;
  goto L_0200ae6e;
L_0200ade8:
  ;
  if (v_r10 == 0)
    goto L_0200ae36;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02004e04, 66, 95, 7, 12);
  v_r3 = v_r5 >> 24;
  if (v_r3 != 0)
    goto L_0200ae0a;
  goto L_0200af22;
L_0200ae0a:
  ;
  if (v_r3 != 2)
    goto L_0200ae1a;
  /* XXX stack argument */;
  /* XXX stack argument */;
  goto L_0200ae6e;
L_0200ae1a:
  ;
  if (v_r7 == 0)
    goto L_0200ae2a;
  /* XXX stack argument */;
  /* XXX stack argument */;
  goto L_0200ae6e;
L_0200ae2a:
  ;
  /* XXX stack argument */;
  /* XXX stack argument */;
  goto L_0200ae6e;
L_0200ae36:
  ;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02004e04, 69, 82, 5, 12);
  v_r3 = v_r5 >> 24;
  if (v_r3 == 0)
    goto L_0200af22;
  if (v_r3 != 2)
    goto L_0200ae60;
  /* XXX stack argument */;
  /* XXX stack argument */;
  goto L_0200ae6e;
L_0200ae60:
  ;
  if (v_r7 == 0)
    goto L_0200ae7a;
  /* XXX stack argument */;
  /* XXX stack argument */;
L_0200ae6e:
  ;
  Call4((void (*)())Func_02004e04, v_r0, 110, 1, 3);
  goto L_0200ae8e;
L_0200ae7a:
  ;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02004e04, 77, 110, 1, 3);
L_0200ae8e:
  ;
  v_r3 = v_r8 << 24;
  v_r0 = v_r3 >> 24;
  if (v_r0 == 0)
    goto L_0200af22;
  if (v_r0 != 2)
    goto L_0200aec2;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02004e04, 83, 115, 6, 12);
  if (v_r6 != 0)
    goto L_0200af22;
  /* XXX stack argument */;
  /* XXX stack argument */;
  goto L_0200aeec;
L_0200aec2:
  ;
  if (v_r9 == 0)
    goto L_0200aef6;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02004e04, 76, 82, 4, 12);
  if (v_r6 != 0)
    goto L_0200af22;
  /* XXX stack argument */;
  /* XXX stack argument */;
L_0200aeec:
  ;
  Call4((void (*)())Func_02004e04, v_r0, v_r1, 1, 3);
  goto L_0200af22;
L_0200aef6:
  ;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02004e04, 82, 82, 6, 12);
  if (v_r6 != 0)
    goto L_0200af22;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02004e04, 81, 91, 1, 3);
L_0200af22:
  ;
  v_r8 = v_r3;
  v_r9 = v_r5;
  v_r10 = v_r6;
}
