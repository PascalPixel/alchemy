#include "types.h"

u8 Func_020011f6(u8);
u8 Func_02001204(s32);
u8 Func_02001206(s32, s32, s32, s32);
s32 Func_02001222(void);
u8 Func_02001242(void);

/* 問い合わせが非零を返すまで毎回1で進行させる。判定は後置、入口は判定へ飛ぶ。 */
void Func_0200004c(s32 arg0)
{
    Func_02001242();
    Func_02001206(arg0, 5, 0, 0x22);
    while (Func_02001222() == 0) {
        Func_020011f6(1);
    }
    Func_02001204(1);
}
