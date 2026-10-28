#include "lemurian_ship.h"
#include "scene_tables.h"

void Func_0200143c(void);
void Func_02001944(s32 actor, s32 id);
void Func_0200245c(s32 actor);

/// Restores six of the scene's map edits and actors 16 to 22 from flags 0x938 to
/// 0x93e, then runs the pending step of one of flags 0x228 to 0x22e.
void Func_02002498(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_02002dc8(gData_02003010);
  Actor_GetById(8)->priorityFlags |= 32;
  Actor_SetSpritePriority(8, 1);
  if (LemurianShip_FlagCheck(0x938)) {
    LemurianShip_BlendStart(44, 0, 1, 1, 48, 32);
    LemurianShip_ObjSetPos(Actor_GetById(16), PIXELS(776), 0, PIXELS(520));
    Actor_GetById(16)->height = 0;
    if (!LemurianShip_FlagCheck(0x228))
      LemurianShip_ObjPlace(9, 0, 0);
  } else {
    Func_0200245c(9);
  }
  if (LemurianShip_FlagCheck(0x939)) {
    LemurianShip_BlendStart(44, 0, 1, 1, 53, 32);
    LemurianShip_ObjSetPos(Actor_GetById(17), PIXELS(856), 0, PIXELS(520));
    Actor_GetById(17)->height = 0;
    if (!LemurianShip_FlagCheck(0x229))
      LemurianShip_ObjPlace(10, 0, 0);
  } else {
    Func_0200245c(10);
  }
  if (LemurianShip_FlagCheck(0x93a)) {
    LemurianShip_BlendStart(44, 0, 1, 1, 43, 37);
    LemurianShip_ObjSetPos(Actor_GetById(18), PIXELS(696), 0, PIXELS(600));
    Actor_GetById(18)->height = 0;
    if (!LemurianShip_FlagCheck(0x22a))
      LemurianShip_ObjPlace(11, 0, 0);
  } else {
    Func_0200245c(11);
  }
  if (LemurianShip_FlagCheck(0x93b)) {
    LemurianShip_BlendStart(44, 0, 1, 1, 48, 37);
    LemurianShip_ObjSetPos(Actor_GetById(19), PIXELS(776), 0, PIXELS(600));
    Actor_GetById(19)->height = 0;
    if (!LemurianShip_FlagCheck(0x22b))
      LemurianShip_ObjPlace(12, 0, 0);
  } else {
    Func_0200245c(12);
  }
  if (LemurianShip_FlagCheck(0x93c)) {
    LemurianShip_BlendStart(44, 0, 1, 1, 50, 37);
    LemurianShip_ObjSetPos(Actor_GetById(20), PIXELS(808), 0, PIXELS(600));
    Actor_GetById(20)->height = 0;
    if (!LemurianShip_FlagCheck(0x22c))
      LemurianShip_ObjPlace(13, 0, 0);
  } else {
    Func_0200245c(13);
  }
  if (LemurianShip_FlagCheck(0x93e)) {
    LemurianShip_BlendStart(44, 0, 1, 1, 48, 43);
    LemurianShip_ObjSetPos(Actor_GetById(22), PIXELS(776), 0, PIXELS(696));
    Actor_GetById(22)->height = 0;
    if (!LemurianShip_FlagCheck(0x22e))
      LemurianShip_ObjPlace(15, 0, 0);
  } else {
    Func_0200245c(15);
  }
  Func_02002db8(gData_02003034);
  LemurianShip_TaskAdd(Func_0200143c, 0x480);
  if (LemurianShip_FlagCheck(0x228)) {
    Func_02001944(9, 16);
    GameFlag_ClearBit(0x228);
  } else if (LemurianShip_FlagCheck(0x229)) {
    Func_02001944(10, 17);
    GameFlag_ClearBit(0x229);
  } else if (LemurianShip_FlagCheck(0x22a)) {
    Func_02001944(11, 18);
    GameFlag_ClearBit(0x22a);
  } else if (LemurianShip_FlagCheck(0x22b)) {
    Func_02001944(12, 19);
    GameFlag_ClearBit(0x22b);
  } else if (LemurianShip_FlagCheck(0x22c)) {
    Func_02001944(13, 20);
    GameFlag_ClearBit(0x22c);
  } else if (LemurianShip_FlagCheck(0x22e)) {
    Func_02001944(15, 22);
    GameFlag_ClearBit(0x22e);
  }
}
