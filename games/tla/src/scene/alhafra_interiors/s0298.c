#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
extern u8 MsgAlhafraInteriors06[];

void Func_02000298(s32 a0) {
  Func_080d47b4(a0, 0x107, 0x1e);
  Event_SetMessageId((s32)MsgAlhafraInteriors06);
  Event_ShowMessageAndWait(a0, 0);
}
