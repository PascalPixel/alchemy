#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "main_routine_work.h"
#include "event_inline.h"

/// The bytes of an actor these handlers use. unk63 is the last of the four
/// scene-owned bytes the Broken Seal's actors keep at this place.
struct TalkActor {
  u8 unk00[6];
  u16 angle; ///< 1/65536 turns
  u8 unk08[0x5b];
  u8 unk63; ///< Func_020002d8 reads it
};

void Func_02004464(void);
void Func_0200446c(void);

extern u8 MsgProx113[];
extern u8 MsgProx114[];
extern u8 MsgProx115[];
extern u8 MsgProx117[];
extern u8 MsgProx118[];
extern u8 MsgProx119[];

void Func_02000764(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_Get(16));
  s16 angle = object->angle;

  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_FaceActor(16, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)MsgProx113);
  } else {
    FieldEvent_SetMessageId((s32)MsgProx117);
  }
  Event_ShowMessage(16, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_0200446c();
}

void Func_020007f4(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_Get(17));
  s16 angle = object->angle;

  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_FaceActor(17, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)MsgProx114);
  } else {
    FieldEvent_SetMessageId((s32)MsgProx118);
  }
  Event_ShowMessage(17, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_0200446c();
}

void Func_02000884(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_Get(18));
  s16 angle = object->angle;

  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_FaceActor(18, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)MsgProx115);
  } else {
    FieldEvent_SetMessageId((s32)MsgProx119);
  }
  Event_ShowMessage(18, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_0200446c();
}
