// Draft, differ 429 of 410: translated from its listing; names and structure
// still to do.
#include "types.h"
#include "lib/call.h"
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

extern u8 Data_02004030[];
extern u8 gData_02003780[];
extern u8 gData_02003788[];
extern u8 gData_02003782[];
extern u8 Func_020015ac[];
extern u8 gData_02002ff0[];
s32 Object_GetById();
void Object_SetModeById();
s32 Func_02002e24();
void FieldEvent_WaitFrames();
s32 Func_02002eec();
s32 Func_02002df4();
s32 Func_02002e04();
s32 Func_02002dfc();
s32 Scheduler_AddOrUpdateCallback();
void GameFlag_SetBit();
void GameFlag_ClearBit();

s32 Func_020019a0(void) {
  s32 r5, v0, r7, r6, r9, r8, r10, r4, r0, r2, r3, r1, r11, v1, v2;
  s32 stack[5];

  r5 = (s32)&gPartyState;
  r5 = r5 + 532;
  /* sub (s32)stack, #20 */
  v0 = Object_GetById(FIELD_AT(r5, s32, 0));
  r7 = (s32)Data_02004030;
  r6 = v0;
  r9 = (s32)Ram_HeapSlots->mapWork;
  Call2((void (*)())Object_SetModeById, FIELD_AT(r5, s32, 0), 0);
  r8 = (FIELD_AT(r7, s32, 4)) >> 19;
  FIELD_AT(r6, u8, 85) = 0;
  r10 = 85 + r6;
  Call3((void (*)())Func_02002e24, 0x50000, 0x50000, 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call1((void (*)())Func_02002eec, 0x22c);
  stack[1] = 84;
  r5 = 7;
  Call6((void (*)())Func_02002df4, 22, 66, 3, 1, r5, 84);
  Call6((void (*)())Func_02002df4, 22, 1, 3, 5, r5, 19);
  stack[4] = 0;
L_02009a2a:
  r4 = 0;
  if ((*(s16 *)(((s32)gData_02003780) + r4)) <= 0)
    goto L_02009a52;
  r5 = 128;
  r5 = r5 << 9;
L_02009a38:
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  r5 = r5 + 65536;
  if ((r5 >> 16) < (FIELD_AT((s32)gData_02003780, s16, 0)))
    goto L_02009a38;
L_02009a52:
  FIELD_AT(r7, s32, 4) =
      (FIELD_AT(r7, s32, 4)) + (FIELD_AT((s32)gData_02003788, s32, 0));
  r0 = FIELD_AT((s32)gData_02003788, s32, 0);
  r2 = FIELD_AT(r9, s32, 424);
  r3 = FIELD_AT(r6, s32, 8);
  if ((FIELD_AT(r6, s32, 8)) >= 0)
    goto L_02009a6e;
  r4 = 0xfffff;
  r3 = r3 + r4;
L_02009a6e:
  r1 = r3 >> 20;
  r3 = FIELD_AT(r6, s32, 16);
  if ((FIELD_AT(r6, s32, 16)) >= 0)
    goto L_02009a7a;
  r4 = 0xfffff;
  r3 = r3 + r4;
L_02009a7a:
  if ((FIELD_AT(r2 + ((r1 + ((r3 >> 20) << 7)) << 2), u8, 2)) != 232)
    goto L_02009a92;
  FIELD_AT(r6, s32, 12) = (FIELD_AT(r6, s32, 12)) + r0;
  FIELD_AT(r6, s32, 20) = (FIELD_AT(r6, s32, 12)) + r0;
L_02009a92:
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  if (r8 != 2)
    goto L_02009ade;
  if (stack[4] != 0)
    goto L_02009ade;
  Call6((void (*)())Func_02002df4, 11, 70, 1, 1, 11, 69);
  r5 = 4;
  Call6((void (*)())Func_02002e04, 10, 68, 1, 1, r5, 86);
  Call6((void (*)())Func_02002dfc, 4, 24, 1, 1, r5, 23);
L_02009ade:
  if (r8 != 1)
    goto L_02009afe;
  if (stack[4] != 15)
    goto L_02009afe;
  Call6((void (*)())Func_02002e04, 18, 73, 1, 1, 4, 86);
L_02009afe:
  stack[4] = ((stack[4] + 1) << 16) >> 16;
  r3 = ((stack[4] + 1) << 16) >> 16;
  if ((((stack[4] + 1) << 16) >> 16) <= 15)
    goto L_02009a2a;
  Call3((void (*)())Func_02002e24, -1, -1, 0xe666);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  r4 = r10;
  FIELD_AT(r4, u8, 0) = 3;
  if ((FIELD_AT(r7, s32, 4)) >= 0x200000)
    goto L_02009b46;
  FIELD_AT((s32)gData_02003782, u16, 0) = 0;
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_020015ac, 0x480);
  goto L_02009b6a;
L_02009b46:
  Call6((void (*)())Func_02002e04, 69, 24, 1, 4, 68, 23);
  Call1((void (*)())GameFlag_SetBit, 0x204);
  Call1((void (*)())GameFlag_ClearBit, 0x17d);
L_02009b6a:
  stack[1] = 84;
  r5 = 7;
  Call6((void (*)())Func_02002df4, 18, 66, 3, 1, r5, 84);
  Call6((void (*)())Func_02002df4, 18, 1, 3, 5, r5, 19);
  stack[4] = (FIELD_AT(r7, s32, 4)) >> 19;
  r2 = (s32)gData_02002ff0;
  if ((FIELD_AT((s32)gData_02002ff0, u8, 0)) == 99)
    goto L_02009c4a;
  r11 = 4;
  r7 = 0;
L_02009ba4:
  r4 = stack[4];
  r1 = r2;
  if (r4 < (*(u8 *)(r2 + r7)))
    goto L_02009c3c;
  r8 = 0;
  if (r8 >= (FIELD_AT(r7 + r2, u8, 2)))
    goto L_02009c3c;
L_02009bba:
  stack[3] = r7;
  r4 = 0;
  r9 = r1;
  if (r4 >= (FIELD_AT(r7 + r1, u8, 1)))
    goto L_02009c26;
  r10 = 0x10000;
L_02009bce:
  r6 = r7 + r9;
  r5 = 14;
  r5 = r5 - stack[4];
  Func_02002dfc(21, r5, 1, 1, (FIELD_AT(r6, u8, 3)) + r4,
                (*(u8 *)(r9 + r11)) + r8, r4, r7, (FIELD_AT(r7, s32, 4)) >> 19);
  r4 = stack[2];
  Call6((void (*)())Func_02002dfc, 25, r5, 1, 1, (FIELD_AT(r6, u8, 3)) + r4,
        ((*(u8 *)(r9 + r11)) + r8) + 64);
  r4 = r10 >> 16;
  r10 = r10 + 65536;
  if (r4 < (FIELD_AT(r6, u8, 1)))
    goto L_02009bce;
L_02009c26:
  r4 = stack[3];
  r8 = ((r8 + 1) << 16) >> 16;
  r1 = (s32)gData_02002ff0;
  if (r8 < (FIELD_AT(r4 + ((s32)gData_02002ff0), u8, 2)))
    goto L_02009bba;
L_02009c3c:
  r7 = r7 + 8;
  r11 = r11 + 8;
  r2 = (s32)gData_02002ff0;
  if ((*(u8 *)(((s32)gData_02002ff0) + r7)) != 99)
    goto L_02009ba4;
L_02009c4a:
  r5 = 14;
  r5 = r5 - stack[4];
  stack[1] = 27;
  stack[0] = 2;
  r9 = 2;
  Call6((void (*)())Func_02002dfc, 19, r5, 2, 1, 2, 27);
  stack[1] = 25;
  r6 = 13;
  Call6((void (*)())Func_02002dfc, 19, r5, 2, 1, r6, 25);
  stack[0] = 7;
  r10 = 7;
  r7 = 23;
  Call6((void (*)())Func_02002dfc, 18, r5, 1, 1, 7, r7);
  r4 = 9;
  r8 = r4;
  Call6((void (*)())Func_02002dfc, 24, r5, 1, 1, r4, r7);
  Call6((void (*)())Func_02002dfc, 19, r5, 2, 1, r9, 91);
  Call6((void (*)())Func_02002dfc, 19, r5, 2, 1, r6, 89);
  stack[0] = r10;
  r6 = 87;
  Call6((void (*)())Func_02002dfc, 26, r5, 1, 1, r10, r6);
  v1 = Func_02002dfc(27, r5, 1, 1, r8, r6);
  r0 = v1;
  r3 = stack[4];
  if (stack[4] != 2)
    goto L_02009d06;
  v2 = Func_02002dfc(18, 7, 1, 1, 4, r7);
  r0 = v2;
L_02009d06:
  /* add (s32)stack, #20 */
  return r0;
}
