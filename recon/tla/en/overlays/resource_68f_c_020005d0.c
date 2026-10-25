// Draft, differ 82 of 105: translated from its listing; names and structure still
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

s32 Func_020005d0(void) {
  s32 r5, r7, r6, r2, r3, r8, v0, v1, v2, v3, v4, v5, v6, r0;
  s32 stack[2];

  r5 = (s32)Ram_HeapSlots->eventWork;
  /* sub (s32)stack, #8 */
  Func_02003048();
  Call1((void (*)())Func_02003148, 0);
  r7 = r5 + 340;
  r6 = 37;
  r2 = 0;
  r3 = FIELD_AT(r7, s16, 0);
  if ((FIELD_AT(r7, s16, 0)) != 3)
    goto L_020085fa;
  r6 = 25;
L_020085fa:
  Call1((void (*)())Func_020031a8, 158);
  stack[0] = 1;
  r8 = 1;
  r5 = 2;
  Call6((void (*)())Func_02002fc0, 30, 38, r6, 49, 1, r5);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call6((void (*)())Func_02002fc0, 32, 38, r6, 49, r8, r5);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  r6 = (s32)&gPartyState;
  r6 = r6 + 532;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, FIELD_AT(r6, s32, 0),
        0x8000, 0x4000);
  v0 = Object_GetById(FIELD_AT(r6, s32, 0));
  FIELD_AT(v0, u8, 85) = 0;
  v1 = Object_GetById(FIELD_AT(r6, s32, 0));
  r5 = FIELD_AT(v1, s32, 8);
  r5 = r5 >> 19;
  r5 = r5 | r8;
  v2 = Object_GetById(FIELD_AT(r6, s32, 0));
  r5 = r5 << 3;
  Call3((void (*)())ObjectMotion_SetPositionAndReset, FIELD_AT(r6, s32, 0), r5,
        (*(s16 *)(v2 + 18)) | r8);
  Call1((void (*)())Func_020031a8, 123);
  v3 = Object_GetById(FIELD_AT(r6, s32, 0));
  r5 = FIELD_AT(v3, s32, 8);
  v4 = Object_GetById(FIELD_AT(r6, s32, 0));
  r5 = r5 >> 19;
  r5 = r5 << 3;
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2,
        FIELD_AT(r6, s32, 0), r5, ((*(s16 *)(v4 + 18)) | r8) - 16);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate,
        FIELD_AT(r6, s32, 0));
  v5 = Func_020030e8(FIELD_AT(r7, s16, 0));
  v6 = Func_02003050();
  /* add (s32)stack, #8 */
  return v6;
}
