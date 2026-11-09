#include "kibombo.h"
#include "scene_tables.h"

void Func_020032b0(void);
void BattleEffect_ApplyColorToSourceBuffer(s32 a, s32 b);

void Func_02004424(void) {
  struct KibomboEventWork *work;
  s32 effect;

  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Func_020032b0();
  KIBOMBO_TASK_ADD(Func_02002ccc, 1152);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = effect = 513;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Event_SetMessageId((s32)MsgOhTheMagic);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjTurn(17, FACING_SOUTH + FACING_STEP);
  Event_ShowMessageAndWait(0x4011, 0);
  Task_RemoveFar(Func_02002ccc);
  Kibombo_ObjTurn(17, FACING_NORTH + FACING_STEP);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(0x109);
  Import_EventSetExitRequest(12);
}
