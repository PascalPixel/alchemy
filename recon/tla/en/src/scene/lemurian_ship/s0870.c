#include "../../../../games/tla/src/scene/lemurian_ship/lemurian_ship.h"

void Actor_WatchAndSetScript(s32 actor, s32 target);

static inline void LemurianShip_SetWork(s32 a, s32 b, s32 c) {
  Func_02002c48(a, b, c);
}

void Func_02000870(void) {
  /* FAKEMATCH(wrapper): Func_02002c48 through LemurianShip_SetWork; as a direct
   * call, 54 of 260 instructions differ in tla-en. */
  struct LemurianShipObject *leader;
  struct LemurianShipObject *helper;
  s32 opened;
  u8 *state;

  if (!GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x900, 131)))) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    Func_02002c68();
    Func_02002da8(0);
    LemurianShip_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    LemurianShip_ObjWalk(gMainRoutineWork.currentOwner, 840, 72);
    opened = GameFlag_Test(0x932);
    state = &leader->field22;
    if (opened) {
      Func_02002de0(243);
      LemurianShip_ObjEmoteWait(gMainRoutineWork.currentOwner, 0x102);
      LemurianShip_SetWork(0, 0x20000, 0x10000);
      FieldEvent_Wait(20);
      LemurianShip_SetWork(0, 0, 0);
      LemurianShip_MapCopy(68, 68, 50, 64, 3, 3);
      *state = 2;
      FieldEvent_Wait(20);
      Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
      LemurianShip_ObjSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      LemurianShip_ObjCommit(gMainRoutineWork.currentOwner, 840, 88);
      FieldEvent_Wait(20);
      Func_02002de0(232);
      LemurianShip_MapCopy(72, 68, 50, 64, 3, 3);
      FieldEvent_Wait(20);
      *state = 1;
    } else {
      helper = Actor_GetById(9);
      Actor_SetSpritePriority(9, 2);
      LemurianShip_ObjSpeed(9, 0x26666, 0x13333);
      Actor_GetById(9)->unk5a[0] &= ~1;
      LemurianShip_ObjWalk(9, 824, 88);
      FieldEvent_Wait(1);
      {
        /* FAKEMATCH(temporary): the bit in a local the byte is or'ed into; as
         * unk5a[0] |= 1, 47 of 260 instructions differ in tla-en. */
        u8 *bits = &Actor_GetById(9)->unk5a[0];
        s32 value = 1;

        value |= *bits;
        *bits = value;
      }
      Func_02002de0(243);
      LemurianShip_ObjEmoteWait(gMainRoutineWork.currentOwner, 0x102);
      LemurianShip_SetWork(0, 0x20000, 0x10000);
      FieldEvent_Wait(20);
      LemurianShip_SetWork(0, 0, 0);
      LemurianShip_MapCopy(68, 68, 50, 64, 3, 3);
      { s32 two = 2;
      *state = two;
      FieldEvent_Wait(20);
      LemurianShip_ObjArm(gMainRoutineWork.currentOwner, 0x6000, 20);
      Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
      LemurianShip_ObjSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      LemurianShip_ObjCommit(gMainRoutineWork.currentOwner, 840, 88);
      LemurianShip_ObjTurn(gMainRoutineWork.currentOwner, 0x8000);
      Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 9);
      helper->field48 = 0x10000;
      helper->field22 = two; }
      Actor_Jump(9, 4, 0);
      LemurianShip_ObjCommit(9, 832, 74);
      Actor_Jump(9, 4, 0);
      LemurianShip_ObjCommit(9, 856, 88);
      LemurianShip_ObjEmoteWait(gMainRoutineWork.currentOwner, 0x102);
      FieldEvent_Wait(20);
      Func_02002de0(232);
      LemurianShip_MapCopy(72, 68, 50, 64, 3, 3);
      FieldEvent_Wait(20);
      Actor_Stop(gMainRoutineWork.currentOwner);
      *state = 1;
      helper->field22 = 0;
      wait(1);
      GameFlag_ClearBit(0x201);
    }
    Func_02002c70();
  }
}
