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
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgFieldWaitingForCompanion);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Actor_WalkToAndWait(4, 68, 280);
  Actor_WalkToAndWait(4, 68, 272);
  Actor_FaceDirectionAndWait(4, FACING_WEST);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_SetSingleChildFlags(((void *)Actor_Get(22)), 0);
  Actor_SetSpriteColor(((void *)Actor_Get(22)), 15);
  angle = 0x4000;
  Actor_SetPositionAndFacing(22, PIXELS(64), PIXELS(225),
                                                 angle);
  Func_02000734();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(PIXELS(68), -1, PIXELS(240), 1);
  Event_OpenMessage(22, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    Actor_SetAnimationAndWait(22, 4);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    EVENT_WORK->message++;
  } else {
    Actor_SetAnimationAndWait(22, 3);
    FieldEvent_Wait(10);
    EVENT_WORK->message++;
    FieldEvent_ShowMessageThenWait(22, 0, 5);
  }
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Actor_ShowEmote(22, 265, 30);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Actor_SetAnimationAndWait(22, 4);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(PIXELS(68), -1, PIXELS(256), 1);
  Actor_WalkByAndWait(22, 0, 16);
  FieldEvent_Wait(30);
  Func_02000810(22, 4);
  Func_020028a0(1);
  Message_ShowCentered((s16)EVENT_WORK->message++, 5);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Flag_Set(0xa25);
  Flag_Set(0x161);
  Flag_Set(0x144);
  Func_020028b8();
}
