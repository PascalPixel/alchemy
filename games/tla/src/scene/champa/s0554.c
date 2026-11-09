#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
extern u8 MsgChampa35[];
extern u8 MsgChampa36[];

void Func_02000554(s32 a0) {
  s32 t0;

  t0 = Event_GetFlag(0x972);
  if (t0 == 0) {
    Event_SetFlag(0x972);
    FieldEvent_SetMessageId((s32)MsgChampa35);
  } else {
    FieldEvent_SetMessageId((s32)MsgChampa36);
  }
  Event_ShowMessage(a0, 0);
}
