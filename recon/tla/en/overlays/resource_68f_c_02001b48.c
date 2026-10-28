// Draft, differ 458 of 347: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gData_02003cd0[];
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
void Actor_SetAnimationSpeed();
void wait();
s32 Func_02003018();
s32 Func_02003028();
void Func_02002fb8();

s32 Func_02001b48(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 v0, r7, r0, r2, r3, r10, r1, v1, r6, r8, v2, r5, v3, r9, v4, v5, r11, v6,
      v7, v8, v9, v10, v11, v12, v13;
  s32 stack[10];

  /* sub (s32)stack, #40 */
  v0 = ObjectTable_Get(FIELD_AT((s32)(s32)&gSequenceParentWork, s32, 532));
  r7 = v0;
  FIELD_AT((s32)gData_02003cd0, u16, 0) = 0;
  r0 = v0;
  r2 = (s32)gData_02003cd0;
  r3 = 0;
L_02009b6e:
  Func_02001868(r0, r1);
  FIELD_AT(r7, s32, 48) = 0x10000;
  FIELD_AT(r7, s32, 52) = 0x8000;
  FIELD_AT(r7, s32, 56) = -0x80000000;
  FIELD_AT(r7, s32, 64) = -0x80000000;
  FIELD_AT(r7, s32, 36) = 0;
  FIELD_AT(r7, s32, 44) = 0;
  r10 = (s32)((u8 *)stack + 28);
  stack[3] = ((FIELD_AT(r7, s32, 8)) & -1048576) + 524288;
  FIELD_AT(r10, s32, 0) = ((FIELD_AT(r7, s32, 8)) & -1048576) + 524288;
  r0 = r10;
  r2 = 0x80000;
  r1 = -0x100000;
  goto L_02009bb4;
L_02009bb4:
  FIELD_AT(r0, s32, 4) = FIELD_AT(r7, s32, 12);
  stack[2] = ((FIELD_AT(r7, s32, 16)) & r1) + r2;
  FIELD_AT(r0, s32, 8) = ((FIELD_AT(r7, s32, 16)) & r1) + r2;
  stack[4] = ((FIELD_AT(r7, s32, 16)) & r1) + r2;
  stack[5] = stack[3];
  stack[1] = r7 + 34;
  v1 = Func_02002fe0(FIELD_AT(r7, u8, 34), stack[3], stack[4]);
  stack[6] = v1;
  r6 = FIELD_AT(r7, s32, 16);
  r6 = stack[4] - r6;
  r8 = (s32)IwramMulQ16;
  v2 = Iwram_MulQ16(stack[5] - (FIELD_AT(r7, s32, 8)),
                    stack[5] - (FIELD_AT(r7, s32, 8)));
  r5 = v2;
  v3 = Iwram_MulQ16(r6, r6);
  r5 = r5 + v3;
  if (r5 >= 0x40000)
    goto L_02009c30;
  r9 = *(s16 *)(((s32)gData_020032b2) +
                ((((u32)(FIELD_AT((s32)gInput, s32, 0)) >> 4) & 15) << 1));
  if (((*(s16 *)(((s32)gData_020032b2) +
                 ((((u32)(FIELD_AT((s32)gInput, s32, 0)) >> 4) & 15) << 1)))
       << 16) != -0x10000)
    goto L_02009c5c;
  goto L_02009e26;
L_02009c30:
  v4 = ArcTan2(stack[4] - (FIELD_AT(r7, s32, 16)),
               stack[5] - (FIELD_AT(r7, s32, 8)));
  r9 = (v4 << 16) >> 16;
  r9 = (((8192 + r9) & -16384) << 16) >> 16;
  goto L_02009c5c;
L_02009c5c:
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, r9,
        (s32)((u8 *)stack + 28));
  v5 = Func_02002fe0(FIELD_AT(stack[1], u8, 0), FIELD_AT(r10, s32, 0),
                     FIELD_AT(r10, s32, 8));
  r11 = v5;
  if (v5 == 255)
    goto L_02009cd6;
  v6 = Map_GetTerrainHeight(FIELD_AT(stack[1], u8, 0), FIELD_AT(r10, s32, 0),
                            FIELD_AT(r10, s32, 8));
  if ((v6 - (FIELD_AT(r7, s32, 12))) > 0x80000)
    goto L_02009cd6;
  FIELD_AT(r10, s32, 0) = stack[3];
  FIELD_AT(r10, s32, 8) = stack[2];
  FIELD_AT(r7, s32, 48) = 0x20000;
  FIELD_AT(r7, s32, 52) = 0x1999;
  FIELD_AT(r7, u16, 100) = 0;
  Func_02002fb0(r7, stack[3], FIELD_AT(r7, s32, 12), stack[2]);
  Call2((void (*)())Func_02002f80, r7, 2);
  Call2((void (*)())Actor_SetAnimationSpeed, r7, 48);
  r5 = 0;
  goto L_02009cfe;
