#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "event_inline.h"
s32 Func_02001b1c();
s32 Func_02001a7c();
s32 Func_02001a84();
extern u8 gMainRoutineWork[];

void Func_020000f4(void) {
  s32 t0;
  s32 t2;

  t0 = Actor_Get(FIELD_AT(gMainRoutineWork, s32, 532));
  Call7(Func_02001b1c, 0x38, 0, 3, 3, 0x4f, 0x19, 1);
  t2 = Event_GetFlag(0xfe2);
  if (t2 == 0) {
    Actor_SetPosition(0x40, 0x1180000, 0x1b80000);
    if ((FIELD_AT(t0, s32, 8) >> 20) == 0x11) {
      if ((FIELD_AT(t0, s32, 16) >> 20) == 0x1b) {
        Func_02001a7c();
        FieldEvent_ResolvePendingActions(0);
        Actor_SetAttachedEffect(FIELD_AT(gMainRoutineWork, s32, 532), 0x102);
        Actor_SetDestinationOffset(FIELD_AT(gMainRoutineWork, s32, 532), -16, 0);
        FIELD_AT(t0, s32, 40) = 0x60000;
        FieldActor_WaitForMoveById(FIELD_AT(gMainRoutineWork, s32, 532));
        Func_02001a84();
      }
    }
  }
}
