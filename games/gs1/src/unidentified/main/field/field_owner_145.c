#include "types.h"

#define Func_080c0228 Func_080c0228

/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 Data_03001ef8[];

void Func_080c0228(void)
{
    u32 i;
    s32 record;
    s32 v3;
    s32 value;
    u32 idx;
    u16 *p;

    record = *(s32 *)(*(s32 *)Data_03001ef8);
    if ((u32)record <= 79) {
        value = (7 & record) + 0xf081;
        if (record >= 0) {
            v3 = record;
        } else {
            v3 = record + 7;
        }
        idx = 13 - (v3 >> 3);
        i = 0;
        p = (u16 *)((idx << 6) + 0x06006000);
        do {
            i++;
            *p = value;
            p++;
        } while (i != 32);

        value = value | 0x800;
        v3 = record;
        if (record < 0) {
            v3 = v3 + 7;
        }
        idx = (v3 >> 3) + 13;
        if (idx <= 20) {
            i = 0;
            p = (u16 *)((idx << 6) + 0x06006000);
            do {
                i++;
                *p = value;
                p++;
            } while (i != 32);
        }
    }
}
