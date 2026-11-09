#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const u8 MsgFixingThisWill[];
extern const u8 MsgFortuneTeller2[];
extern const u8 MsgConfirmGraphic2[];
extern const u8 MsgPiers2[];
void Func_020006e4(void);
void Func_020006ec(void);

void Func_02000098(s32 index) {
  Func_020006e4();
  Event_ResolvePendingActions(0);
  if (gMainRoutineWork.entrance == 99)
    Event_SetMessageId(index + (s32)MsgFixingThisWill);
  else if (gMainRoutineWork.entrance == 98)
    Event_SetMessageId(index + (s32)MsgFortuneTeller2);
  else if (gMainRoutineWork.entrance == 97)
    Event_SetMessageId(index + (s32)MsgConfirmGraphic2);
  else
    Event_SetMessageId(index + (s32)MsgPiers2);
  Event_ShowMessageAndWait(index, 0);
  Func_020006ec();
}
