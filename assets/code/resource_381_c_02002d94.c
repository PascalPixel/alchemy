#include "types.h"

extern s32 Data_03001e40;
extern void Func_0200624c(s32, s32);
extern void Func_02006256(s32, s32);
extern void Func_02005c74(s32);

void Func_02002d94(s32 a)
{
    if ((Data_03001e40 & 2) != 0) {
        Func_0200624c(a, 7);
    } else {
        Func_02006256(a, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02005c74(a);
    }
}
