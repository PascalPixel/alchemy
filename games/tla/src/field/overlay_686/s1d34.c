#include "ov686.h"

void Func_02001d34(void)
{
    s32 done = Ov686_FlagCheck(0x202);

    if (done == 0) {
        Func_02002edc(0);
        Func_02002eec(584);
        Battle_WaitMode0(15);
        Func_02002eec(202);
        Ov686_Call2df4(18, 66, 3, 1, 7, 66);
        Ov686_Call2e24(0x20000, 0x20000, 0x10000);
        Battle_WaitMode0(15);
        Data_02003780 = 7;
        Data_02003782 = done;
        Data_02003784 = 200;
        Data_02003788 = 0x8000;
        Data_0200378c = 1;
        Ov686_FlagSet(381);
        Func_0200160c();
        Ov686_FlagSet(0x202);
    }
}

void Func_02001ddc(void)
{
    if (Ov686_FlagCheck(0x9e2) == 0) {
        Ov686_FlagSet(0x9e2);
        Func_02002eec(584);
        Func_02002e44();
        Func_02002edc(0);
        Ov686_Call2df4(26, 70, 1, 3, 14, 65);
        Battle_WaitMode0(20);
        Func_02002eec(181);
        Ov686_Call2df4(26, 67, 1, 3, 14, 65);
        Ov686_Call2dfc(15, 5, 1, 1, 14, 5);
        Func_02002e4c();
    }
}

void Func_02001e54(void)
{
    s32 done = Ov686_FlagCheck(0x203);

    if (done == 0) {
        Func_02002edc(0);
        Func_02002eec(584);
        Battle_WaitMode0(15);
        Func_02002eec(202);
        Ov686_Call2df4(18, 66, 3, 1, 7, 84);
        Ov686_Call2e24(0x20000, 0x20000, 0x10000);
        Battle_WaitMode0(15);
        Data_02003780 = 3;
        Data_02003782 = done;
        Data_02003784 = 100;
        Data_02003788 = 0x8000;
        Data_0200378c = 2;
        Ov686_FlagSet(381);
        Func_020019a0();
        Ov686_FlagSet(0x203);
    }
}

void Func_02001f04(void)
{
    if (Ov686_FlagCheck(0x9e3) == 0) {
        Ov686_FlagSet(0x9e3);
        Func_02002eec(584);
        Func_02002e44();
        Func_02002edc(0);
        Ov686_Call2df4(26, 70, 1, 3, 2, 83);
        Battle_WaitMode0(20);
        Func_02002eec(181);
        Ov686_Call2df4(26, 67, 1, 3, 2, 83);
        Ov686_Call2dfc(1, 23, 1, 1, 2, 23);
        Func_02002e4c();
    }
}

