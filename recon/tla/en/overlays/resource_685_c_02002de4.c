// Draft, differ 164 of 175: the frame callback of the glow: cycles thirty-two
// colours of the object palettes through a wave of greens and blues, places the
// glow's two sprites at its spot with their scales, and lets each sprite's
// second scale trail its first by a quarter. Translated from its listing; names
// and structure still to do.
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

s32 Math_Sine();
void Func_020035c4();
void Func_02003604();
s32 __divsi3();

s32 Func_02002de4(s32 a0) {
  s32 r11_1, r7_1, r6_1, r8_1, r5_1, r7_2, r5_2, r8_2, r6_2, r10_1, r7_3, r9_1,
      r7_4, r6_3, r5_3;
  s32 r11, r7, r6, r8, v0, r5, v1, r1, r2, r0, r3, r10, r9, v2, v3;
  s32 stack[4];

  /* sub (s32)stack, #16 */
  r11_1 = FIELD_AT(50331868, s32, 0);
  r7_1 = 0;
L_0200ae00:
  r6_1 = 128;
  r6_1 = r6_1 << 5;
  r6_1 = r6_1 + 116;
  r6_1 = r6_1 + r11_1;
  r8_1 = 31;
  v0 = Math_Sine(((FIELD_AT(r6_1, s16, 0)) + r7_1) << 11);
  r5_1 = v0 << 1;
  r5_1 = r5_1 + v0;
  v1 = Math_Sine((((((((FIELD_AT(r6_1, u16, 0)) << 16) >> 16) +
                     ((u32)((FIELD_AT(r6_1, u16, 0)) << 16) >> 31)) >>
                    1) +
                   r7_1)
                  << 11) +
                 18432);
  r5_1 = r5_1 << 1;
  r5_1 = r5_1 >> 16;
  r5_1 = r5_1 + 20;
  r5_1 = r5_1 << 5;
  r1 = r7_1 + 1;
  r2 = ((((v1 << 1) + v1) >> 16) + 3) << 10;
  if (r7_1 <= 15) {
    r2 = (r2 | r5_1) | r8_1;
    r0 = 0x5000200;
    r3 = 111;
  } else {
    r2 = (r2 | r5_1) | r8_1;
    r0 = 0x5000200;
    r3 = 255;
  }
  FIELD_AT(((r3 - r7_1) << 1) + r0, u16, 0) = r2;
  r7_2 = r1;
  if (r7_2 <= 31)
    goto L_0200ae00;
  r5_2 = (s32)((u8 *)stack + 4);
  FIELD_AT(r5_2, s32, 0) = FIELD_AT(r11_1, s32, 4096);
  r8_2 = 4100 + r11_1;
  r6_2 = 128;
  FIELD_AT(r5_2, s32, 4) = FIELD_AT(r11_1, s32, 4100);
  r6_2 = r6_2 << 5;
  r6_2 = r6_2 + 8;
  r6_2 = r6_2 + r11_1;
  FIELD_AT(r5_2, s32, 8) = FIELD_AT(r6_2, s32, 0);
  r10_1 = 4096 + r11_1;
  Func_020035c4(r5_2);
  r7_3 = 128;
  r7_3 = r7_3 << 5;
  r7_3 = r7_3 + 12;
  r7_3 = r7_3 + r11_1;
  FIELD_AT(r7_3, s32, 12) = FIELD_AT(r5_2, s32, 0);
  FIELD_AT(r7_3, s32, 16) = FIELD_AT(r5_2, s32, 8);
  FIELD_AT(r7_3, s32, 20) = FIELD_AT(r11_1, s32, 4196);
  stack[0] = 4200 + r11_1;
  r9_1 = 4196 + r11_1;
  FIELD_AT(r7_3, s32, 24) = FIELD_AT(r11_1, s32, 4200);
  Func_02003604(r7_3, r1, 4200 + r11_1, FIELD_AT(r11_1, s32, 4200),
                4200 + r11_1);
  FIELD_AT(r5_2, s32, 0) = FIELD_AT(r10_1, s32, 0);
  r7_4 = 128;
  FIELD_AT(r5_2, s32, 4) = FIELD_AT(r8_2, s32, 0);
  r7_4 = r7_4 << 5;
  r7_4 = r7_4 + 52;
  FIELD_AT(r5_2, s32, 8) = FIELD_AT(r6_2, s32, 0);
  Func_020035c4(r5_2);
  r7_4 = r7_4 + r11_1;
  FIELD_AT(r7_4, s32, 12) = FIELD_AT(r5_2, s32, 0);
  r6_3 = 128;
  r5_3 = 128;
  FIELD_AT(r7_4, s32, 16) = FIELD_AT(r5_3, s32, 8);
  r5_3 = r5_3 << 5;
  r5_3 = r5_3 + 108;
  r5_3 = r5_3 + r11_1;
  r6_3 = r6_3 << 5;
  FIELD_AT(r7_4, s32, 20) = FIELD_AT(r5_3, s32, 0);
  r6_3 = r6_3 + 112;
  r6_3 = r6_3 + r11_1;
  FIELD_AT(r7_4, s32, 24) = FIELD_AT(r6_3, s32, 0);
  Func_02003604(r7_4);
  v2 = __divsi3(((FIELD_AT(r9_1, s32, 0)) << 5) - (FIELD_AT(r9_1, s32, 0)), 40);
  FIELD_AT(stack[0], s32, 0) = v2;
  v3 = __divsi3(((FIELD_AT(r5_3, s32, 0)) << 5) - (FIELD_AT(r5_3, s32, 0)), 40);
  FIELD_AT(r6_3, s32, 0) = v3;
  /* add (s32)stack, #16 */
  FIELD_AT(r11_1, u16, 4212) = (FIELD_AT(r11_1, u16, 4212)) + 1;
  return v3;
}
