#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_02008128(void);
extern void Func_02008296(s32);
extern void Func_02008164(s32, s32, s32);
extern void Func_020081c4(s32, s32);
extern void Func_020080e0(s32);
extern void Func_02008156(s32);
extern void Func_020081d2(s32, s32, s32);
extern void Func_020080fe(s32);
extern void Func_02008174(s32);
extern void Func_020081e8(s32, s32, s32);
extern void Func_02008118(s32);
extern void Func_0200818e(s32);
extern void Func_02008202(s32, s32, s32);
extern void Func_020081a0(s32);
extern void Func_020082b6(s32);
extern void Func_0200814c(s32);
extern void Func_02008152(s32);
extern void Func_02008158(s32);
extern void Func_020081d4(void);

void Func_02003d40(void)
{
    u8 *base = Data_03001ebc;
    s32 v;

    Func_02008128();
    Func_02008296(158);
    Func_02008164(0, 0x8000, 0x4000);
    Func_020081c4(0, 2);
    v = *(s16 *)(base + 364);
    if (v == 32) {
        Func_020080e0(1);
        Func_02008156(10);
        Func_020081d2(0, 0, -16);
    } else if (v == 30) {
        Func_020080fe(4);
        Func_02008174(10);
        Func_020081e8(0, 3, -16);
    } else {
        Func_02008118(2);
        Func_0200818e(10);
        Func_02008202(0, 3, -16);
    }
    Func_020081a0(16);
    Func_020082b6(*(s16 *)(base + 364));
    Func_0200814c(1);
    Func_02008152(2);
    Func_02008158(4);
    Func_020081d4();
}
