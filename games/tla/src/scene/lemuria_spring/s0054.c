#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02001358();
extern u8 MsgAndTheOthers[];

void Func_02000054(void) {
  Message_ShowCentered((s32)MsgAndTheOthers, 1);
  Event_PlaySound(0x7e);
  Func_02001358(1);
}
