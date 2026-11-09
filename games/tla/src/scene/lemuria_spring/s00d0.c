#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "scene_tables.h"
#include "event_inline.h"
s32 Func_02000070();
s32 Func_02001380();
s32 Func_020001e4();
s32 Func_02001388();

void Func_020000d0(void) {
  s32 t0;
  s32 actor = FIELD_AT(&gMainRoutineWork, s32, 532);

  t0 = Event_GetFlag(0x200);
  if (t0 == 0) {
    Event_SetFlag(0x200);
    Func_02000070();
  }
  Func_02001380();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(actor, 120, 152);
  Actor_FaceDirection(actor, FACING_SOUTH, 0);
  Func_020001e4();
  Func_02001388();
}
