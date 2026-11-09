#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
s32 Func_02000988();
void Func_02000a40();
void Func_02000990();
extern char SceneId_SuhallaGate2;
extern char SceneId_VenusLighthouseEntrance;
extern u8 MsgHaThatWas[];

void Func_02000494(s32 p0) {
  struct MainRoutineWork *work;
  s32 *owner;

  Func_02000988(p0);
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgHaThatWas);
  work = &gMainRoutineWork;
  owner = &work->currentOwner;
  Actor_WalkByAndWait(*owner, 0, -4);
  FieldEvent_Wait(30);
  Actor_GetById(*owner)->actionFlags &= 254;
  Actor_WalkByAndWait(*owner, 0, 12);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(*owner, 0, 16);
  FieldEvent_Wait(30);
  Actor_SetPosition(13, 0x2680000, 0x3900000);
  Actor_WalkToAndWait(13, 0x258, 0x398);
  Actor_SetPosition(15, 0x2680000, 0x3900000);
  Actor_FaceDirection(13, 0x4000, 0);
  Actor_WalkToAndWait(15, 0x278, 0x398);
  Actor_FaceDirection(15, 0x4000, 0);
  Actor_SetPosition(14, 0x2680000, 0x3900000);
  Actor_WalkToAndWait(14, 0x268, 0x398);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(13, 0, 10);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_SetAnimationAndWait(15, 3);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_SetAnimationAndWait(13, 3);
  Actor_SetAnimation(14, 3);
  Actor_SetAnimationAndWait(15, 3);
  Event_SetAfterBattleDestination((s32)&SceneId_SuhallaGate2, 99);
  Event_SetDefeatDestination((s32)&SceneId_VenusLighthouseEntrance, 98);
  *((u8 *)work + 0x24b) = 2;
  Func_02000a40(9, 2);
  Func_02000990();
}
