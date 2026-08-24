#include "types.h"

extern u8 *Func_0200743e(s32);
extern s32 Func_020073fe(s32);
extern void Func_0200290a(s32, s32);
extern s32 Data_0200f63c[];
extern s32 Data_0200f6cc[];

void Func_02001454(void)
{
    u8 *p = Func_0200743e(22);
    if (Func_020073fe(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 1) {
            Func_0200290a((s32)Data_0200f63c, (s32)Data_0200f6cc);
        }
    }
}
