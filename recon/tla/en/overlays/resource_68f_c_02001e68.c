// Draft, differ 315 of 246: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 Func_020017dc[];
void Func_02001868();
s32 Func_02002fe0();
void Vector_AddPolarOffsetFar();
s32 Map_GetTerrainHeight();
s32 Func_02002f80();
void Actor_SetAnimationSpeed();
s32 Func_02003028();
void Func_02002fb0();
void Func_02002fb8();
void wait();

s32 Func_02001e68(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 v0, r5, r0, r2, r3, r9, r6, r8, r1, r7, r11, v1, v2, r10, v3, v4, v5, v6,
      v7, v8;
  s32 stack[8];

  /* sub (s32)stack, #32 */
  v0 = ObjectTable_Get(FIELD_AT((s32)(s32)&gMainRoutineWork, s32, 532));
  r5 = v0;
  stack[4] = ((((FIELD_AT(r5, u16, 6)) + 8192) & -16384) << 16) >> 16;
  r0 = v0;
  r2 = -0x4000;
  r3 = ((((FIELD_AT(r5, u16, 6)) + 8192) & -16384) << 16) >> 16;
L_02009e9a:
  Func_02001868();
  FIELD_AT(r5, s32, 48) = 0x10000;
  FIELD_AT(r5, s32, 52) = 0x8000;
  FIELD_AT(r5, s32, 56) = -0x80000000;
  FIELD_AT(r5, s32, 64) = -0x80000000;
  FIELD_AT(r5, s32, 36) = 0;
  FIELD_AT(r5, s32, 44) = 0;
  r9 = 0x80000;
  r6 = (s32)((u8 *)stack + 20);
  FIELD_AT(r6, s32, 0) = ((FIELD_AT(r5, s32, 8)) & -1048576) + r9;
  r8 = ((FIELD_AT(r5, s32, 8)) & -1048576) + r9;
  r1 = 0x80000;
  r2 = -0x100000;
  goto L_02009edc;
L_02009edc:
  FIELD_AT(r6, s32, 4) = FIELD_AT(r5, s32, 12);
  r7 = ((FIELD_AT(r5, s32, 16)) & r2) + r1;
  FIELD_AT(r6, s32, 8) = r7;
  stack[2] = r8;
  stack[1] = r7;
  r11 = 34 + r5;
  v1 = Func_02002fe0(FIELD_AT(r5, u8, 34), r8, r7);
  stack[3] = v1;
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, stack[4], r6);
  v2 = Func_02002fe0(FIELD_AT(r11, u8, 0), FIELD_AT(r6, s32, 0),
                     FIELD_AT(r6, s32, 8));
  r10 = v2;
  if (v2 == 255)
    goto L_02009f70;
  v3 = Map_GetTerrainHeight(FIELD_AT(r11, u8, 0), FIELD_AT(r6, s32, 0),
                            FIELD_AT(r6, s32, 8));
  if ((v3 - (FIELD_AT(r5, s32, 12))) > r9)
    goto L_02009f70;
  FIELD_AT(r6, s32, 0) = stack[2];
  FIELD_AT(r6, s32, 8) = stack[1];
  FIELD_AT(r5, s32, 48) = 0x20000;
  FIELD_AT(r5, s32, 52) = 0x1999;
  FIELD_AT(r5, u16, 100) = 0;
  Call2((void (*)())Func_02002f80, r5, 2);
  Call2((void (*)())Actor_SetAnimationSpeed, r5, 48);
  FIELD_AT(r5, s32, 108) = (s32)Func_020017dc;
  goto L_0200a01a;
L_02009f70:
  FIELD_AT(r5, u16, 6) = FIELD_AT((s32)((u8 *)stack + 16), u16, 0);
  FIELD_AT(r5, s32, 36) = 0;
  FIELD_AT(r5, s32, 44) = 0;
  FIELD_AT(r5, s32, 8) = r8;
  FIELD_AT(r5, s32, 16) = r7;
  goto L_0200a066;
L_02009f84:
  v4 = Map_GetTerrainHeight(FIELD_AT(r11, u8, 0), FIELD_AT(r7, s32, 0),
                            FIELD_AT(r7, s32, 8));
  if ((v4 - (FIELD_AT(r5, s32, 12))) > 0x80000)
    goto L_0200a03a;
  stack[0] = (FIELD_AT(r5, u16, 32)) - 2;
  r6 = (s32)Ram_HeapSlots->scriptObjects;
  r9 = 0;
  r8 = 89 + r6;
L_02009fb2:
  if ((FIELD_AT(r6, s32, 0)) == 0)
    goto L_02009fdc;
  if ((1 & (FIELD_AT(r8, u8, 0))) == 0)
    goto L_02009fdc;
  if (r6 == r5)
    goto L_02009fdc;
  v5 = Func_02003028(r6 + 8, stack[0], (s32)((u8 *)stack + 20),
                     (FIELD_AT(r6, u16, 32)) - 2, (FIELD_AT(r5, u16, 32)) - 2);
  if (v5 >= 0)
    goto L_0200a03a;
L_02009fdc:
  r9 = r9 + 1;
  r8 = r8 + 128;
  r6 = r6 + 128;
  if (r9 <= 63)
    goto L_02009fb2;
  stack[2] = FIELD_AT(r7, s32, 0);
  stack[1] = FIELD_AT(r7, s32, 8);
  FIELD_AT(r5, s32, 48) = 0x20000;
  FIELD_AT(r5, s32, 52) = 0x1999;
  Func_02002fb0(r5, FIELD_AT(r7, s32, 0), FIELD_AT(r7, s32, 4),
                FIELD_AT(r7, s32, 8));
  Func_02002fb8(r5);
  if (r10 != stack[3])
    goto L_0200a060;
L_0200a01a:
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, stack[4],
        (s32)((u8 *)stack + 20));
  r7 = (s32)((u8 *)stack + 20);
  v6 = Func_02002fe0(FIELD_AT(r11, u8, 0), FIELD_AT(r7, s32, 0),
                     FIELD_AT(r7, s32, 8));
  r10 = v6;
  if (v6 != 255)
    goto L_02009f84;
L_0200a03a:
  FIELD_AT(r5, s32, 48) = 0x20000;
  FIELD_AT(r5, s32, 52) = 0x10000;
  Func_02002fb0(r5, stack[2], FIELD_AT(r5, s32, 12), stack[1]);
  Func_02002fb8(r5);
  Call1((void (*)())wait, 2);
  r0 = v7;
  goto L_02009e9a;
L_0200a060:
  Call1((void (*)())wait, 10);
L_0200a066:
  FIELD_AT(r5, s32, 108) = 0;
  FIELD_AT(r5, u8, 90) = 1 | (FIELD_AT(r5, u8, 90));
  FIELD_AT(r5, s32, 52) = 0x4000;
  v8 = Func_02002f80(r5, 1);
  /* add (s32)stack, #32 */
  return v8;
}