L_02009cd6:
  FIELD_AT(r7, u16, 6) = r9;
  FIELD_AT(r7, s32, 36) = 0;
  FIELD_AT(r7, s32, 44) = 0;
  FIELD_AT(r7, s32, 8) = stack[3];
  FIELD_AT(r7, s32, 16) = stack[2];
  goto L_02009e26;
L_02009cea:
  if (((FIELD_AT((s32)gData_02003cd0, u16, 0)) << 16) == 0)
    goto L_02009cf6;
  goto L_02009e26;
L_02009cf6:
  Call1((void (*)())wait, 1);
  r5 = r5 + 1;
L_02009cfe:
  if (r5 <= 179) {
    v7 = Func_02003018(r7);
    if (v7 == 0)
      goto L_02009cea;
  }
  FIELD_AT(r7, s32, 108) = (s32)Func_020017dc;
  goto L_02009dda;
L_02009d12:
  v8 = Map_GetTerrainHeight(FIELD_AT(stack[1], u8, 0), FIELD_AT(r6, s32, 0),
                            FIELD_AT(r6, s32, 8));
  if ((v8 - (FIELD_AT(r7, s32, 12))) > 0x80000)
    goto L_02009dfa;
  if (((FIELD_AT((s32)gData_02003cd0, u16, 0)) << 16) != 0)
    goto L_02009e26;
  stack[0] = (FIELD_AT(r7, u16, 32)) - 2;
  r5 = (s32)Ram_HeapSlots->scriptObjects;
  r10 = 0;
  r8 = 89 + r5;
L_02009d4a:
  if ((FIELD_AT(r5, s32, 0)) == 0)
    goto L_02009d74;
  if ((1 & (FIELD_AT(r8, u8, 0))) == 0)
    goto L_02009d74;
  if (r5 == r7)
    goto L_02009d74;
  v9 = Func_02003028(r5 + 8, stack[0], (s32)((u8 *)stack + 28),
                     (FIELD_AT(r5, u16, 32)) - 2, (FIELD_AT(r7, u16, 32)) - 2);
  if (v9 >= 0)
    goto L_02009dfa;
L_02009d74:
  r10 = r10 + 1;
  r8 = r8 + 128;
  r5 = r5 + 128;
  if (r10 <= 63)
    goto L_02009d4a;
  stack[5] = FIELD_AT(r6, s32, 0);
  stack[4] = FIELD_AT(r6, s32, 8);
  FIELD_AT(r7, s32, 48) = 0x20000;
  FIELD_AT(r7, s32, 52) = 0x1999;
  r5 = 0;
  Func_02002fb0(r7, FIELD_AT(r6, s32, 0), FIELD_AT(r6, s32, 4),
                FIELD_AT(r6, s32, 8));
  goto L_02009db2;
L_02009daa:
  Call1((void (*)())wait, 1);
  r5 = r5 + 1;
L_02009db2:
  if (r5 > 179)
    goto L_02009dca;
  v10 = Func_02003018(r7);
  if (v10 != 0)
    goto L_02009dca;
  if (((FIELD_AT((s32)gData_02003cd0, u16, 0)) << 16) == 0)
    goto L_02009daa;
L_02009dca:
  if (((FIELD_AT((s32)gData_02003cd0, u16, 0)) << 16) != 0)
    goto L_02009e26;
  if (r11 != stack[6])
    goto L_02009e20;
L_02009dda:
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, r9,
        (s32)((u8 *)stack + 28));
  r6 = (s32)((u8 *)stack + 28);
  v11 = Func_02002fe0(FIELD_AT(stack[1], u8, 0), FIELD_AT(r6, s32, 0),
                      FIELD_AT(r6, s32, 8));
  r11 = v11;
  if (v11 != 255)
    goto L_02009d12;
L_02009dfa:
  FIELD_AT(r7, s32, 48) = 0x20000;
  FIELD_AT(r7, s32, 52) = 0x10000;
  Func_02002fb0(r7, stack[5], FIELD_AT(r7, s32, 12), stack[4]);
  Func_02002fb8(r7);
  Call1((void (*)())wait, 2);
  r0 = v12;
  goto L_02009b6e;
L_02009e20:
  Call1((void (*)())wait, 10);
L_02009e26:
  FIELD_AT(r7, s32, 108) = 0;
  FIELD_AT(r7, u8, 90) = 1 | (FIELD_AT(r7, u8, 90));
  FIELD_AT(r7, s32, 52) = 0x4000;
  v13 = Func_02002f80(r7, 1);
  /* add (s32)stack, #40 */
  return v13;
}
