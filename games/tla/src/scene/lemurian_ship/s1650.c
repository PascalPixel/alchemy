#include "lemurian_ship.h"
#include "event_inline.h"
#include "scene_tables.h"

extern const u8 gData_02002f1c[];
extern const u8 gData_02002f44[];
extern const u8 gData_02002f88[];
extern const u8 gData_02002fcc[];

void Func_020015c8(void);
void PartyInventory_Remove(s32 item);

/// The scene that clears the ship and sets its flags.
/// "How did all these monsters get on board?"
void Func_02001650(void) {
  Func_02002c68();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemurianShip01);
  Sound_Play(78);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 808, 208);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(176), 1);
  Func_02002cd8(7, gMainRoutineWork.currentOwner);
  Func_02002cd8(6, gMainRoutineWork.currentOwner);
  Func_02002cd8(5, gMainRoutineWork.currentOwner);
  Func_02002cd8(8, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(6, gData_02002f44);
  Actor_SetMovementTypeById(5, gData_02002f88);
  Actor_SetMovementTypeById(8, gData_02002fcc);
  Actor_SetMovementTypeAndWaitById(7, gData_02002f1c);
  Event_Wait(20);
  Event_ShowMessageAndWait(7, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTH);
  Event_ShowMessageAndWait(7, 0);
  Actor_SetModeById(7, 3);
  Event_ShowMessageAndWait(7, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_WalkToAndWait(7, 808, 148);
  Motion_SetVarCbAndRefresh(7, 2);
  Actor_SetSpritePriority(7, 1);
  Sound_Play(220);
  Task_Add(Func_020015c8, 0x480);
  Event_Wait(60);
  Sound_Play(390);
  Event_Wait(20);
  Task_RemoveFar(Func_020015c8);
  Sound_Play(251);
  Field_SetCameraBounds(0x2b00000, 0, 0x3a00000, 0x2200000);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(448), 0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(808), PIXELS(480));
  Actor_SetPosition(7, PIXELS(808), PIXELS(420));
  Actor_SetPosition(6, PIXELS(784), PIXELS(468));
  Actor_SetPosition(5, PIXELS(832), PIXELS(468));
  Actor_SetHorizontalPositionWithTerrain(8, PIXELS(792), PIXELS(488));
  Func_02002c10();
  Event_Wait(20);
  Sound_Play(246);
  Event_Wait(80);
  ((struct LemurianShipEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      513;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  PartyInventory_Remove(242);
  GameFlag_SetBit(0x983);
  GameFlag_SetBit(0x930);
  GameFlag_SetBit(0x931);
  GameFlag_SetBit(0x933);
  GameFlag_SetBit(0x934);
  GameFlag_SetBit(0x935);
  GameFlag_SetBit(0x936);
  GameFlag_SetBit(0x937);
  GameFlag_SetBit(0x938);
  GameFlag_SetBit(0x939);
  GameFlag_SetBit(0x93a);
  GameFlag_SetBit(0x93b);
  GameFlag_SetBit(0x93c);
  GameFlag_SetBit(0x93e);
  GameFlag_SetBit(0x932);
  GameFlag_SetBit(0x93f);
  gMainRoutineWork.ownerOverride = 1;
  Import_EventSetExitRequest(90);
}
