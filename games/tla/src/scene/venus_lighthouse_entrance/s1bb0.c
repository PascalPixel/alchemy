#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
void Func_020028b4(void);
void Func_020028bc(void);
extern u8 MsgNotThatWayThe[];

void Func_02001bb0(void) {
  s32 t7;

  Func_020028b4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNotThatWayThe);
  Func_080d3070(8, 0x10, 0, 0x8000);
  Actor_WaitForMoveById(8);
  Event_ShowMessageThenWait(0x2008, 0, 0xa);
  Actor_SetModeById(8, 2);
  t7 = ((s32)Actor_GetById(5));
  if (t7 != 0) {
    Actor_ResetAndSetPosition(8, FIELD_AT(t7, s16, 10), FIELD_AT(t7, s16, 18));
  }
  Actor_WaitForMoveById(8);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Event_Wait(10);
  Actor_WalkByAndWait(5, -16, 0);
  Func_020028bc();
}
