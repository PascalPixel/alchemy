#include "types.h"

extern u8 Data_02001004;

void Func_02000a54(void)
{
    u8 raw = Data_02001004;
    s32 init = 0x3f42;
    s8 v;
    s32 val;

    *(u16 *)0x4000050 = init;
    v = raw;
    if (v == 0) {
        val = 0x1000;
        *(u16 *)0x4000052 = val;
    } else if (v == 1) {
        val = 0xe00;
        *(u16 *)0x4000052 = val;
    } else if (v == 2) {
        val = 0xc00;
        *(u16 *)0x4000052 = val;
    } else if (v == 3) {
        val = 0xa00;
        *(u16 *)0x4000052 = val;
    } else if (v == 4) {
        val = 0x800;
        *(u16 *)0x4000052 = val;
    } else {
        val = 0x600;
        *(u16 *)0x4000052 = val;
    }
}
