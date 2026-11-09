#include "types.h"
#include "ram_buffer.h"
#include "flag_call.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

struct JupiterLighthouseRoom5EventWork {
  u8 unk000[0x1c4];
  u16 message;
};

#define EVENT_WORK                                                             \
  ((struct JupiterLighthouseRoom5EventWork *)Ram_HeapSlots->eventWork)

void Func_020028a0(s32 value);
void Func_020028b0(void);
void Func_020028b8(void);
void Func_02000734(void);
void Func_02000810(s32 actor, s32 target);
void Actor_SetSingleChildFlags(void *object, s32 flags);

/// Once flag 0xa24 is set and neither 0xa23 nor 0xa22 is, actor 4 turns
/// about, actor 22 rises in sparks and talks with it, then joins it; flags
/// 0xa25, 0x161 and 0x144 are set.
void Func_020008a4(void) {
  s32 angle;

  if (!Flag_IsSet(0xa24))
    return;
  if (Flag_IsSet(0xa23))
    return;
  if (Flag_IsSet(0xa22))
    return;
  Flag_Set(0xa22);
  Func_020028b0();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgFieldWaitingForCompanion);
  Event_ShowMessageThenWait(22, 0, 5);
  Actor_WalkToAndWait(4, 68, 280);
  Actor_SetPositionAndReset(4, 68, 272);
  Actor_ArmCallbackAndRefresh(4, FACING_WEST);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(20);
  Actor_ArmCallbackAndRefresh(4, FACING_EAST);
  Event_Wait(20);
  Actor_ArmCallbackAndRefresh(4, FACING_NORTH);
  Event_Wait(10);
  Actor_SetSingleChildFlags(((void *)Actor_GetById(22)), 0);
  Actor_SetPartAttribute(((void *)Actor_GetById(22)), 15);
  angle = 0x4000;
  Actor_SetHorizontalPositionAndAngleWithTerrain(22, PIXELS(64), PIXELS(225),
                                                 angle);
  Func_02000734();
  Event_Wait(40);
  Camera_MoveWithinMapBounds(PIXELS(68), -1, PIXELS(240), 1);
  FieldMessage_ShowAtActor(22, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Motion_SetModeAndWaitAnimation(22, 4);
    Event_ShowMessageThenWait(22, 0, 5);
    EVENT_WORK->message++;
  } else {
    Motion_SetModeAndWaitAnimation(22, 3);
    Event_Wait(10);
    EVENT_WORK->message++;
    Event_ShowMessageThenWait(22, 0, 5);
  }
  Motion_SetModeAndWaitAnimation(22, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(22, 0, 5);
  Func_080d47b4(22, 265, 30);
  Event_ShowMessageThenWait(22, 0, 5);
  Motion_SetModeAndWaitAnimation(22, 4);
  Event_ShowMessageThenWait(22, 0, 5);
  Event_ShowMessageThenWait(22, 0, 5);
  Motion_SetModeAndWaitAnimation(22, 3);
  Event_Wait(30);
  Camera_MoveWithinMapBounds(PIXELS(68), -1, PIXELS(256), 1);
  Actor_CommitPositionAndActivate(22, 0, 16);
  Event_Wait(30);
  Func_02000810(22, 4);
  Func_020028a0(1);
  FieldMessage_Show((s16)EVENT_WORK->message++, 5);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(22, 3);
  Event_Wait(30);
  Event_ShowMessageThenWait(22, 0, 5);
  Flag_Set(0xa25);
  Flag_Set(0x161);
  Flag_Set(0x144);
  Func_020028b8();
}
