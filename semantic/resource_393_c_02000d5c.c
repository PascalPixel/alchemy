#include "types.h"
extern u32 Data_03001ed0;

void Func_02000d5c(void)
{
    u32 *data = &Data_03001ed0;
    u32 *p = (u32 *)0x040000d4;
    u32 b = *data;
    u32 src = 0x020090e0;
    u32 c = 0x840000e0;
    p[0] = b;
    p[1] = src;
    p[2] = c;
}
