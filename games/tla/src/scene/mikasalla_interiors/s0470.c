#include "types.h"
#include "event_inline.h"

extern u8 MsgMikasallaInteriors20[];
extern u8 MsgMikasallaInteriors21[];

void Func_020004dc(void);

void Func_02000470(void) {
  if (Event_GetFlag(0xa39)) {
    FieldEvent_SetMessageId(Event_GetFlag(0x340) ? (s32)MsgMikasallaInteriors20
                                            : (s32)MsgMikasallaInteriors21);
    Event_ShowMessage(18, 0);
  } else {
    Func_020004dc();
  }
}
