#include "types.h"
#include "scene_tables.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
extern u8 MsgAlhafra50[];

void Func_02000288(void) {
  Event_SetMessageId((s32)MsgAlhafra50);
  FieldEvent_ShowMessageThenWait(0x18, 0, 0xa);
  Actor_ArmCallback(19, -256, 0);
  Event_Wait(10);
  Func_080d47b4(0x13, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x13, 0, 0xa);
  FieldActor_StartRepeatedMotion(24, 3);
  Func_080d489c(0x18, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(19, FACING_WEST, 0);
  FieldEvent_ShowMessageThenWait(0x18, 0, 0);
}
