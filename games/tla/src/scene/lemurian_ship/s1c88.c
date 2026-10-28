#include "lemurian_ship.h"

void Func_02001944(s32 actor, s32 target);

/// Places actors 10 and 11 as flags 0x930 and 0x931 say, then runs the pending
/// step of flag 0x220 or 0x221.
void Func_02001c88(void) {
  s32 flag;

  if (LemurianShip_FlagCheck(0x930)) {
    flag = LemurianShip_FlagCheck(0x220);
    if (flag != 0) {
      LemurianShip_BlendStart(0, 25, 1, 3, 6, 25);
      LemurianShip_ObjSetPos(Actor_GetById(10), 0x680000, 0, 0x1a80000);
      Actor_GetById(10)->height = 0;
    } else {
      LemurianShip_ObjPlace(8, 0, 0);
      LemurianShip_BlendStart(0, 25, 1, 3, 6, 25);
      LemurianShip_ObjSetPos(Actor_GetById(10), 0x680000, 0, 0x1a80000);
      Actor_GetById(10)->height = 0;
    }
    wait_(1);
  }
  if (LemurianShip_FlagCheck(0x931)) {
    if (LemurianShip_FlagCheck(0x221))
      Actor_SetSpritePriority(9, 1);
    else
      LemurianShip_ObjPlace(9, 0, 0);
    LemurianShip_ObjSetPos(Actor_GetById(11), 0x680000, 0xffe00000, 0x2180000);
    Actor_GetById(11)->height = 0xffe00000;
    Actor_SetSpritePriority(11, 1);
    wait_(1);
  }
  Func_02002db8(gData_02003014);
  if (LemurianShip_FlagCheck(0x220)) {
    Func_02001944(8, 10);
    GameFlag_ClearBit(0x220);
  } else if (LemurianShip_FlagCheck(0x221)) {
    Func_02001944(9, 11);
    GameFlag_ClearBit(0x221);
  }
}
