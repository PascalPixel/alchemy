// Draft of overlay 671's Func_02004d48, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Actor_GetById();
s32 Func_02005654();
s32 Func_02005624();
s32 Func_02004938();
s32 wait();
s32 GameFlag_SetBit();


void Func_02004d48(s32 p0, s32 p1) {
  u8 frame[8];
  s32 s0, s4, v_r0, v_r10, v_r11, v_r2, v_r3, v_r5, v_r6, v_r7, v_r8, v_r9;

  v_r7 = v_r11;
  v_r6 = v_r10;
  v_r5 = v_r9;
  v_r7 = v_r8;
  v_r11 = p0;
  v_r2 = v_r11 + 1;
  /* XXX stack argument */;
  /* XXX stack argument */;
  v_r0 = Actor_GetById(p0);
  v_r7 = v_r0;
  v_r9 = 0;
  v_r3 = v_r7 + 100;
  FIELD_AT(v_r3, u16, 0) = v_r9;
  v_r10 = 7;
  v_r3 = v_r3 + 2;
  FIELD_AT(v_r3, u16, 0) = v_r10;
  v_r8 = 1;
  v_r3 = v_r3 - 4;
  v_r5 = 2;
  FIELD_AT(v_r3, u8, 0) = v_r5;
  v_r3 = v_r3 + 1;
  FIELD_AT(v_r3, u8, 0) = v_r8;
  FIELD_AT(v_r7, s32, 76) = v_r9;
  v_r0 = Func_02005654();
  Call2((void (*)())Func_02005624, v_r7, 2);
  v_r0 = s0;
  v_r6 = s0;
  v_r0 = Actor_GetById(v_r0);
  v_r7 = v_r0;
  v_r3 = v_r7 + 100;
  FIELD_AT(v_r3, u16, 0) = v_r5;
  v_r3 = v_r3 + 2;
  FIELD_AT(v_r3, u16, 0) = v_r10;
  v_r3 = v_r3 - 4;
  FIELD_AT(v_r3, u8, 0) = 0;
  v_r3 = v_r3 + 1;
  FIELD_AT(v_r3, u8, 0) = v_r8;
  FIELD_AT(v_r7, s32, 76) = v_r9;
  v_r0 = Func_02005654();
  v_r6 = v_r6 + 1;
  Call2((void (*)())Func_02005624, v_r7, 2);
  v_r0 = Actor_GetById(v_r6);
  v_r7 = v_r0;
  v_r2 = v_r7 + 100;
  FIELD_AT(v_r2, u16, 0) = 8;
  v_r3 = v_r7 + 102;
  FIELD_AT(v_r3, u16, 0) = v_r10;
  v_r3 = v_r3 - 4;
  FIELD_AT(v_r3, u8, 0) = 2;
  v_r3 = v_r3 + 1;
  FIELD_AT(v_r3, u8, 0) = v_r8;
  FIELD_AT(v_r7, s32, 76) = v_r9;
  v_r0 = Func_02005654();
  v_r6 = v_r6 + 1;
  Call2((void (*)())Func_02005624, v_r7, 2);
  v_r0 = Actor_GetById(v_r6);
  v_r7 = v_r0;
  v_r2 = v_r7 + 100;
  FIELD_AT(v_r2, u16, 0) = 10;
  v_r3 = v_r7 + 102;
  FIELD_AT(v_r3, u16, 0) = v_r10;
  v_r3 = v_r3 - 4;
  FIELD_AT(v_r3, u8, 0) = 0;
  v_r3 = v_r3 + 1;
  FIELD_AT(v_r3, u8, 0) = v_r8;
  FIELD_AT(v_r7, s32, 76) = v_r9;
  v_r0 = Func_02005654();
  Call2((void (*)())Func_02005624, v_r7, 2);
  v_r10 = 0;
L_0200ce40:
  ;
  v_r9 = 1;
  v_r5 = 0;
L_0200ce46:
  ;
  v_r2 = 4 + v_r11;
  v_r8 = v_r2;
  v_r6 = v_r11;
  if (v_r11 >= v_r8)
    goto L_0200ce7c;
L_0200ce52:
  ;
  v_r0 = Actor_GetById(v_r6);
  v_r7 = v_r0;
  v_r3 = v_r7 + 99;
  v_r3 = FIELD_AT(v_r3, u8, 0);
  if (v_r3 == 0)
    goto L_0200ce6c;
  if (v_r3 == 7)
    goto L_0200ce6c;
  v_r9 = 0;
L_0200ce6c:
  ;
  v_r6 = v_r6 + 1;
  Call4((void (*)())Func_02004938, v_r7, v_r5, v_r6, v_r3);
  if (v_r6 < v_r8)
    goto L_0200ce52;
L_0200ce7c:
  ;
  v_r5 = v_r5 + 1;
  Call1((void (*)())wait, 1);
  if (v_r5 <= 15)
    goto L_0200ce46;
  if (v_r9 != 0)
    goto L_0200ce98;
  v_r10 = v_r10 + 1;
  if (v_r10 <= 127)
    goto L_0200ce40;
L_0200ce98:
  ;
  Call1((void (*)())wait, 20);
  v_r3 = s4;
  if (v_r3 == 0)
    goto L_0200cf32;
  v_r0 = Actor_GetById(v_r11);
  v_r3 = FIELD_AT(v_r0, s32, 8);
  v_r6 = 0;
  v_r5 = 0;
  if (v_r3 == 0)
    goto L_0200ceb8;
  v_r6 = FIELD_AT(v_r0, s32, 76);
L_0200ceb8:
  ;
  v_r0 = s0;
  v_r0 = Actor_GetById(v_r0);
  v_r3 = FIELD_AT(v_r0, s32, 8);
  if (v_r3 == 0)
    goto L_0200ced0;
  v_r0 = FIELD_AT(v_r0, s32, 76);
  if (v_r6 == 0)
    goto L_0200cece;
  if ((u32)v_r6 <= (u32)v_r0)
    goto L_0200ced0;
L_0200cece:
  ;
  v_r6 = v_r0;
L_0200ced0:
  ;
  v_r0 = v_r11 + 2;
  v_r0 = Actor_GetById(v_r0);
  v_r3 = FIELD_AT(v_r0, s32, 8);
  if (v_r3 == 0)
    goto L_0200cee0;
  v_r5 = FIELD_AT(v_r0, s32, 76);
L_0200cee0:
  ;
  v_r0 = v_r11 + 3;
  v_r0 = Actor_GetById(v_r0);
  v_r3 = FIELD_AT(v_r0, s32, 8);
  if (v_r3 == 0)
    goto L_0200cefa;
  v_r0 = FIELD_AT(v_r0, s32, 76);
  if (v_r5 == 0)
    goto L_0200cef8;
  if ((u32)v_r5 <= (u32)v_r0)
    goto L_0200cefa;
L_0200cef8:
  ;
  v_r5 = v_r0;
L_0200cefa:
  ;
  if (v_r6 == 0)
    goto L_0200cf2a;
  if (v_r5 == 0)
    goto L_0200cf32;
  if (v_r6 == v_r5)
    goto L_0200cf32;
  if ((u32)v_r6 >= (u32)v_r5)
    goto L_0200cf1c;
  Call1((void (*)())GameFlag_SetBit, 0x9eb);
  goto L_0200cf32;
L_0200cf1c:
  ;
  Call1((void (*)())GameFlag_SetBit, 0x9eb);
  goto L_0200cf32;
L_0200cf2a:
  ;
  if (v_r5 != 0)
    goto L_0200cf32;
L_0200cf32:
  ;
  v_r8 = v_r3;
  v_r9 = v_r5;
  v_r10 = v_r6;
  v_r11 = v_r7;
}
