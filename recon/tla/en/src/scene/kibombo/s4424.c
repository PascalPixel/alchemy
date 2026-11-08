#include "../../../../../../games/tla/src/scene/kibombo/kibombo.h"
#include "scene_tables.h"

void Func_020032b0(void);
void Func_02005bdc(s32 a, s32 b);

void Func_02004424(void) {
  struct KibomboEventWork *work;
  s32 effect;

  Func_02005a9c();
  Func_02005c3c(0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Func_020032b0();
  KIBOMBO_TASK_ADD(Func_02002ccc, 1152);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = effect = 513;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Func_02005b4c((s32)MsgOhTheMagic);
  Func_02005b64(17, 0);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjTurn(17, FACING_SOUTH + FACING_STEP);
  Func_02005b64(SPLIT_CONSTANT(SPLIT_CONSTANT(0x4000, 17)), 0);
  Task_RemoveFar(Func_02002ccc);
  Kibombo_ObjTurn(17, FACING_NORTH + FACING_STEP);
  Func_02005bdc(0x10000, 0);
  Func_02005bd4(0, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(0x109);
  Func_02005bc4(12);
}
