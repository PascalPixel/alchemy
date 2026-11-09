#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02004464();
s32 Actor_SetSingleChildFlags();
s32 Event_StartScreenTransition();
s32 Func_0200446c();
extern u8 gMainRoutineWork[];

void Func_02001c24(s32 a0) {
  s32 t0;

  t0 = ((s32)Actor_Get(FIELD_AT(gMainRoutineWork, s32, 532)));
  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  if (a0 != 0) {
    Actor_SetSingleChildFlags(
        ((s32)Actor_Get(FIELD_AT(gMainRoutineWork, s32, 532))), 0);
    FieldActor_SetPositionAndResetMotion(t0, (FIELD_AT(t0, s32, 8) + -65536),
                                    FIELD_AT(t0, s32, 12),
                                    (FIELD_AT(t0, s32, 16) + -393216));
    System_Wait(1);
    Event_StartScreenTransition();
    Actor_SetAnimationAndWait(FIELD_AT(gMainRoutineWork, s32, 532), 0x17);
  } else {
    FieldActor_SetPositionAndResetMotion(t0, (FIELD_AT(t0, s32, 8) + 0x10000),
                                    FIELD_AT(t0, s32, 12),
                                    (FIELD_AT(t0, s32, 16) + -393216));
    System_Wait(1);
    Event_StartScreenTransition();
    Actor_SetAnimationAndWait(FIELD_AT(gMainRoutineWork, s32, 532), 0x18);
  }
  Actor_SetSingleChildFlags(
      ((s32)Actor_Get(FIELD_AT(gMainRoutineWork, s32, 532))), 1);
  Actor_SetSpeed(FIELD_AT(gMainRoutineWork, s32, 532), 0x8000,
                           0x4000);
  Actor_SetDestinationOffset(FIELD_AT(gMainRoutineWork, s32, 532), 0, 8);
  FieldActor_WaitForMoveById(FIELD_AT(gMainRoutineWork, s32, 532));
  Actor_SetAnimation(FIELD_AT(gMainRoutineWork, s32, 532), 1);
  Func_0200446c();
}
