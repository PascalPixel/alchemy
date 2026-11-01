#include "yallam.h"
#include "event_work.h"

/// The bytes of an actor this handler uses. unk63 is the last of the four
/// scene-owned bytes the Broken Seal's actors keep at this place.
struct TalkActor {
  u8 unk00[6];
  u16 angle; ///< 1/65536 turns
  u8 unk08[0x5b];
  u8 unk63;
};

void Func_02000544(void) {
  Func_02002488();
  Func_02002590(0);
  Func_02002520(MsgDoYouWant);
  FieldMessage_ShowAtActor(8, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0)
    Func_02001944();
  else
    Func_02002538(8, 0);
  Func_02002490();
}

void Func_02000584(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = (struct TalkActor *)Actor_GetById(9);
  s16 angle = object->angle;

  Func_02002488();
  Func_02002590(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
    Func_02002520(MsgRunningAroundIn);
  } else {
    Func_02002520(MsgThereSALot);
  }
  Func_02002538(9, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_02002490();
}

void Func_02000614(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = (struct TalkActor *)Actor_GetById(10);
  s16 angle = object->angle;

  Func_02002488();
  Func_02002590(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_SetAngleToward(10, gMainRoutineWork.currentOwner, 0);
    Func_02002520(MsgThereSAGame);
  } else {
    Func_02002520(MsgIOnceHeard);
  }
  Func_02002538(10, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_02002490();
}
