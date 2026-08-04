#include "types.h"

extern u8 Value_00001a40;

void Func_02000b64(void);
void Func_02000ba2(s32);
s32 Func_02000bc2(s32, s32);
void Func_02000b7e(void);

void Func_02000504(void)
{
    Func_02000b64();
    Func_02000ba2((s32)&Value_00001a40);
    Func_02000bc2(12, 0);
    Func_02000b7e();
}
