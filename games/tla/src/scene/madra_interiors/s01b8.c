#include "madra_interiors.h"

void Func_020001b8(s32 actor) {
  u16 facing = Object_GetById(gPartyState.currentOwner)->angle;

  /* FAKEMATCH: the one-pass block keeps the quadrant offset after the facing
   * load; as a plain if the ARM9TDMI schedule builds 0x2000 first (differ 2).
   */
  do {
    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
      Func_02001f00(actor);
    } else {
      if (FlagCheck(0x8ff))
        Func_02001e18((s32)MsgMadraInteriors149);
      else if (FlagCheck(0x8ab))
        Func_02001e18((s32)MsgMadraInteriors81);
      else
        Func_02001e18((s32)MsgMadraInteriors33);
      Func_02001e30(actor, 0);
    }
  } while (0);
}

void Func_0200023c(void) {
  Func_02001d50();
  Func_02001eb8(0);
  ObjectMotion_SetAngleToward(14, 4, 0);
  ObjectMotion_Launch(14, 6, 20);
  ObjectMotion_SetAngleToward(4, 14, 0);
  Func_02001e18((s32)MsgMadraInteriors18);
  Func_02001e30(14, 0);
  ActorTurn(14, 0x2000, 0);
  Func_02001d58();
}

void Func_0200028c(void) {
  s32 msg;

  if (FlagCheck(0x98a)) {
    if (FlagCheck(0x310))
      msg = (s32)MsgMadraInteriors111;
    else
      msg = (s32)MsgMadraInteriors118;
    Func_02001e18(msg);
    Func_02001e30(10, 0);
  } else {
    if (FlagCheck(0x210)) {
      Func_02001e18((s32)MsgMadraInteriors116);
      Func_02001e30(10, 0);
    } else {
      Func_02001e18((s32)MsgMadraInteriors106);
      Func_02001e30(10, 0);
    }
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 346) = 1;
  }
}

void Func_02000308(void) {
  if (FlagCheck(0x98a)) {
    if (FlagCheck(0x310)) {
      Func_02001e18((s32)MsgMadraInteriors112);
      Func_02001e30(11, 0);
    } else {
      Func_02001e18((s32)MsgMadraInteriors119);
      Func_02001e30(11, 0);
    }
  } else if (FlagCheck(0x210)) {
    Func_02001e18((s32)MsgMadraInteriors117);
    Func_02001e30(11, 0);
  } else {
    Func_02001e18((s32)MsgMadraInteriors107);
    Func_02001e30(11, 0);
  }
}

void Func_02000368(void) {
  if (FlagCheck(0x98a)) {
    if (FlagCheck(0x310)) {
      Func_02001e18((s32)MsgMadraInteriors113);
      Func_02001e30(10, 0);
    } else {
      Func_02001e18((s32)MsgMadraInteriors120);
      Func_02001e30(10, 0);
    }
  } else {
    s32 flag = FlagCheck(0x210);
    s32 msg = (s32)MsgMadraInteriors108;

    if (flag) {
      Func_02001e18(msg);
      Func_02001e30(10, 0);
    } else {
      Func_02001e18(msg);
      Func_02001e30(10, 0);
    }
  }
}

void Func_020003c8(void) {
  if (FlagCheck(0x98a)) {
    if (FlagCheck(0x310)) {
      Func_02001e18((s32)MsgMadraInteriors114);
      Func_02001e30(11, 0);
    } else {
      Func_02001e18((s32)MsgMadraInteriors121);
      Func_02001e30(11, 0);
    }
  } else {
    s32 flag = FlagCheck(0x210);
    s32 msg = (s32)MsgMadraInteriors109;

    if (flag) {
      Func_02001e18(msg);
      Func_02001e30(11, 0);
    } else {
      Func_02001e18(msg);
      Func_02001e30(11, 0);
    }
  }
}

void Func_02000428(void) {
  if (!FlagCheck(0x211)) {
    FlagSet(0x211);
    Func_02001e18((s32)MsgUhhhnnnUhhhnnn2);
    ObjectMotion_CommitPositionAndActivate(14, -12, 0);
    FieldEvent_WaitFrames(20);
    Motion_SetVarCbAndRefresh(14, 2);
    FieldEvent_WaitFrames(20);
    Func_02001e30(14, 0);
    Func_02001e48(14, 265, 30);
    FieldEvent_WaitFrames(20);
    ObjectMotion_CommitPositionAndActivate(14, 12, 0);
    ObjectMotion_SetAngleToward(14, 4, 0);
    Func_02001e30(14, 0);
  } else {
    Func_02001e18((s32)MsgMadraInteriors97);
    ObjectMotion_SetAngleToward(14, 4, 0);
    Func_02001e30(14, 0);
  }
}

const u8 *Func_020004c4(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov659Room2)
    return gData_02002c9c;
  if (gPartyState.entrance == 20)
    return gData_02002bb8;
  if (FlagCheck(0x8ff))
    return gData_0200308c;
  if (FlagCheck(0x8ab))
    return gData_02002da4;
  return gData_020028a0;
}

void Func_02000534(void) {
  if (FlagCheck(0x8ab)) {
    ActorPlace(26, 0x3780000, 0x80000);
  } else {
    Func_02001d38(Object_GetById(26), 242);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(26), 0);
  }
}
