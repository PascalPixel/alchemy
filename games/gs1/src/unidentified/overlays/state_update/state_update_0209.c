#include "types.h"
extern void Func_020023ac(void); extern void Func_0200244a(int); extern int Func_02002388(int);
extern void Func_02001326(int); extern void Func_020023de(void);
void Func_020009f0(void)
{
    u8 *workspace;
    Func_020023ac(); Func_0200244a(0x1336);
    if (Func_02002388(2) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Func_02001326(12); Func_020023de();
}
