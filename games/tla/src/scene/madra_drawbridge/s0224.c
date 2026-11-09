#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_020002b8(void);
void Func_020002c0(void);
void Func_02000338(void);

void Func_02000224(void) {
  Func_020002b8();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgMadraHeardAboutYou);
  Camera_MoveWithinMapBounds(PIXELS(164), -1, PIXELS(260), 1);
  Func_02000338();
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_ShowMessageAndWait(9, 0);
  GameFlag_SetBit(0x916);
  Func_020002c0();
}
