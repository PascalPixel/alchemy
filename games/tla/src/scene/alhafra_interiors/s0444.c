#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
extern u8 gMainRoutineWork[];
extern u8 MsgAlhafraInteriors08[];

void Func_02000444(s32 a0) {
  Actor_FaceEachOther(a0, FIELD_AT(gMainRoutineWork, s32, 532), 0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors08);
  Event_ShowMessage(a0, 0);
  Actor_FaceDirection(14, 0x3000, 0);
}
