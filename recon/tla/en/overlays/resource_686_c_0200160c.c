// Draft, differ 459 of 410: translated from its listing; names and structure still
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

extern u8 Data_02004030[];
extern u8 gData_02003780[];
extern u8 gData_02003788[];
extern u8 gData_02003782[];
extern u8 Func_020015ac[];
extern u8 gData_02002fa8[];
s32 Object_GetById();
void Func_02002e24();
void FieldEvent_WaitFrames();
void Func_02002eec();
void Func_02002df4();
void Func_02002e04();
s32 Func_02002dfc();
s32 Scheduler_AddOrUpdateCallback();
void GameFlag_SetBit();
void GameFlag_ClearBit();

s32 Func_0200160c(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r5_1, r6_1;
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
  Call3((void (*)())Func_02002e24, 0x40000, 0x40000, 0x10000);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call1((void (*)())Func_02002eec, 0x22c);
  stack[1] = 66;
  r5_1 = 7;
  Call6((void (*)())Func_02002df4, 22, 66, 3, 1, r5_1, 66);
  Call6((void (*)())Func_02002df4, 22, 1, 3, 5, r5_1, 1);
  stack[4] = 0;
L_02009696:
  r4 = 0;
  if ((*(s16 *)(((s32)gData_02003780) + r4)) <= 0)
    goto L_020096be;
  r5_1 = 128;
  r5_1 = r5_1 << 9;
L_020096a4:
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  r5_1 = r5_1 + 65536;
  if ((r5_1 >> 16) < (FIELD_AT((s32)gData_02003780, s16, 0)))
    goto L_020096a4;
L_020096be:
  FIELD_AT(r7, s32, 4) =
      (FIELD_AT(r7, s32, 4)) + (FIELD_AT((s32)gData_02003788, s32, 0));
  r0 = FIELD_AT((s32)gData_02003788, s32, 0);
  r2 = FIELD_AT(r9, s32, 424);
  r3 = FIELD_AT(r6, s32, 8);
  if ((FIELD_AT(r6, s32, 8)) < 0) {
    r4 = 0xfffff;
    r3 = r3 + r4;
  }
  r1 = r3 >> 20;
  r3 = FIELD_AT(r6, s32, 16);
  if ((FIELD_AT(r6, s32, 16)) < 0) {
    r4 = 0xfffff;
    r3 = r3 + r4;
  }
  if ((FIELD_AT(r2 + ((r1 + ((r3 >> 20) << 7)) << 2), u8, 2)) == 232) {
    FIELD_AT(r6, s32, 12) = (FIELD_AT(r6, s32, 12)) + r0;
    FIELD_AT(r6, s32, 20) = (FIELD_AT(r6, s32, 12)) + r0;
  }
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  if (r8 != 2)
    goto L_02009748;
  if (stack[4] != 0)
    goto L_02009748;
  r5_1 = 11;
  Call6((void (*)())Func_02002df4, 11, 70, 1, 1, r5_1, 69);
  Call6((void (*)())Func_02002e04, 10, 68, 1, 1, r5_1, 68);
  Func_02002dfc(10, 5, 1, 1, r5_1, 5);
L_02009748:
  if (r8 != 1)
    goto L_02009768;
  if (stack[4] != 15)
    goto L_02009768;
  Call6((void (*)())Func_02002e04, 18, 73, 1, 1, 11, 68);
L_02009768:
  stack[4] = ((stack[4] + 1) << 16) >> 16;
  r3 = ((stack[4] + 1) << 16) >> 16;
  if ((((stack[4] + 1) << 16) >> 16) <= 15)
    goto L_02009696;
  Call3((void (*)())Func_02002e24, -1, -1, 0xe666);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  r4 = r10;
  FIELD_AT(r4, u8, 0) = 3;
  if ((FIELD_AT(r7, s32, 4)) < 0x200000) {
    FIELD_AT((s32)gData_02003782, u16, 0) = 0;
    Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_020015ac, 0x480);
  } else {
    Call6((void (*)())Func_02002e04, 69, 6, 1, 2, 68, 5);
    Call1((void (*)())GameFlag_SetBit, 0x204);
    Call1((void (*)())GameFlag_ClearBit, 0x17d);
  }
  stack[1] = 66;
  r5_1 = 7;
  Call6((void (*)())Func_02002df4, 18, 66, 3, 1, r5_1, 66);
  Value6((s32 (*)())Func_02002df4, 18, 1, 3, 5, r5_1, 1);
  stack[4] = (FIELD_AT(r7, s32, 4)) >> 19;
  r2 = (s32)gData_02002fa8;
  if ((FIELD_AT((s32)gData_02002fa8, u8, 0)) == 99)
    goto L_020098b4;
  r11 = 4;
  r7 = 0;
