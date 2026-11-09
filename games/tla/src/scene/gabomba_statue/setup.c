#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "input.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_020022d0(s32 id, s32 x, s32 z, s32 facing);
void Func_020011bc(s32 message, s32 value);
void Func_020056b4(void);
void Func_0200533c(void);
void Func_020057b4(void);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Func_02004d48(s32 a, s32 b);
void Func_0200581c(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02000f50(s32 id) {
  Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  Actor_Stop(id);
  Actor_SetModeById(id, 1);
}

void Func_02000f6c(void) {
  Func_02000f50(11);
  Func_02000f50(12);
  Func_02000f50(13);
  Func_02000f50(14);
  Func_020022d0(11, 11, 9, 0);
  Func_020022d0(12, 11, 9, 0x8000);
  Func_020022d0(13, 10, 10, 0x4000);
  Func_020022d0(14, 10, 10, 0xc000);
}

void Func_02000fc0(void) {
  s32 result;

  GameFlag_SetBit(0x205);
  Func_020056b4();
  Event_ResolvePendingActions(0);
  Sound_Play(78);
  Func_0200533c();
  Event_Wait(40);
  Event_SetMessageId(MsgGabombaAcceptOffering);
  Event_ShowMessageAndWait(24, 0);
  Camera_MoveWithinMapBounds(PIXELS(392), -1, PIXELS(256), 1);
  Func_020057b4();
  Sound_Play(220);
  wait(1);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(40);
  wait(80);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Func_02000f6c();
  wait(1);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(10);
  wait(20);
  Sound_Play(130);
  Actor_SetPartAttribute(Actor_GetById(9), 7);
  Actor_SetPartAttribute(Actor_GetById(10), 7);
  wait(8);
  Actor_SetPartAttribute(Actor_GetById(10), 3);
  Actor_SetPartAttribute(Actor_GetById(9), 0);
  wait(8);
  Sound_Play(11);
  result = Func_02004d48(11, 1);
  Func_0200581c();
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  switch (result) {
  case 3:
    Import_EventSetExitRequest(14);
    break;
  case 1:
    Import_EventSetExitRequest(15);
    break;
  case 2:
    Import_EventSetExitRequest(16);
    break;
  case 0:
    Import_EventSetExitRequest(17);
    break;
  }
}
