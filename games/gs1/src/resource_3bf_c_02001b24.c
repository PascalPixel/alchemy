#include "types.h"

extern u8 Value_0000240d;

void Func_0200715a(void);
void Func_0200715e(void);
void Func_02007252(s32, s32, s32);
void Func_020071d4(s32, s32, s32);
void Func_020071ac(s32, s32);
void Func_020071fc(s32, s32);
void Func_0200722e();
void Func_0200720e(s32, s32);
void Func_02007200(s32, s32, s32);
void Func_020071d8(s32, s32);
void Func_020071ee();
void Func_0200727e(s32);
void Func_02007296(s32, s32);
void Func_02007278(s32, s32, s32);
void Func_020072ce(s32, s32, s32);
void Func_020072a4(s32);
void Func_020072bc(s32, s32);
void Func_02007340(void);
void Func_020071f6(s32);
void Func_0200732c(s32);
void Func_02007210(void);

void Func_02001b24(s32 arg0)
{
    Func_0200715a();
    Func_0200715e();
    Func_02007252(arg0, 256, 1);
    Func_020071d4(arg0, 0, 0);
    Func_020071ac(arg0, 1);
    Func_020071fc(arg0, 0);
    Func_0200722e(arg0, 0, 0);
    Func_0200720e(0, 1);
    Func_02007200(arg0, 0, 0);
    Func_020071d8(arg0, 1);
    Func_020071ee(arg0);
    Func_0200722e(arg0, 0);
    Func_020071ee(0, 1);
    {
        u8 *t = &Value_0000240d;

        Func_0200727e((s32)t);
        Func_02007296(arg0, 0);
        Func_02007278(0, arg0, 0);
        Func_020072ce(0, 258, 60);
        Func_020072a4((s32)(t + 1));
    }
    Func_020072bc(arg0, 0);
    Func_02007340();
    Func_020071f6(60);
    Func_0200732c(60);
    Func_02007210();
}