L_0200980e:
  r4 = stack[4];
  r1 = r2;
  if (r4 < (*(u8 *)(r2 + r7)))
    goto L_020098a6;
  r8 = 0;
  if (r8 >= (FIELD_AT(r7 + r2, u8, 2)))
    goto L_020098a6;
L_02009824:
  stack[3] = r7;
  r4 = 0;
  r9 = r1;
  if (r4 >= (FIELD_AT(r7 + r1, u8, 1)))
    goto L_02009890;
  r10 = 0x10000;
L_02009838:
  r6 = r7 + r9;
  r5_1 = 14;
  r5_1 = r5_1 - stack[4];
  Func_02002dfc(21, r5_1, 1, 1, (FIELD_AT(r6, u8, 3)) + r4,
                (*(u8 *)(r9 + r11)) + r8, r4, r7, (FIELD_AT(r7, s32, 4)) >> 19);
  r4 = stack[2];
  Func_02002dfc(25, r5_1, 1, 1, (FIELD_AT(r6, u8, 3)) + r4,
                ((*(u8 *)(r9 + r11)) + r8) + 64);
  r4 = r10 >> 16;
  r10 = r10 + 65536;
  if (r4 < (FIELD_AT(r6, u8, 1)))
    goto L_02009838;
L_02009890:
  r4 = stack[3];
  r8 = ((r8 + 1) << 16) >> 16;
  r1 = (s32)gData_02002fa8;
  if (r8 < (FIELD_AT(r4 + ((s32)gData_02002fa8), u8, 2)))
    goto L_02009824;
L_020098a6:
  r7 = r7 + 8;
  r11 = r11 + 8;
  r2 = (s32)gData_02002fa8;
  if ((*(u8 *)(((s32)gData_02002fa8) + r7)) != 99)
    goto L_0200980e;
L_020098b4:
  r5_1 = 14;
  r5_1 = r5_1 - stack[4];
  stack[1] = 7;
  r8 = 7;
  stack[0] = 2;
  r11 = 2;
  Call6((void (*)())Func_02002dfc, 19, r5_1, 2, 1, 2, 7);
  stack[1] = r8;
  r4 = 13;
  stack[0] = r4;
  r9 = r4;
  Call6((void (*)())Func_02002dfc, 22, r5_1, 2, 1, r4, r8);
  stack[0] = r8;
  r7 = 5;
  Call6((void (*)())Func_02002dfc, 18, r5_1, 1, 1, r8, r7);
  stack[0] = 9;
  r10 = 9;
  Call6((void (*)())Func_02002dfc, 24, r5_1, 1, 1, 9, r7);
  stack[0] = r11;
  r6 = 71;
  Call6((void (*)())Func_02002dfc, 19, r5_1, 2, 1, r11, r6);
  r4 = r9;
  Call6((void (*)())Func_02002dfc, 22, r5_1, 2, 1, r4, r6);
  stack[0] = r8;
  r6_1 = 69;
  Call6((void (*)())Func_02002dfc, 26, r5_1, 1, 1, r8, r6_1);
  v1 = Func_02002dfc(27, r5_1, 1, 1, r10, r6_1);
  r0 = v1;
  if (stack[4] == 2) {
    v2 = Func_02002dfc(18, 8, 1, 1, 11, r7);
    r0 = v2;
  }
  /* add (s32)stack, #20 */
  return r0;
}
