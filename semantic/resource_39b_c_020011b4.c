#include "types.h"








/* Install the scene-0x205 actor presentation and owner-record mode. */
extern void Func_02003694();
extern void Func_02003704();
extern void Func_0200369a();
extern void Func_02003790();
extern void Func_0200379a();
extern void Func_020036d6();
void Func_020011b4(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;

    Func_02003694();
    Func_02003704(8, 2);
    Func_0200369a(20);
    *(int *)(workspace + 448) = 512;
    Func_02003790(53, 31);
    *(u8 *)(0x02000240 + 0x22b) = 3;
    Func_0200379a(36, 1);
    Func_020036d6();
}
