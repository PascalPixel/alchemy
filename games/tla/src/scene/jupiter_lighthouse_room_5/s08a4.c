#include "types.h"
#include "ram_buffer.h"
#include "call.h"
#include "flag_call.h"
#include "text/msg_ids.h"

struct JupiterLighthouseRoom5EventWork {
  u8 unk000[0x1c4];
  u16 message;
};

#define EVENT_WORK ((struct JupiterLighthouseRoom5EventWork *)Ram_HeapSlots->eventWork)

void Func_020028a0(s32 value);
void Func_020028b0(void);
void Func_020028b8(void);
void Func_020029a8(s32 value);
void Func_02002928(void *message);
void Func_02002938(s32 actor, s32 a, s32 b);
void Func_02002940(s32 actor, s32 angle);
void Func_02002900(s32 actor, s32 x, s32 z, s32 angle);
s32 Func_02002950(s32 actor, s32 a, s32 b);
void Func_02000734(void);
void Func_02000810(s32 actor, s32 target);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void FieldEvent_WaitFrames(s32 frames);
void *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(void *object, s32 flags);
void Object_SetPartAttribute(void *object, s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
s32 UiText_OpenMessageAtObject(s32 actor, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);

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
  Func_020029a8(0);
  Func_02002928(MsgFieldWaitingForCompanion);
  Func_02002938(22, 0, 5);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, 68, 280);
  ObjectMotion_SetPositionAndReset(4, 68, 272);
  Call2((void (*)())Func_02002940, 4, 0x8000);
  FieldEvent_WaitFrames(20);
  Call2((void (*)())Func_02002940, 4, 0xc000);
  FieldEvent_WaitFrames(20);
  Call2((void (*)())Func_02002940, 4, 0);
  FieldEvent_WaitFrames(20);
  Call2((void (*)())Func_02002940, 4, 0xc000);
  FieldEvent_WaitFrames(10);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(22), 0);
  Object_SetPartAttribute(Object_GetById(22), 15);
  angle = 0x4000;
  Func_02002900(22, 0x400000, 0xe10000, angle);
  Func_02000734();
  FieldEvent_WaitFrames(40);
  Camera_MoveWithinMapBounds(0x440000, -1, 0xf00000, 1);
  Value2((s32 (*)())UiText_OpenMessageAtObject, 22, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Motion_SetModeAndWaitAnimation(22, 4);
    Func_02002938(22, 0, 5);
    EVENT_WORK->message++;
  } else {
    Motion_SetModeAndWaitAnimation(22, 3);
    FieldEvent_WaitFrames(10);
    EVENT_WORK->message++;
    Func_02002938(22, 0, 5);
  }
  Motion_SetModeAndWaitAnimation(22, 3);
  FieldEvent_WaitFrames(10);
  Func_02002938(22, 0, 5);
  Value3((s32 (*)())Func_02002950, 22, 265, 30);
  Func_02002938(22, 0, 5);
  Motion_SetModeAndWaitAnimation(22, 4);
  Func_02002938(22, 0, 5);
  Func_02002938(22, 0, 5);
  Motion_SetModeAndWaitAnimation(22, 3);
  FieldEvent_WaitFrames(30);
  Camera_MoveWithinMapBounds(0x440000, -1, 0x1000000, 1);
  ObjectMotion_CommitPositionAndActivate(22, 0, 16);
  FieldEvent_WaitFrames(30);
  Func_02000810(22, 4);
  Func_020028a0(1);
  UiText_ShowPositionedMessageAndWait((s16)EVENT_WORK->message++, 5);
  FieldEvent_WaitFrames(30);
  Motion_SetModeAndWaitAnimation(22, 3);
  FieldEvent_WaitFrames(30);
  Func_02002938(22, 0, 5);
  Flag_Set(0xa25);
  Flag_Set(0x161);
  Flag_Set(0x144);
  Func_020028b8();
}
