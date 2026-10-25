// Draft, differ 466 of 315: translated from its listing; names and structure
// still to do.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 IwramMulQ16[];
extern u8 gFrameCount[];
s32 Object_GetById();
s32 Func_020003d0();
s32 Func_0200034c();
s32 Math_Cosine();
s32 Math_Sine();
s32 Func_02002054();
s32 Func_0200015c();

s32 Func_0200054c(void) {
  s32 r5, v0, r9, r6, r1, r3, v1, r2, r0, r11, v2, v3, v4, v5, r7, r10, r8, v6,
      v7, v8, v9, v10, v11, r4, v12, v13;
  s32 stack[22];

  r5 = (s32)Ram_HeapSlots->eventWork;
  /* sub (s32)stack, #88 */
  v0 = Object_GetById(8);
  r5 = r5 + 340;
  r9 = v0;
  r6 = r9;
  r6 = r6 + 8;
  r1 = (s32)((u8 *)stack + 76);
  r3 = (FIELD_AT(r5, s16, 0)) - 30;
  if ((u32)((FIELD_AT(r5, s16, 0)) - 30) > (u32)7)
    goto L_020085e8;
  switch (r3) {
  case 0:
    goto L_020085a8;
  case 1:
    goto L_020085c6;
  case 2:
    goto L_020085ac;
  case 3:
    goto L_020085ce;
  case 4:
    goto L_020085b6;
  case 5:
    goto L_020085d8;
  case 6:
    goto L_020085be;
  case 7:
    goto L_020085de;
  }
L_020085a8:
  r3 = 0;
  goto L_020085e0;
L_020085ac:
  FIELD_AT(r1, s32, 0) = -1;
  r3 = 0;
  goto L_020085e6;
L_020085b6:
  FIELD_AT(r1, s32, 0) = 0;
  r3 = 1;
  goto L_020085e6;
L_020085be:
  FIELD_AT(r1, s32, 0) = 1;
  r3 = 0;
  goto L_020085e6;
L_020085c6:
  FIELD_AT(r1, s32, 0) = -1;
  r3 = -1;
  goto L_020085e6;
L_020085ce:
  FIELD_AT(r1, s32, 0) = -1;
  r3 = 1;
  goto L_020085e6;
L_020085d8:
  FIELD_AT(r1, s32, 0) = 1;
  r3 = 1;
  goto L_020085e6;
L_020085de:
  r3 = 1;
L_020085e0:
  FIELD_AT(r1, s32, 0) = r3;
  r3 = -1;
L_020085e6:
  FIELD_AT(r1, s32, 8) = r3;
L_020085e8:
  r5 = (s32)((u8 *)stack + 76);
  FIELD_AT(r5, s32, 0) =
      ((((FIELD_AT(r9, s32, 8)) >> 21) + (FIELD_AT(r5, s32, 0))) << 21) +
      1048576;
  FIELD_AT(r5, s32, 8) =
      ((((FIELD_AT(r9, s32, 16)) >> 21) + (FIELD_AT(r5, s32, 8))) << 21) +
      1048576;
  v1 = Func_020003d0(r6, r5);
  if (v1 <= 46)
    goto L_02008648;
  r2 = FIELD_AT(r6, s32, 0);
  if ((FIELD_AT(r6, s32, 0)) >= (FIELD_AT(r5, s32, 0)))
    goto L_02008628;
  r1 = 0x8000;
  goto L_0200862a;
L_02008628:
  r1 = -0x8000;
L_0200862a:
  FIELD_AT(r6, s32, 0) = r2 + r1;
  r0 = FIELD_AT(r9, s32, 16);
  r2 = r9;
  if ((FIELD_AT(r9, s32, 16)) >= (FIELD_AT(r5, s32, 8)))
    goto L_02008640;
  r5 = 128;
  r5 = r5 << 8;
  r3 = r0 + r5;
  goto L_02008646;
L_02008640:
  r3 = r0 - 32768;
  r2 = r9;
L_02008646:
  FIELD_AT(r2, s32, 16) = r3;
L_02008648:
  r11 = (s32)((u8 *)stack + 76);
  v2 = Func_0200034c(1, (FIELD_AT((s32)((u8 *)stack + 76), s32, 0)) >> 20,
                     (FIELD_AT((s32)((u8 *)stack + 76), s32, 8)) >> 20);
  if ((v2 >> 8) != 40)
    goto L_0200866c;
  v3 = Math_Cosine(-0x200);
  stack[4] = v3;
  r0 = -0x200;
  goto L_0200867a;
L_0200866c:
  v4 = Math_Cosine(0x200);
  stack[4] = v4;
  r0 = 0x200;
L_0200867a:
  v5 = Math_Sine(r0);
  r7 = v5;
  r5 = FIELD_AT(r9, s32, 8);
  r5 = r5 - (FIELD_AT(r11, s32, 0));
  r10 = FIELD_AT(r9, s32, 16);
  r8 = (s32)IwramMulQ16;
  r10 = r10 - (FIELD_AT(r11, s32, 8));
  v6 = Iwram_MulQ16(r5, stack[4]);
  r6 = v6;
  v7 = Iwram_MulQ16(r10, r7);
  r6 = r6 - v7;
  v8 = Iwram_MulQ16(r5, r7);
  r5 = v8;
  v9 = Iwram_MulQ16(r10, stack[4]);
  r5 = r5 + v9;
  r6 = r6 + (FIELD_AT(r11, s32, 0));
  r7 = (s32)gFrameCount;
  r5 = r5 + (FIELD_AT(r11, s32, 8));
  FIELD_AT(r9, s32, 16) = r5;
  FIELD_AT(r9, s32, 8) = r6;
  r10 = 3;
  r5 = r10;
  r5 = r5 & (FIELD_AT(r7, s32, 0));
  r10 = r5;
  r1 = r11;
  r2 = FIELD_AT(r7, s32, 0);
  r0 = v9;
  r3 = 3;
  if (r5 == 0)
    goto L_020086e6;
  goto L_020087ee;
L_020086e6:
  r3 = 10 & r2;
  if ((10 & r2) != 0)
    goto L_020086f8;
  Func_02002054(0x255);
L_020086f8:
  FIELD_AT((s32)((u8 *)stack + 20), s32, 0) = 2;
  FIELD_AT((s32)((u8 *)stack + 20), s32, 4) = 7;
  FIELD_AT((s32)((u8 *)stack + 20), s32, 8) = 0xcccc;
  FIELD_AT((s32)((u8 *)stack + 20), s32, 12) = 0xcccc;
  r8 = (s32)((u8 *)stack + 20);
  v10 = Math_Cosine(FIELD_AT(r9, u16, 6));
  r5 = v10;
  v11 = Math_Sine(FIELD_AT(r9, u16, 6));
  r6 = (s32)((u8 *)stack + 60);
  r4 = -(((v11 << 2) + v11) << 1);
  FIELD_AT(r6, s32, 0) = ((r5 << 2) + r5) << 1;
  FIELD_AT(r6, s32, 4) = ((v11 << 2) + v11) << 1;
  FIELD_AT(r6, s32, 8) = -(((r5 << 2) + r5) << 1);
  FIELD_AT(r6, s32, 12) = r4;
  r5 = r9;
  FIELD_AT(r6, s32, 0) = (((r5 << 2) + r5) << 1) + (FIELD_AT(r5, s32, 8));
  FIELD_AT(r6, s32, 4) = (((v11 << 2) + v11) << 1) + (FIELD_AT(r5, s32, 16));
  r11 = 0x8000;
  FIELD_AT(r6, s32, 8) = (-(((r5 << 2) + r5) << 1)) + (FIELD_AT(r5, s32, 8));
  r4 = r4 + (FIELD_AT(r5, s32, 16));
  FIELD_AT(r6, s32, 12) = r4;
  r2 = (((v11 << 2) + v11) << 1) + (FIELD_AT(r5, s32, 16));
  if (((((v11 << 2) + v11) << 1) + (FIELD_AT(r5, s32, 16))) ==
      (FIELD_AT(r5, s32, 16)))
    goto L_0200876a;
  FIELD_AT(r6, s32, 4) = r2 - 786432;
  FIELD_AT(r6, s32, 12) = r4 - 786432;
L_0200876a:
  r7 = FIELD_AT(r7, s32, 0);
  r7 = r7 & 4;
  r1 = FIELD_AT(r9, s32, 12);
  r2 = FIELD_AT(r6, s32, 4);
  r0 = FIELD_AT(r6, s32, 0);
  if (r7 == 0)
    goto L_020087b8;
  r5 = r10;
  stack[0] = r5;
  stack[1] = r5;
  r5 = 176;
  r5 = r5 << 12;
  Func_0200015c(r0 + 196608, r1, r2, r11, r5, r5, r5, r8);
  stack[0] = r10;
  stack[1] = r10;
  stack[2] = r5;
  r5 = r8;
  v12 = Func_0200015c((FIELD_AT(r6, s32, 8)) - 196608, FIELD_AT(r9, s32, 12),
                      FIELD_AT(r6, s32, 12), -0x8000, r10, r10, r5, r5);
  r0 = v12;
  goto L_020087ee;
L_020087b8:
  r5 = 176;
  r5 = r5 << 12;
  Func_0200015c(r0 - 196608, r1, r2, -0x8000, r7, r7, r5, r8);
  v13 = Func_0200015c((FIELD_AT(r6, s32, 8)) + 196608, FIELD_AT(r9, s32, 12),
                      FIELD_AT(r6, s32, 12), r11, r7, r7, r5, r8);
  r0 = v13;
L_020087ee:
  /* add (s32)stack, #88 */
  return r0;
}
