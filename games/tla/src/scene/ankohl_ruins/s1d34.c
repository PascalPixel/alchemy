#include "ankohl_ruins.h"

void Func_02001d34(void) {
  s32 done = AnkohlRuins_FlagCheck(0x202);

  if (done == 0) {
    Func_02002edc(0);
    Func_02002eec(584);
    FieldEvent_Wait(15);
    Func_02002eec(202);
    AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 66);
    AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
    FieldEvent_Wait(15);
    gData_02003780 = 7;
    gData_02003782 = done;
    gData_02003784 = 200;
    gData_02003788 = 0x8000;
    gData_0200378c = 1;
    AnkohlRuins_FlagSet(381);
    Func_0200160c();
    AnkohlRuins_FlagSet(0x202);
  }
}

void Func_02001ddc(void) {
  if (AnkohlRuins_FlagCheck(0x9e2) == 0) {
    AnkohlRuins_FlagSet(0x9e2);
    Func_02002eec(584);
    Func_02002e44();
    Func_02002edc(0);
    AnkohlRuins_Call2df4(26, 70, 1, 3, 14, 65);
    FieldEvent_Wait(20);
    Func_02002eec(181);
    AnkohlRuins_Call2df4(26, 67, 1, 3, 14, 65);
    AnkohlRuins_Call2dfc(15, 5, 1, 1, 14, 5);
    Func_02002e4c();
  }
}

void Func_02001e54(void) {
  s32 done = AnkohlRuins_FlagCheck(0x203);

  if (done == 0) {
    Func_02002edc(0);
    Func_02002eec(584);
    FieldEvent_Wait(15);
    Func_02002eec(202);
    AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 84);
    AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
    FieldEvent_Wait(15);
    gData_02003780 = 3;
    gData_02003782 = done;
    gData_02003784 = 100;
    gData_02003788 = 0x8000;
    gData_0200378c = 2;
    AnkohlRuins_FlagSet(381);
    Func_020019a0();
    AnkohlRuins_FlagSet(0x203);
  }
}

void Func_02001f04(void) {
  if (AnkohlRuins_FlagCheck(0x9e3) == 0) {
    AnkohlRuins_FlagSet(0x9e3);
    Func_02002eec(584);
    Func_02002e44();
    Func_02002edc(0);
    AnkohlRuins_Call2df4(26, 70, 1, 3, 2, 83);
    FieldEvent_Wait(20);
    Func_02002eec(181);
    AnkohlRuins_Call2df4(26, 67, 1, 3, 2, 83);
    AnkohlRuins_Call2dfc(1, 23, 1, 1, 2, 23);
    Func_02002e4c();
  }
}
