// Draft, differ 71 of 105: translated from its listing; names and structure still
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

void Func_02003048();
void Func_02003148();
void Func_020031a8();
void Func_02002fc0();
void FieldEvent_WaitFrames();
void ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_ResetAndSetPositionInMode2();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void ObjectMotion_CommitCurrentPositionAndActivate();
s32 Func_020030e8();
s32 Func_02003050();

s32 Func_020005d0(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r5_1, r7_1, r6_1, r6_2, r8_1, r5_2, r6_3, r5_3, r5_4;
  s32 r5, r7, r6, r2, r3, r8, v0, v1, v2, v3, v4, v5, v6, r0;
  s32 stack[2];

  r5_1 = (s32)Ram_HeapSlots->eventWork;
  /* sub (s32)stack, #8 */
  Func_02003048();
  Call1((void (*)())Func_02003148, 0);
  r7_1 = r5_1 + 340;
  r6_1 = 37;
  r2 = 0;
  r3 = FIELD_AT(r7_1, s16, 0);
  if ((FIELD_AT(r7_1, s16, 0)) == 3) {
    r6_2 = 25;
  }
  Call1((void (*)())Func_020031a8, 158);
  stack[0] = 1;
  r8_1 = 1;
  r5_2 = 2;
  Call6((void (*)())Func_02002fc0, 30, 38, r6_2, 49, 1, r5_2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call6((void (*)())Func_02002fc0, 32, 38, r6_2, 49, r8_1, r5_2);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  r6_3 = (s32)&gPartyState;
  r6_3 = r6_3 + 532;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, FIELD_AT(r6_3, s32, 0),
        0x8000, 0x4000);
  v0 = Object_GetById(FIELD_AT(r6_3, s32, 0));
  FIELD_AT(v0, u8, 85) = 0;
  v1 = Object_GetById(FIELD_AT(r6_3, s32, 0));
  r5_3 = FIELD_AT(v1, s32, 8);
  r5_3 = r5_3 >> 19;
  r5_3 = r5_3 | r8_1;
  v2 = Object_GetById(FIELD_AT(r6_3, s32, 0));
  r5_3 = r5_3 << 3;
  ObjectMotion_SetPositionAndReset(FIELD_AT(r6_3, s32, 0), r5_3,
                                   (*(s16 *)(v2 + 18)) | r8_1);
  Call1((void (*)())Func_020031a8, 123);
  v3 = Object_GetById(FIELD_AT(r6_3, s32, 0));
  r5_4 = FIELD_AT(v3, s32, 8);
  v4 = Object_GetById(FIELD_AT(r6_3, s32, 0));
  r5_4 = r5_4 >> 19;
  r5_4 = r5_4 << 3;
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2,
        FIELD_AT(r6_3, s32, 0), r5_4, ((*(s16 *)(v4 + 18)) | r8_1) - 16);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  ObjectMotion_CommitCurrentPositionAndActivate(FIELD_AT(r6_3, s32, 0));
  v5 = Func_020030e8(FIELD_AT(r7_1, s16, 0));
  v6 = Func_02003050();
  /* add (s32)stack, #8 */
  return v6;
}
