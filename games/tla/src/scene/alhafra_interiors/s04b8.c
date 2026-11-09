#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
extern u8 MsgAlhafraInteriors55[];

void Func_020004b8(s32 a0) {
  GameFlag_SetBit(0x8b8);
  Event_SetMessageId((s32)MsgAlhafraInteriors55);
  Event_ShowMessage(a0, 0);
  Event_Wait(10);
  Actor_ShowEmote(a0, 0x108, 0x1e);
  Event_ShowMessageAndWait(a0, 0);
}
