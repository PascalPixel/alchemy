#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02003bc8();
s32 Func_02003b70();
s32 Event_FinishScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_02003bd0();
extern u8 gMainRoutineWork[];

void Func_02000568(void) {
  Func_02003bc8();
  Event_ResolvePendingActions(0);
  Sound_Play(0x9e);
  MapGrid_CopyCellsTo(64, 64, 83, 35, 1, 2);
  Func_02003b70(0);
  Event_Wait(45);
  Actor_SetSpeedParameters(FIELD_AT(gMainRoutineWork, s32, 532), 0x8000,
                           0x4000);
  Actor_ResetAndSetPositionInMode2(FIELD_AT(gMainRoutineWork, s32, 532), 312,
                                   680);
  Event_Wait(5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WaitForMoveById(FIELD_AT(gMainRoutineWork, s32, 532));
  Sound_Play(0x7b);
  Import_EventSetExitRequest(4);
  Func_02003bd0();
}
