// Draft, differ 359 of 280: translated from its listing; names and structure
// still to do.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gFrameCount[];
void *Object_GetById(s32 id);
s32 Func_02001fac();
s32 Func_02002044();
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
s32 Func_0200034c();
s32 Func_02001f6c();
s32 Func_02002054();
s32 Func_0200015c();
void WaitFrames(s32 frames);
s32 Func_02001f9c();
s32 Func_02001fb4();

s32 Func_02000818(void) {
  s32 v0, r8, r6, r5, v1, r11, v2, r9, r1, r3, v3, r2, r7, r10, v4, v5, r4, r0,
      v6, v7, v8;
  s32 stack[24];

  /* sub sp, #96 */
  stack[9] = (s32)Ram_HeapSlots->eventWork;
  v0 = Object_GetById(8);
  r8 = v0;
  r6 = stack[9] + 340;
  r5 = FIELD_AT(r6, u16, 0);
  Func_02001fac();
  Func_02002044(0);
  r5 = r5 - 45;
  stack[8] = (FIELD_AT(r8, s32, 8)) >> 20;
  r5 = r5 << 30;
  r5 = r5 >> 16;
  stack[7] = (FIELD_AT(r8, s32, 16)) >> 20;
  v1 = Math_Cosine(r5);
  r11 = v1 >> 16;
  v2 = Math_Sine(r5);
  r9 = v2 >> 16;
  r5 = r8;
  FIELD_AT(r5, s32, 48) = 0x40000;
  FIELD_AT(r5, s32, 52) = 0x40000;
  stack[5] = 0;
  stack[6] = 0;
  r1 = r9;
  if (r9 >= 0)
    goto L_0200888c;
  r1 = -r1;
L_0200888c:
  r1 = stack[8] + r1;
  r3 = r11;
  if (r11 >= 0)
    goto L_02008898;
  r3 = -r3;
L_02008898:
  r5 = stack[7];
  v3 = Func_0200034c(1, r1, r5 + r3);
  if ((v3 >> 8) != (FIELD_AT(r6, s16, 0)))
    goto L_020088cc;
  r2 = r9;
  if (r9 >= 0)
    goto L_020088b4;
  r2 = -r2;
L_020088b4:
  stack[6] = ((r2 << 4) - r2) << 16;
  r2 = r11;
  if (r11 >= 0)
    goto L_020088c4;
  r2 = -r2;
L_020088c4:
  stack[5] = ((r2 << 4) - r2) << 16;
L_020088cc:
  r5 = stack[6];
  r5 = r5 + (stack[8] << 20);
  stack[6] = r5;
  stack[5] = stack[5] + (stack[7] << 20);
  r3 = stack[7] << 20;
  goto L_02008a3c;
L_020088e2:
  stack[8] = stack[8] + r11;
  r5 = r11;
  stack[7] = stack[7] + r9;
  stack[6] = stack[6] + (r5 << 20);
  stack[5] = stack[5] + (r9 << 20);
  Func_02001f6c(r8, stack[6], FIELD_AT(r8, s32, 12), stack[5]);
  goto L_02008a30;
L_02008912:
  r7 = (s32)gFrameCount;
  r5 = 3;
  r5 = r5 & (FIELD_AT(r7, s32, 0));
  stack[4] = r5;
  r2 = FIELD_AT(r7, s32, 0);
  if (r5 == 0)
    goto L_02008922;
  goto L_02008a2a;
L_02008922:
  r3 = 10 & r2;
  if ((10 & r2) != 0)
    goto L_02008934;
  Func_02002054(0x255);
L_02008934:
  FIELD_AT((s32)((u8 *)stack + 40), s32, 0) = 2;
  FIELD_AT((s32)((u8 *)stack + 40), s32, 4) = 7;
  FIELD_AT((s32)((u8 *)stack + 40), s32, 8) = 0x16666;
  FIELD_AT((s32)((u8 *)stack + 40), s32, 12) = 0x13333;
  r10 = (s32)((u8 *)stack + 40);
  v4 = Math_Cosine(FIELD_AT(r8, u16, 6));
  r5 = v4;
  v5 = Math_Sine(FIELD_AT(r8, u16, 6));
  r6 = (s32)((u8 *)stack + 80);
  r4 = -(((v5 << 1) + v5) << 2);
  FIELD_AT(r6, s32, 0) = ((r5 << 1) + r5) << 2;
  FIELD_AT(r6, s32, 4) = ((v5 << 1) + v5) << 2;
  FIELD_AT(r6, s32, 8) = -(((r5 << 1) + r5) << 2);
  FIELD_AT(r6, s32, 12) = r4;
  r5 = r8;
  FIELD_AT(r6, s32, 0) = (((r5 << 1) + r5) << 2) + (FIELD_AT(r5, s32, 8));
  FIELD_AT(r6, s32, 4) = (((v5 << 1) + v5) << 2) + (FIELD_AT(r5, s32, 16));
  FIELD_AT(r6, s32, 8) = (-(((r5 << 1) + r5) << 2)) + (FIELD_AT(r5, s32, 8));
  r4 = r4 + (FIELD_AT(r5, s32, 16));
  FIELD_AT(r6, s32, 12) = r4;
  r2 = (((v5 << 1) + v5) << 2) + (FIELD_AT(r5, s32, 16));
  if (((((v5 << 1) + v5) << 2) + (FIELD_AT(r5, s32, 16))) ==
      (FIELD_AT(r5, s32, 16)))
    goto L_020089a2;
  FIELD_AT(r6, s32, 4) = r2 - 786432;
  FIELD_AT(r6, s32, 12) = r4 - 786432;
L_020089a2:
  r7 = FIELD_AT(r7, s32, 0);
  r7 = r7 & 4;
  r1 = FIELD_AT(r8, s32, 12);
  r2 = FIELD_AT(r6, s32, 4);
  r0 = FIELD_AT(r6, s32, 0);
  if (r7 == 0)
    goto L_020089f2;
  r5 = stack[4];
  stack[0] = r5;
  stack[1] = r5;
  stack[3] = r10;
  r5 = 176;
  r5 = r5 << 12;
  Func_0200015c(r0 + 262144, r1, r2, 0x8000, r5, r5, r5, r10);
  stack[0] = stack[4];
  stack[1] = stack[4];
  stack[2] = r5;
  r5 = r10;
  Func_0200015c((FIELD_AT(r6, s32, 8)) - 262144, FIELD_AT(r8, s32, 12),
                FIELD_AT(r6, s32, 12), -0x8000, stack[4], stack[4], r5, r5);
  goto L_02008a2a;
L_020089f2:
  r5 = 176;
  r5 = r5 << 12;
  Func_0200015c(r0 - 262144, r1, r2, -0x8000, r7, r7, r5, r10);
  Func_0200015c((FIELD_AT(r6, s32, 8)) + 262144, FIELD_AT(r8, s32, 12),
                FIELD_AT(r6, s32, 12), 0x8000, r7, r7, r5, r10);
L_02008a2a:
  WaitFrames(1);
L_02008a30:
  v6 = Func_02001f9c(r8);
  if (v6 != 0)
    goto L_02008a3c;
  goto L_02008912;
L_02008a3c:
  v7 = Func_0200034c(1, stack[8], stack[7]);
  r5 = stack[9];
  r0 = v7 >> 8;
  r1 = 0x154;
  r2 = 0;
  r3 = FIELD_AT(r5, s16, 340);
  if ((v7 >> 8) != (FIELD_AT(r5, s16, 340)))
    goto L_02008a5a;
  goto L_020088e2;
L_02008a5a:
  v8 = Func_02001fb4();
  /* add sp, #96 */
  return v8;
}
