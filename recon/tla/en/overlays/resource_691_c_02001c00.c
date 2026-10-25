// Draft, differ 79 of 218: translated from its listing; names and structure still
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
void Func_02003aec();
void Func_02003bcc();
void GameFlag_SetBit();
void Func_02003c54();
void Object_SetPartAttribute();
void Object_SetActionById();
s32 Random16Far();
void Func_0200015c();
void WaitFrames();
void Camera_MoveWithinMapBounds();
void FieldEvent_WaitFrames();
void ObjectDispatch_SetSingleChildFlags();
s32 Func_02003ac4();
void Func_02003acc();
s32 Func_02003b74();
void Func_02003b6c();
void GameFlag_ClearBit();
s32 Func_02003af4();

s32 Func_02001c00(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r5_1, r7_1, r9_1, r10_1, r5_2, r8_1, r5_3, r10_2, r6_1, r5_4, r5_5;
  s32 r5, v0, r7, v1, r9, r10, v2, r8, v3, r4, r2, r6, v4, v5, r3, v6, v7, v8,
      v9, v10, r0;
  s32 stack[17];

  r5_1 = (s32)&gPartyState;
  r5_1 = r5_1 + 532;
  /* sub (s32)stack, #68 */
  v0 = Object_GetById(FIELD_AT(r5_1, s32, 0));
  r7_1 = v0;
  Func_02003aec();
  Call1((void (*)())Func_02003bcc, 0);
  Call1((void (*)())GameFlag_SetBit, 0x260);
  Call1((void (*)())Func_02003c54, 133);
  v1 = Object_GetById(FIELD_AT(r5_1, s32, 0));
  Call2((void (*)())Object_SetPartAttribute, v1, 6);
  Call2((void (*)())Object_SetModeById, FIELD_AT(r5_1, s32, 0), 10);
  Call2((void (*)())Object_SetActionById, FIELD_AT(r5_1, s32, 0), 32);
  r9_1 = (s32)((u8 *)stack + 28);
  FIELD_AT(r9_1, s32, 0) = 1;
  FIELD_AT(r9_1, s32, 4) = 14;
  FIELD_AT(r9_1, s32, 8) = 0xcccc;
  FIELD_AT(r9_1, s32, 12) = 0xcccc;
  r10_1 = 0;
L_02009c6c:
  v2 = Random16Far();
  r5_2 = 15;
  FIELD_AT((s32)stack, s32, 16) = ((v2 & r5_2) << 16) - 524288;
  r8_1 = 16 + (s32)stack;
  v3 = Random16Far();
  FIELD_AT(r8_1, s32, 4) = (v3 & r5_2) << 16;
  r4 = FIELD_AT(r7_1, s32, 8);
  r4 = r4 + (FIELD_AT(r8_1, s32, 0));
  stack[0] = 0x13333;
  stack[2] = 0xb0000;
  stack[3] = r9_1;
  r5_3 = 0;
  Func_0200015c(r4, (FIELD_AT(r7_1, s32, 12)) + ((v3 & r5_3) << 16),
                FIELD_AT(r7_1, s32, 16), 0, 0x13333, r5_3, 0xb0000, r9_1);
  Call1((void (*)())WaitFrames, 5);
  r10_1 = r10_1 + 1;
  r2 = 1;
  if (r10_1 != 8)
    goto L_02009c6c;
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, r2 - 2, 0);
  FIELD_AT((s32)(s32)&gPartyState, u8, 530) = r5_3;
  FIELD_AT(r7_1, u8, 85) = 3;
  FIELD_AT(r7_1, s32, 72) = 0xcccc;
  FIELD_AT(r7_1, s32, 68) = r5_3;
  Call1((void (*)())FieldEvent_WaitFrames, 1);
  FIELD_AT(r7_1, u16, 6) = 0xc000;
  r10_2 = 60;
L_02009d02:
  r6_1 = FIELD_AT((s32)gFrameCount, s32, 0);
  r6_1 = r6_1 & 1;
  if (r6_1 == 0) {
    v4 = Random16Far();
    r5_4 = 15;
    FIELD_AT(r8_1, s32, 0) = ((v4 & r5_4) << 16) - 524288;
    v5 = Random16Far();
    FIELD_AT(r8_1, s32, 4) = (v5 & r5_4) << 16;
    r4 = FIELD_AT(r7_1, s32, 8);
    r4 = r4 + (FIELD_AT(r8_1, s32, 0));
    Func_0200015c(r4, (FIELD_AT(r7_1, s32, 12)) + ((v5 & r5_4) << 16),
                  FIELD_AT(r7_1, s32, 16), 0, 0x13333, r6_1, 0xb0000, r9_1);
  }
  Call1((void (*)())WaitFrames, 1);
  r3 = FIELD_AT(r7_1, s32, 40);
  if ((FIELD_AT(r7_1, s32, 40)) != 0) {
    r10_2 = r10_2 - 1;
    r2 = -1;
    r3 = r10_2;
    if (r10_2 != 0)
      goto L_02009d02;
  }
  Call1((void (*)())Func_02003c54, 188);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, r7_1, 1);
  r5_5 = (s32)&gPartyState;
  r5_5 = r5_5 + 532;
  Call2((void (*)())Object_SetModeById, FIELD_AT(r5_5, s32, 0), 22);
  Call3((void (*)())Func_02003ac4, 0x50000, 0x80000, 0x10000);
  v6 = Func_02003ac4(-1, -1, 0xe666);
  Func_02003acc();
  v7 = Func_02003b74(FIELD_AT(r5_5, s32, 0), 1);
  Func_02003b6c();
  Call2((void (*)())Object_SetModeById, FIELD_AT(r5_5, s32, 0), 4);
  v8 = Object_GetById(FIELD_AT(r5_5, s32, 0));
  Call2((void (*)())Object_SetPartAttribute, v8, 0);
  ObjectMotion_WaitForAnimationChange(FIELD_AT(r5_5, s32, 0));
  Call1((void (*)())GameFlag_ClearBit, 0x260);
  FIELD_AT(r7_1, s32, 72) = 0x10000;
  FIELD_AT(r7_1, s32, 68) = 0x4000;
  v10 = Func_02003af4();
  /* add (s32)stack, #68 */
  return v10;
}
