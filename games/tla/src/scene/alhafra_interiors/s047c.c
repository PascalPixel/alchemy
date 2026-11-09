#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
extern u8 gMainRoutineWork[];
extern u8 MsgAlhafraInteriors48[];

void Func_0200047c(s32 a0, s32 a1) {
  Actor_TurnToFaceEachOther(a1, FIELD_AT(gMainRoutineWork, s32, 532), 0);
  Event_SetMessageId((s32)MsgAlhafraInteriors48);
  Event_ShowMessageAndWait(a1, 0);
  Actor_FaceDirection(a1, 0xe000, 0);
}
