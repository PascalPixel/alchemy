#include "types.h"

extern u8 *Func_0200753e(s32);
extern s32 Func_020074fe(s32);
extern void Func_02002a0a(s32, s32);
extern s32 Data_0200f748[];
extern s32 Data_0200f7c4[];

void Func_02001554(void)
{
    u8 *p = Func_0200753e(22);
    if (Func_020074fe(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 2) {
            Func_02002a0a((s32)Data_0200f748, (s32)Data_0200f7c4);
        }
    }
}
