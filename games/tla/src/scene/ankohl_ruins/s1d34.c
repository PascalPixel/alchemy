#include "ankohl_ruins.h"

void Func_02001d34(void) {
  s32 done = AnkohlRuins_FlagCheck(0x202);

  if (done == 0) {
    Event_ResolvePendingActions(0);
    Sound_Play(584);
    Event_Wait(15);
    Sound_Play(202);
    Map_CopyCellsRect(18, 66, 3, 1, 7, 66);
    AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
    Event_Wait(15);
    gData_02003780 = 7;
    gData_02003782 = done;
    gData_02003784 = 200;
    gData_02003788 = 0x8000;
    gData_0200378c = 1;
    GameFlag_SetBit(381);
    Func_0200160c();
    GameFlag_SetBit(0x202);
  }
}

void Func_02001ddc(void) {
  if (AnkohlRuins_FlagCheck(0x9e2) == 0) {
    GameFlag_SetBit(0x9e2);
    Sound_Play(584);
    Func_02002e44();
    Event_ResolvePendingActions(0);
    Map_CopyCellsRect(26, 70, 1, 3, 14, 65);
    Event_Wait(20);
    Sound_Play(181);
    Map_CopyCellsRect(26, 67, 1, 3, 14, 65);
    Map_CopyCellAttributeRect(15, 5, 1, 1, 14, 5);
    Func_02002e4c();
  }
}

void Func_02001e54(void) {
  s32 done = AnkohlRuins_FlagCheck(0x203);

  if (done == 0) {
    Event_ResolvePendingActions(0);
    Sound_Play(584);
    Event_Wait(15);
    Sound_Play(202);
    AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 84);
    AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
    Event_Wait(15);
    gData_02003780 = 3;
    gData_02003782 = done;
    gData_02003784 = 100;
    gData_02003788 = 0x8000;
    gData_0200378c = 2;
    GameFlag_SetBit(381);
    Func_020019a0();
    GameFlag_SetBit(0x203);
  }
}

void Func_02001f04(void) {
  if (AnkohlRuins_FlagCheck(0x9e3) == 0) {
    GameFlag_SetBit(0x9e3);
    Sound_Play(584);
    Func_02002e44();
    Event_ResolvePendingActions(0);
    Map_CopyCellsRect(26, 70, 1, 3, 2, 83);
    Event_Wait(20);
    Sound_Play(181);
    Map_CopyCellsRect(26, 67, 1, 3, 2, 83);
    Map_CopyCellAttributeRect(1, 23, 1, 1, 2, 23);
    Func_02002e4c();
  }
}
