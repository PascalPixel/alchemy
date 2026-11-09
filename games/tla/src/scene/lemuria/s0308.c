#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_02001bcc(void);
void Func_02001bd4(void);

extern u8 MsgItHasBeen;
extern u8 MsgEveryoneSpoilsMe;
extern u8 MsgWeAreThe;
extern u8 MsgAreThereOther;

/// Actor 18's talk: one message, and Actor_TurnToFaceEachOther with the party leader,
/// while the event work's halfword at 0x166 is 0, another once it is set; the
/// actor's angle is restored after.
void Func_02000308(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_Get(18);
  s16 angle = object->angle;

  object->action = 1;
  Func_02001bcc();
  FieldEvent_ResolvePendingActions(0);
  if (*(s16 *)(work + 0x166) == 0) {
    FieldEvent_SetMessageId((s32)&MsgItHasBeen);
    Actor_FaceEachOther(18, gMainRoutineWork.currentOwner, 2);
  } else {
    FieldEvent_SetMessageId((s32)&MsgEveryoneSpoilsMe);
  }
  Actor_SetAnimation(18, 0);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  Func_02001bd4();
  object->action = 0;
  object->angle = angle;
  System_Wait(1);
}

/// The same for actor 19.
void Func_02000398(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_Get(19);
  s16 angle = object->angle;

  object->action = 1;
  Func_02001bcc();
  FieldEvent_ResolvePendingActions(0);
  if (*(s16 *)(work + 0x166) == 0) {
    FieldEvent_SetMessageId((s32)&MsgWeAreThe);
    Actor_FaceEachOther(19, gMainRoutineWork.currentOwner, 2);
  } else {
    FieldEvent_SetMessageId((s32)&MsgAreThereOther);
  }
  Actor_SetAnimation(19, 0);
  FieldEvent_ShowMessageThenWait(19, 0, 10);
  Func_02001bd4();
  object->action = 0;
  object->angle = angle;
  System_Wait(1);
}
