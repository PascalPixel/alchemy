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
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgNotThatWayThe);
  FieldActor_LaunchFromLeader(8, 0x10, 0, 0x8000);
  FieldActor_WaitForMoveById(8);
  FieldEvent_ShowMessageThenWait(0x2008, 0, 0xa);
  Actor_SetAnimation(8, 2);
  t7 = ((s32)Actor_Get(5));
  if (t7 != 0) {
    Actor_SetDestination(8, FIELD_AT(t7, s16, 10), FIELD_AT(t7, s16, 18));
  }
  FieldActor_WaitForMoveById(8);
  Actor_SetPosition(8, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(5, -16, 0);
  Func_020028bc();
}
