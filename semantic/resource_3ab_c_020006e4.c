#include "types.h"
extern s32 Func_080770c0(s32);
u8 *Func_020006e4(void)
{
    s16 mapState = *(s16 *)(0x02000240 + 448);
    if (mapState == 0x9f) {
        Func_080770c0(0x941);
        return (u8 *)0x0200a3b4;
    }
    if (mapState == 0x68 && Func_080770c0(0x941) != 0)
        return (u8 *)0x0200a1bc;
    return (u8 *)0x02009fc4;
}
