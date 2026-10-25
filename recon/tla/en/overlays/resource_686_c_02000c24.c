// Draft, differ 36 of 249: translated from its listing; names and structure still
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

s32 Object_GetById();
void GameFlag_SetBit();
void Func_02000b30();
s32 GameFlag_Test();
void Func_02002e44();
void Func_02002edc();
void Func_02002e24();
void Camera_MoveWithinMapBounds();
void FieldEvent_WaitFrames();
void Func_02002eec();
void Func_02002e04();
s32 Func_02002df4();
void Func_02000b78();
s32 Func_02002e4c();

s32 Func_02000c24(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r5_1, r7_1, r6_1, r5_2, r6_2, r9_1, r10_1, r8_1;
  s32 r5, v0, r7, r6, v1, r0, v2, r9, r10, r8, v3, v4, r1, r2;
  s32 stack[2];

  r5_1 = a1;
  /* sub (s32)stack, #8 */
  v0 = Object_GetById(r5_1);
  r7_1 = (FIELD_AT(v0, s32, 8)) >> 20;
  r6_1 = (FIELD_AT(v0, s32, 16)) >> 20;
  if (r5_1 != 9)
    goto L_02008c5e;
  if (r7_1 != 12)
    goto L_02008c5e;
  if (r6_1 != 45)
    goto L_02008c5e;
  Call1((void (*)())GameFlag_SetBit, 0x9d9);
  Call2((void (*)())Func_02000b30, 9, 181);
L_02008c5e:
  if (r5_1 != 10)
    goto L_02008c7c;
  if (r7_1 != 14)
    goto L_02008c7c;
  if (r6_1 != 45)
    goto L_02008c7c;
  Call1((void (*)())GameFlag_SetBit, 0x9da);
  Call2((void (*)())Func_02000b30, 10, 181);
L_02008c7c:
  v1 = GameFlag_Test(0x9da);
  r0 = v1;
  if (v1 != 0)
    goto L_02008c8c;
  goto L_02008e50;
L_02008c8c:
  v2 = GameFlag_Test(0x9d9);
  r0 = v2;
  if (v2 != 0)
    goto L_02008c9c;
  goto L_02008e50;
L_02008c9c:
  Func_02002e44(r0, r1, r2);
  Call1((void (*)())Func_02002edc, 0);
  Call3((void (*)())Func_02002e24, 0x20000, 0x20000, 0x10000);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0xd80000, -1, 0x3020000, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call1((void (*)())Func_02002eec, 202);
  r5_2 = 12;
  r6_2 = 111;
  Call6((void (*)())Func_02002e04, 12, 91, 3, 1, r5_2, r6_2);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call1((void (*)())Func_02002eec, 0x23f);
  stack[1] = 47;
  r9_1 = 47;
  Call6((void (*)())Func_02002df4, 16, 27, 3, 2, r5_2, 47);
  Call6((void (*)())Func_02002df4, 16, 91, 3, 2, r5_2, r6_2);
  r10_1 = -40;
  stack[0] = 0x2e8;
  stack[1] = 3;
  r8_1 = 3;
  Call6((void (*)())Func_02000b78, 3, 15, 200, r10_1, 0x2e8, 3);
  Call6((void (*)())Func_02002df4, 16, 27, 3, 3, r5_2, r9_1);
  Call6((void (*)())Func_02002df4, 16, 91, 3, 3, r5_2, r6_2);
  Call6((void (*)())Func_02000b78, 3, 10, 200, r10_1, 0x2f8, r8_1);
  Call6((void (*)())Func_02002df4, 16, 27, 3, 4, r5_2, r9_1);
  Call6((void (*)())Func_02002df4, 16, 91, 3, 4, r5_2, r6_2);
  Call6((void (*)())Func_02000b78, 3, 10, 200, r10_1, 0x308, r8_1);
  Call6((void (*)())Func_02002df4, 16, 27, 3, 5, r5_2, r9_1);
  Call6((void (*)())Func_02002df4, 16, 91, 3, 5, r5_2, r6_2);
  Call6((void (*)())Func_02000b78, 3, 10, 200, r10_1, 0x318, r8_1);
  Call6((void (*)())Func_02002df4, 16, 27, 3, 6, r5_2, r9_1);
  Call6((void (*)())Func_02002df4, 16, 91, 3, 6, r5_2, r6_2);
  Call3((void (*)())Func_02002e24, -1, -1, 0xe666);
  Call6((void (*)())Func_02000b78, 3, 10, 200, r10_1, 0x328, r8_1);
  Call6((void (*)())Func_02002df4, 16, 27, 3, 13, r5_2, r9_1);
  v3 = Func_02002df4(16, 91, 3, 13, r5_2, r6_2);
  v4 = Func_02002e4c();
  r0 = v4;
L_02008e50:
  /* add (s32)stack, #8 */
  return r0;
}
