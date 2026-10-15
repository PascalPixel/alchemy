#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02003bc8();
s32 Func_02003c88();
s32 GameFlag_SetBit();
s32 Func_02003bd0();

void Func_020006d4(void)
{
    s32 t0;

    t0 = Value1(Object_GetById, 0xd);
    Func_02003bc8();
    Call1(Func_02003c88, 0);
    if ((FIELD_AT(t0, s32, 8) >> 19) == 0x17)
    {
        Call1(GameFlag_SetBit, 0x879);
    }
    Func_02003bd0();
}
