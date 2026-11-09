#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
void Func_02004e5c(void);
void Func_02004e64(void);
extern u8 MsgGaiaRockInside05[];

void Func_02000edc(void) {
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGaiaRockInside05);
  Event_ShowMessage(0xa, 0);
  Func_02004e64();
}
