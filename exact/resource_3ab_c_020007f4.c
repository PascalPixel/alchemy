#include "types.h"



extern s32 Func_020021ba(s32);
extern s32 Func_020021c4(s32);
extern void Func_02002282(s32);
extern void Func_0200229a(s32, s32);
extern void Func_02002292(s32);
extern void Func_020022aa(s32, s32);
void Func_020007f4(void)
{
    if (Func_020021ba(0x941) == 0) {
        s32 message;
        if (Func_020021c4(0x85a) == 0)
            message = 0x1be2;
        else
            message = 0x1ba5;
        Func_02002282(message);
        Func_0200229a(18, 0);
    } else {
        Func_02002292(0x250c);
        Func_020022aa(18, 0);
    }
}
