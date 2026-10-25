// Draft, differ 210 of 165: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gFrameCount[];
s32 Object_GetById();
void Func_02001fac();
void Func_02002044();
void Func_02001f6c();
s32 Math_Cosine();
s32 Math_Sine();
void Func_0200015c();
void WaitFrames();
s32 Func_02001f9c();
s32 Func_02001fb4();

s32 Func_02000ef0(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r8_1, r5_1, r7_1, r10_1, r9_1, r5_2, r6_1, r5_3, r11_1, r5_4, r5_5, r5_6,
      r5_7;
  s32 v0, r8, r5, r7, r10, r9, v1, v2, r6, r4, r11, r2, r1, r0, v3, v4, r3;
  s32 stack[18];

  /* sub (s32)stack, #72 */
  v0 = Object_GetById(8);
  r8_1 = v0;
  Func_02001fac();
  Call1((void (*)())Func_02002044, 0);
  r5_1 = 128;
  r5_1 = r5_1 << 12;
  Func_02001f6c(r8_1, FIELD_AT(r8_1, s32, 8), FIELD_AT(r8_1, s32, 12),
                (FIELD_AT(r8_1, s32, 16)) + r5_1);
  goto L_02009036;
L_02008f26:
  r7_1 = (s32)gFrameCount;
  r10_1 = FIELD_AT(r7_1, s32, 0);
  r10_1 = r10_1 & 3;
  if ((r10_1 & 3) != 0)
    goto L_02009030;
  FIELD_AT((s32)((u8 *)stack + 16), s32, 0) = 2;
  FIELD_AT((s32)((u8 *)stack + 16), s32, 4) = 7;
  FIELD_AT((s32)((u8 *)stack + 16), s32, 8) = 0x16666;
  r9_1 = (s32)((u8 *)stack + 16);
  FIELD_AT((s32)((u8 *)stack + 16), s32, 12) = 0x13333;
  v1 = Math_Cosine(FIELD_AT(r8_1, u16, 6));
  r5_2 = v1;
  v2 = Math_Sine(FIELD_AT(r8_1, u16, 6));
  r6_1 = (s32)((u8 *)stack + 56);
  r4 = -(((v2 << 1) + v2) << 2);
  FIELD_AT(r6_1, s32, 0) = ((r5_2 << 1) + r5_2) << 2;
  FIELD_AT(r6_1, s32, 4) = ((v2 << 1) + v2) << 2;
  FIELD_AT(r6_1, s32, 8) = -(((r5_2 << 1) + r5_2) << 2);
  FIELD_AT(r6_1, s32, 12) = r4;
  r5_3 = r8_1;
  FIELD_AT(r6_1, s32, 0) =
      (((r5_3 << 1) + r5_3) << 2) + (FIELD_AT(r5_3, s32, 8));
  FIELD_AT(r6_1, s32, 4) = (((v2 << 1) + v2) << 2) + (FIELD_AT(r5_3, s32, 16));
  FIELD_AT(r6_1, s32, 8) =
      (-(((r5_3 << 1) + r5_3) << 2)) + (FIELD_AT(r5_3, s32, 8));
  r4 = r4 + (FIELD_AT(r5_3, s32, 16));
  FIELD_AT(r6_1, s32, 12) = r4;
  r11_1 = 0x8000;
  r2 = (((v2 << 1) + v2) << 2) + (FIELD_AT(r5_3, s32, 16));
  if (((((v2 << 1) + v2) << 2) + (FIELD_AT(r5_3, s32, 16))) ==
      (FIELD_AT(r5_3, s32, 16)))
    goto L_02008fac;
  FIELD_AT(r6_1, s32, 4) = r2 - 786432;
  FIELD_AT(r6_1, s32, 12) = r4 - 786432;
L_02008fac:
  r7_1 = FIELD_AT(r7_1, s32, 0);
  r7_1 = r7_1 & 4;
  r1 = FIELD_AT(r8_1, s32, 12);
  r2 = FIELD_AT(r6_1, s32, 4);
  r0 = FIELD_AT(r6_1, s32, 0);
  if (r7_1 == 0)
    goto L_02008ffa;
  r5_4 = r10_1;
  stack[0] = r5_4;
  stack[1] = r5_4;
  r5_5 = 176;
  r5_5 = r5_5 << 12;
  Func_0200015c(r0 + 262144, r1, r2, r11_1, r5_5, r5_5, r5_5, r9_1);
  stack[0] = r10_1;
  stack[1] = r10_1;
  stack[2] = r5_5;
  r5_6 = r9_1;
  Func_0200015c((FIELD_AT(r6_1, s32, 8)) - 262144, FIELD_AT(r8_1, s32, 12),
                FIELD_AT(r6_1, s32, 12), -0x8000, r10_1, r10_1, r5_6, r5_6);
  goto L_02009030;
L_02008ffa:
  r5_7 = 176;
  r5_7 = r5_7 << 12;
  Func_0200015c(r0 - 262144, r1, r2, -0x8000, r7_1, r7_1, r5_7, r9_1);
  Func_0200015c((FIELD_AT(r6_1, s32, 8)) + 262144, FIELD_AT(r8_1, s32, 12),
                FIELD_AT(r6_1, s32, 12), r11_1, r7_1, r7_1, r5_7, r9_1);
L_02009030:
  Call1((void (*)())WaitFrames, 1);
L_02009036:
  v3 = Func_02001f9c(r8_1);
  r0 = v3;
  if (v3 != 0)
    goto L_02009042;
  goto L_02008f26;
L_02009042:
  v4 = Func_02001fb4(r0, r1, r2, r3);
  /* add (s32)stack, #72 */
  return v4;
}
