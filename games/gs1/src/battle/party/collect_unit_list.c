#include "inventory.h"

extern u8 *Data_03001e74;
extern s8 Data_080c2a10[];

s32 Func_080b6a60(u16 *out);
s32 Func_080b6ae0(u16 *out);

void BattleParty_CollectUnitList(void)
{
    u16 buf[14];
    u8 *state;
    s32 count;
    s32 i;
    s32 offset;
    s32 index;
    s32 last;
    s32 kind;
    u16 *out;

    state = Data_03001e74;
    count = Func_080b6a60(buf);
    for (i = 0; i < count; i++) {
        *(u16 *)(state + 88 + i * 2) = buf[i];
    }
    offset = count * 2 + 88;
    *(u16 *)(state + offset) = 0xFF;

    count = Func_080b6ae0(buf);
    kind = state[66];
    if (kind >= 0) {
        if (kind <= 1) {
            for (i = 0; i < count; i++) {
                out = (u16 *)(state + 2);
                out[50 + i] = buf[i];
            }
            goto done;
        }
    }
    for (i = 0; i < count; i++) {
        index = (Data_080c2a10[i] + count / 2) * 2 + 100;
        out = (u16 *)(state + 2);
        *(u16 *)((u8 *)out + index) = buf[i];
    }
done:
    out = (u16 *)(state + 2);
    last = count * 2 + 100;
    *(u16 *)((u8 *)out + last) = 0xFF;
}
