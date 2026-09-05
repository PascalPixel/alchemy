#include "types.h"

extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
s32 Func_02003362();
s32 Func_020033bc();
s32 Func_020033c4();
s32 Func_020033ce();
s32 Func_020033d6();
void Func_02003404();

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Func_02000e50(void)
{
    s32 dst;
    s32 src;
    s32 idx;
    s32 tbl;
    s32 idx4;
    s32 off24a;
    u8 *work;

    work = *(volatile s32 *)Data_03001ebc;
    if (Value1(Func_02003362, 0x9ca) != 0) {
        off24a = 0x24a;
        if (*(s16 *)((s32)Data_02000240 + off24a) != 15) {
            idx = *(s16 *)((s32)work + 0x16c);
            dst = Value1(Func_020033bc, 15);
            src = Value1(Func_020033c4, 0);
            *(volatile s32 *)(dst + 48) = *(volatile s32 *)(src + 48);
            dst = Value1(Func_020033ce, 15);
            src = Value1(Func_020033d6, 0);
            *(volatile s32 *)(dst + 52) = *(volatile s32 *)(src + 48);
            idx -= 30;
            tbl = 0x0200a808;
            idx <<= 3;
            idx4 = idx + 4;
            Func_02003404(15, *(volatile s32 *)(tbl + idx), *(volatile s32 *)(tbl + idx4));
        }
    }
}
