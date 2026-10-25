// Draft, differ 305 of 304: translated from its listing; names and structure still
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

extern u8 IwramMulQ16[];
extern u8 gInput[];
extern u8 gData_020032b2[];
extern u8 Func_020017dc[];
void Func_02001868();
s32 Func_02002fe0();
void Vector_AddPolarOffsetFar();
s32 Map_GetTerrainHeight();
void Func_02002fb0();
s32 Func_02002f80();
void ObjectDispatch_SetAnimationSpeed();
void Func_02002fb8();
s32 Func_02003028();
void WaitFrames();

s32 Func_0200189c(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r7_1, r10_1, r6_1, r8_1, r5_1, r9_1, r11_1, r5_2, r10_2, r8_2, r6_2,
      r11_2;
  s32 v0, r7, r0, r10, v1, r6, r8, v2, r5, v3, r9, r3, v4, v5, r11, v6, v7, v8,
      v9, v10, v11, r1, r2;
  s32 stack[10];

  /* sub (s32)stack, #40 */
  v0 = ObjectTable_Get(FIELD_AT((s32)(s32)&gPartyState, s32, 532));
  r7_1 = v0;
  r0 = v0;
L_020098bc:
  Func_02001868(r0, r1, r2);
  FIELD_AT(r7_1, s32, 48) = 0x10000;
  FIELD_AT(r7_1, s32, 52) = 0x8000;
  FIELD_AT(r7_1, s32, 56) = -0x80000000;
  FIELD_AT(r7_1, s32, 64) = -0x80000000;
  FIELD_AT(r7_1, s32, 36) = 0;
  FIELD_AT(r7_1, s32, 44) = 0;
  r10_1 = (s32)((u8 *)stack + 28);
  stack[3] = ((FIELD_AT(r7_1, s32, 8)) & -1048576) + 524288;
  FIELD_AT(r10_1, s32, 0) = ((FIELD_AT(r7_1, s32, 8)) & -1048576) + 524288;
  FIELD_AT(r10_1, s32, 4) = FIELD_AT(r7_1, s32, 12);
  stack[2] = ((FIELD_AT(r7_1, s32, 16)) & -1048576) + 524288;
  FIELD_AT(r10_1, s32, 8) = ((FIELD_AT(r7_1, s32, 16)) & -1048576) + 524288;
  stack[4] = ((FIELD_AT(r7_1, s32, 16)) & -1048576) + 524288;
  stack[5] = stack[3];
  stack[1] = r7_1 + 34;
  v1 = Func_02002fe0(FIELD_AT(r7_1, u8, 34), stack[3], stack[4]);
  stack[6] = v1;
  r6_1 = FIELD_AT(r7_1, s32, 16);
  r6_1 = stack[4] - r6_1;
  r8_1 = (s32)IwramMulQ16;
  v2 = Iwram_MulQ16(stack[5] - (FIELD_AT(r7_1, s32, 8)),
                    stack[5] - (FIELD_AT(r7_1, s32, 8)));
  r5_1 = v2;
  v3 = Iwram_MulQ16(r6_1, r6_1);
  r5_1 = r5_1 + v3;
  r0 = 0x40000;
  if (r5_1 >= 0x40000)
    goto L_02009974;
  r9 = *(s16 *)(((s32)gData_020032b2) +
                ((((u32)(FIELD_AT((s32)gInput, s32, 0)) >> 4) & 15) << 1));
  r3 = (*(s16 *)(((s32)gData_020032b2) +
                 ((((u32)(FIELD_AT((s32)gInput, s32, 0)) >> 4) & 15) << 1)))
       << 16;
  if (((*(s16 *)(((s32)gData_020032b2) +
                 ((((u32)(FIELD_AT((s32)gInput, s32, 0)) >> 4) & 15) << 1)))
       << 16) != -0x10000)
    goto L_020099a0;
  goto L_02009b36;
L_02009974:
  v4 = ArcTan2(stack[4] - (FIELD_AT(r7_1, s32, 16)),
               stack[5] - (FIELD_AT(r7_1, s32, 8)));
  r9_1 = (v4 << 16) >> 16;
  r9_1 = (((8192 + r9_1) & -16384) << 16) >> 16;
  goto L_020099a0;
L_020099a0:
  Vector_AddPolarOffsetFar(0x100000, r9_1, (s32)((u8 *)stack + 28));
  v5 = Func_02002fe0(FIELD_AT(stack[1], u8, 0), FIELD_AT(r10_1, s32, 0),
                     FIELD_AT(r10_1, s32, 8));
  r11_1 = v5;
  if (v5 == 255)
    goto L_02009a22;
  v6 = Map_GetTerrainHeight(FIELD_AT(stack[1], u8, 0), FIELD_AT(r10_1, s32, 0),
                            FIELD_AT(r10_1, s32, 8));
  if ((v6 - (FIELD_AT(r7_1, s32, 12))) > 0x80000)
    goto L_02009a22;
  FIELD_AT(r10_1, s32, 0) = stack[3];
  FIELD_AT(r10_1, s32, 8) = stack[2];
  FIELD_AT(r7_1, s32, 48) = 0x20000;
  FIELD_AT(r7_1, s32, 52) = 0x1999;
  FIELD_AT(r7_1, u16, 100) = 0;
  Func_02002fb0(r7_1, stack[3], FIELD_AT(r7_1, s32, 12), stack[2]);
  Call2((void (*)())Func_02002f80, r7_1, 2);
  Call2((void (*)())ObjectDispatch_SetAnimationSpeed, r7_1, 48);
  Func_02002fb8(r7_1);
  FIELD_AT(r7_1, s32, 108) = (s32)Func_020017dc;
  goto L_02009acc;
L_02009a22:
  FIELD_AT(r7_1, u16, 6) = r9_1;
  FIELD_AT(r7_1, s32, 36) = 0;
  FIELD_AT(r7_1, s32, 44) = 0;
  FIELD_AT(r7_1, s32, 8) = stack[3];
  FIELD_AT(r7_1, s32, 16) = stack[2];
  goto L_02009b18;
L_02009a36:
  v7 = Map_GetTerrainHeight(FIELD_AT(stack[1], u8, 0), FIELD_AT(r6_1, s32, 0),
                            FIELD_AT(r6_1, s32, 8));
  if ((v7 - (FIELD_AT(r7_1, s32, 12))) > 0x80000)
    goto L_02009aec;
  stack[0] = (FIELD_AT(r7_1, u16, 32)) - 2;
  r5_2 = (s32)Ram_HeapSlots->scriptObjects;
  r10_2 = 0;
  r8_2 = 89 + r5_2;
L_02009a64:
  if ((FIELD_AT(r5_2, s32, 0)) == 0)
    goto L_02009a8e;
  if ((1 & (FIELD_AT(r8_2, u8, 0))) == 0)
    goto L_02009a8e;
  if (r5_2 == r7_1)
    goto L_02009a8e;
  v8 = Func_02003028(r5_2 + 8, stack[0], (s32)((u8 *)stack + 28),
                     (FIELD_AT(r5_2, u16, 32)) - 2,
                     (FIELD_AT(r7_1, u16, 32)) - 2);
  if (v8 >= 0)
    goto L_02009aec;
L_02009a8e:
  r10_2 = r10_2 + 1;
  r8_2 = r8_2 + 128;
  r5_2 = r5_2 + 128;
  if (r10_2 <= 63)
    goto L_02009a64;
  stack[5] = FIELD_AT(r6_1, s32, 0);
  stack[4] = FIELD_AT(r6_1, s32, 8);
  FIELD_AT(r7_1, s32, 48) = 0x20000;
  FIELD_AT(r7_1, s32, 52) = 0x1999;
  Func_02002fb0(r7_1, FIELD_AT(r6_1, s32, 0), FIELD_AT(r6_1, s32, 4),
                FIELD_AT(r6_1, s32, 8));
  Func_02002fb8(r7_1);
  if (r11_1 != stack[6])
    goto L_02009b12;
L_02009acc:
  Vector_AddPolarOffsetFar(0x100000, r9_1, (s32)((u8 *)stack + 28));
  r6_2 = (s32)((u8 *)stack + 28);
  v9 = Func_02002fe0(FIELD_AT(stack[1], u8, 0), FIELD_AT(r6_2, s32, 0),
                     FIELD_AT(r6_2, s32, 8));
  r11_2 = v9;
  if (v9 != 255)
    goto L_02009a36;
L_02009aec:
  FIELD_AT(r7_1, s32, 48) = 0x20000;
  FIELD_AT(r7_1, s32, 52) = 0x10000;
  Func_02002fb0(r7_1, stack[5], FIELD_AT(r7_1, s32, 12), stack[4]);
  Func_02002fb8(r7_1);
  Call1((void (*)())WaitFrames, 2);
  r0 = v10;
  goto L_020098bc;
L_02009b12:
  Call1((void (*)())WaitFrames, 10);
L_02009b18:
  FIELD_AT(r7_1, s32, 108) = 0;
  FIELD_AT(r7_1, u8, 90) = 1 | (FIELD_AT(r7_1, u8, 90));
  FIELD_AT(r7_1, s32, 52) = 0x4000;
  v11 = Func_02002f80(r7_1, 1);
  r0 = v11;
L_02009b36:
  /* add (s32)stack, #40 */
  return r0;
}
