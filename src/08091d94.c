#include "types.h"

void Func_08077088(void);
void Func_080f9010(s32);
void Func_08015120(s32, s32);
void Func_08015040(void *, s32);
extern u8 Value_0000001e;

void Func_08091d94(s32 first, s32 second)
{
    Func_08077088();
    Func_080f9010(0x53);
    Func_08015120(first, 1);
    Func_08015120(second, 4);
    Func_08015040(&Value_0000001e, 3);
}
