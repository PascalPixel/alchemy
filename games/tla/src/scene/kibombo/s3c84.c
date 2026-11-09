#include "kibombo.h"
#include "scene_tables.h"

void Func_02005a4c(void);
void BattleEffect_StartBufferInterpolation(s32 frames);

void Func_02003c84(void) {
  Sound_Play(212);
  Kibombo_FadeTo(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Kibombo_FadeTo(0x10000, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(60);
  Task_RemoveFar(Func_02002d64);
  wait(1);

  Sound_Play(212);
  Kibombo_FadeTo(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Kibombo_CamMove(146 << 18, -1, PIXELS(248), 0);
  Func_02005a4c();
  wait(1);
  Kibombo_FadeTo(0x10000, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(60);
  Task_RemoveFar(Func_02002d64);
  wait(1);

  Sound_Play(212);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Camera_MoveWithinMapBounds(PIXELS(248), -1, 134 << 18, 0);
  Func_02005a4c();
  wait(1);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(180);
  Task_RemoveFar(Func_02002d64);
  wait(1);

  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 1;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Import_EventSetExitRequest(10);
}
