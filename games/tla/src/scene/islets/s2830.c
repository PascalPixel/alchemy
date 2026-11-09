#include "islets.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02002830(void) {
  struct IsletsEventWork *work;
  struct Actor *owner = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_Get(9);

  Func_02003eec(9, gMainRoutineWork.currentOwner);
  Actor_SetPosition(10, PIXELS(273), PIXELS(520));
  System_Wait(1);
  object->angle = FACING_NORTH;
  owner->update = (void (*)(void))Func_02001278;
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetAnimation(9, 3);
  System_Wait(1);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(9, 0x9999, 0x4ccc);
  Actor_SetSpeed(10, 0x9999, 0x4ccc);
  Actor_Get(10)->update = (void (*)(void))Func_02000df8;
  object->update = (void (*)(void))Func_02000df8;
  Actor_WalkTo(10, 280, 468);
  Actor_MoveToAndWait(9, 296, 452);
  Actor_SetAnimation(10, 1);
  owner->update = 0;
  object->update = 0;
  Actor_Get(10)->update = 0;
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.currentOwner, 9);
  FieldEvent_SetMessageId((s32)MsgHereWeAre);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(9, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 0);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 312, 440);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTHWEST);
  Event_ShowMessage(9, 0);
  Func_02003fb4();
  Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x100;
  Func_020034d8();
}
