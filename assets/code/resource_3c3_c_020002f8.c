#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_02000c78(s32);
extern void Func_02000b9e(s32);
extern void Func_02000ba4(s32);
extern void Func_02000c7a(s32);

void Func_020002f8(void)
{
    s16 token = *(s16 *)(Data_03001ebc + 364);

    Func_02000c78(123);
    Func_02000b9e(0x8FB);
    Func_02000ba4(0x8FC);
    Func_02000c7a(token);
}
