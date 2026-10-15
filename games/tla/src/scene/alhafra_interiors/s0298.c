#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32       Func_02000b5c();
s32       Func_02000b24();
s32       Func_02000b3c();
extern u8 MsgAlhafraInteriors06[];

void Func_02000298(s32 a0)
{
    Call3(Func_02000b5c, a0, 0x107, 0x1e);
    Call1(Func_02000b24, (s32)MsgAlhafraInteriors06);
    Call2(Func_02000b3c, a0, 0);
}
