#include "lemurian_ship.h"

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
  Func_02002da8(0);
  Func_02002d18((s32)MsgLemurianShip01);
  LemurianShip_SePlay(78);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  LemurianShip_ObjSpeed(gSequenceParentWork.currentOwner, 0xcccc, 0x6666);
  LemurianShip_ObjWalk(gSequenceParentWork.currentOwner, 808, 208);
  LemurianShip_CamSpeed(0xcccc, 0x1999);
  LemurianShip_CamMove(0x3280000, -1, 0xb00000, 1);
  Func_02002cd8(7, gSequenceParentWork.currentOwner);
  Func_02002cd8(6, gSequenceParentWork.currentOwner);
  Func_02002cd8(5, gSequenceParentWork.currentOwner);
  Func_02002cd8(8, gSequenceParentWork.currentOwner);
  wait_(1);
  LemurianShip_ObjSpeed(7, 0xcccc, 0x6666);
  LemurianShip_ObjSpeed(6, 0xcccc, 0x6666);
  LemurianShip_ObjSpeed(5, 0xcccc, 0x6666);
  LemurianShip_ObjSpeed(8, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(6, gData_02002f44);
  Actor_SetMovementTypeById(5, gData_02002f88);
  Actor_SetMovementTypeById(8, gData_02002fcc);
  Actor_SetMovementTypeAndWaitById(7, gData_02002f1c);
  LemurianShip_Wait(20);
  LemurianShip_MessagePut(7, 0);
  LemurianShip_ObjTurn(7, 0x4000);
  LemurianShip_MessagePut(7, 0);
  Actor_SetModeById(7, 3);
  LemurianShip_MessagePut(7, 0);
  Motion_SetModeAndWaitAnimation(gSequenceParentWork.currentOwner, 3);
  LemurianShip_ObjWalk(7, 808, 148);
  Motion_SetVarCbAndRefresh(7, 2);
  Actor_SetSpritePriority(7, 1);
  LemurianShip_SePlay(220);
  Task_Add(Func_020015c8, 0x480);
  LemurianShip_Wait(60);
  LemurianShip_SePlay(390);
  LemurianShip_Wait(20);
  Task_RemoveFar(Func_020015c8);
  LemurianShip_SePlay(251);
  LemurianShip_CamSet(0x2b00000, 0, 0x3a00000, 0x2200000);
  LemurianShip_CamMove(0x3280000, -1, 0x1c00000, 0);
  LemurianShip_ObjPlace(gSequenceParentWork.currentOwner, 0x3280000, 0x1e00000);
  LemurianShip_ObjPlace(7, 0x3280000, 0x1a40000);
  LemurianShip_ObjPlace(6, 0x3100000, 0x1d40000);
  LemurianShip_ObjPlace(5, 0x3400000, 0x1d40000);
  LemurianShip_ObjPlace(8, 0x3180000, 0x1e80000);
  Func_02002c10();
  LemurianShip_Wait(20);
  LemurianShip_SePlay(246);
  LemurianShip_Wait(80);
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
  gSequenceParentWork.ownerOverride = 1;
  Func_02002d70(90);
}
