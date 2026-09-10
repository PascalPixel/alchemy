#include "types.h"

#define FieldScene_RunFourCallSequence Func_02001408
#define FieldScene_ApplyTable2b20000 Func_02001790
#define FieldScene_RunFourStepSequence Func_02002504
extern u8 Data_02b20000[];

void Func_02006ec0(void);
void Func_020017b8(void);
void Func_02006ed0(void);
void Func_02002838(void);
void Func_020052c0(s32, s32, s32);
void Func_02007fbc(void);
void Func_020028b4(void);
void Func_02004a2c(void);
void Func_02007fd0(void);

void FieldScene_RunFourCallSequence(void)
{
    Func_02006ec0();
    Func_020017b8();
    Func_02006ed0();
    Func_02002838();
}

void FieldScene_ApplyTable2b20000(void)
{
    Func_020052c0((s32)Data_02b20000, 0, 0x2480000);
}

void FieldScene_RunFourStepSequence(void)
{
    Func_02007fbc();
    Func_020028b4();
    Func_02004a2c();
    Func_02007fd0();
}
