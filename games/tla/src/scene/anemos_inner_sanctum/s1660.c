#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02003398();
s32 Func_020033a0();
extern u8 gMainRoutineWork[];

void Func_02001660(void) {
  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(FIELD_AT(gMainRoutineWork, s32, 532), 536, 360);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(50, 22, 33, 22, 1, 1);
  Actor_SetPosition(14, 0, 0);
  Event_PlaySound(0x248);
  FieldEvent_Wait(30);
  Event_PlaySound(0xbc);
  MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  MapGrid_CopyCellsTo(51, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  Event_SetFlag(0x200);
  Func_020033a0();
}
