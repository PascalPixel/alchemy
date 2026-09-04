#include "types.h"

#define FieldScene_RunScriptedStep1120 Func_02004618
extern u8 Value_00001120;

extern void Func_02008d18(void);
extern void Func_02008cf8(s32, s32);
extern void Func_02008d2c(void);

void FieldScene_RunScriptedStep1120(void)
{
    Func_02008d18();
    Func_02008cf8((s32)&Value_00001120, 1);
    Func_02008d2c();
}
