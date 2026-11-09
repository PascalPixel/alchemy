#include "types.h"
#include "event_inline.h"

extern char MsgLoho32;
extern char MsgLoho36;
extern char MsgLoho55;
extern char MsgLoho56;

void Func_02000bf4(void) {
  if (GameFlag_Test(0x90a))
    Event_SetMessageId((s32)&MsgLoho32);
  else
    Event_SetMessageId((s32)&MsgLoho55);
  Event_ShowMessageAndWait(11, 0);
}

void Func_02000c24(void) {
  if (GameFlag_Test(0x90a))
    Event_SetMessageId((s32)&MsgLoho36);
  else
    Event_SetMessageId((s32)&MsgLoho56);
  Event_ShowMessageAndWait(11, 0);
}
