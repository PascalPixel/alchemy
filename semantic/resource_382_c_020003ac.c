#include "types.h"

extern u8 *Data_03001e8c[];
extern u8 *Func_0808a080(s32);
extern s32 Func_02001674(u8 *, const u8 *, s32, s32);

s32 Func_020003ac(u8 *entity)
{
    u8 *base = Data_03001e8c[0];
    u8 *workspace = Data_03001e8c[12];
    s32 flag = 0;
    s32 selector = 18;
    u8 *leader;

    if (*(s32 *)(entity + 56) == (s32)0x80000000)
        return 0;

    leader = Func_0808a080(0);
    if (*(s16 *)(workspace + 376) != 0 || base[0x0ea4] != 0) {
        selector = 26;
        flag = 1;
    }
    Func_02001674(entity, leader, selector, flag);
    return 0;
}
