#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02003bc8();
s32 Func_02003c88();
s32 Func_02003cd8();
s32 Func_02003b68();
s32 Func_02003b70();
s32 FieldEvent_WaitTicks();
s32 ObjectMotion_SetSpeedParameters();
s32 ObjectMotion_ResetAndSetPositionInMode2();
s32 Event_FinishScreenTransition();
s32 Event_WaitTransitionFrames();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 Func_02003c48();
s32 Func_02003bd0();
extern u8 gPartyState[];

void Func_02000568(void) {
  Func_02003bc8();
  Call1(Func_02003c88, 0);
  Call1(Func_02003cd8, 0x9e);
  Call6(Func_02003b68, 0x40, 0x40, 0x53, 0x23, 1, 2);
  Call1(Func_02003b70, 0);
  Call1(FieldEvent_WaitTicks, 0x2d);
  Call3(ObjectMotion_SetSpeedParameters, FIELD_AT(gPartyState, s32, 532),
        0x8000, 0x4000);
  Call3(ObjectMotion_ResetAndSetPositionInMode2,
        FIELD_AT(gPartyState, s32, 532), 0x138, 0x2a8);
  Call1(FieldEvent_WaitTicks, 5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call1(ObjectMotion_CommitCurrentPositionAndActivate,
        FIELD_AT(gPartyState, s32, 532));
  Call1(Func_02003cd8, 0x7b);
  Call1(Func_02003c48, 4);
  Func_02003bd0();
}
