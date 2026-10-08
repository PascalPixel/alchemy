#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02003bc8();
s32 Func_02003c88();
s32 Func_02003ca8();
s32 Func_02003bd0();
extern u8 Data_02003e4c[];

void Func_02001da8(void)
{
    Func_02003bc8();
    Call1(Func_02003c88, 0);
    Call1(Func_02003ca8, (s32)Data_02003e4c);
    Func_02003bd0();
}

