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
s32 FieldEvent_Wait();
s32 Actor_SetSpeedParameters();
s32 Actor_ResetAndSetPositionInMode2();
s32 Event_FinishScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Actor_WaitForMoveById();
s32 Func_02003c48();
s32 Func_02003bd0();
extern u8 gMainRoutineWork[];

void Func_02000568(void) {
  Func_02003bc8();
  Call1(Func_02003c88, 0);
  Call1(Func_02003cd8, 0x9e);
  Call6(Func_02003b68, 64, 64, 83, 35, 1, 2);
  Call1(Func_02003b70, 0);
  Call1(FieldEvent_Wait, 45);
  Call3(Actor_SetSpeedParameters, FIELD_AT(gMainRoutineWork, s32, 532),
        0x8000, 0x4000);
  Call3(Actor_ResetAndSetPositionInMode2, FIELD_AT(gMainRoutineWork, s32, 532),
        312, 680);
  Call1(FieldEvent_Wait, 5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call1(Actor_WaitForMoveById,
        FIELD_AT(gMainRoutineWork, s32, 532));
  Call1(Func_02003cd8, 0x7b);
  Call1(Func_02003c48, 4);
  Func_02003bd0();
}
