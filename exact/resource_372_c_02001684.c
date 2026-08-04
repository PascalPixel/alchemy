#include "types.h"

extern s32 Func_02005d66(s32);
extern s32 Func_02005d70(s32);
extern void Func_02005d98(void);
extern void Func_02005e58(s32, s32);
extern void Func_02005d9e(s32);
extern void Func_02005e84(s32);
extern void Func_02002df0(void);
extern void Func_02005dbc(void);

void Func_02001684(void)
{
    if (Func_02005d66(0x837) == 0 && Func_02005d70(0x836) != 0) {
        Func_02005d98();
        Func_02005e58(22, 2);
        Func_02005d9e(20);
        Func_02005e84(0xe71);
        Func_02002df0();
        Func_02005dbc();
    }
}
