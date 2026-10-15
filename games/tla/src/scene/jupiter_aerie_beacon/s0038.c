#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32 Func_020059f4();
s32 ObjectDispatch_SetSingleChildFlags();

s32 Func_02000038(s32 a0)
{
    Call1(Func_020059f4, 0x98);
    Call2(ObjectDispatch_SetSingleChildFlags, a0, 0);
    return 0;
}
