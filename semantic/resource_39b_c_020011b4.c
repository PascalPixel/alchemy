#include "types.h"

extern void Func_0808a018();
extern void Func_0808a138();
extern void Func_0808a010();
extern void Func_0808a260();
extern void Func_0808a250();
extern void Func_0808a020();

/* Install the scene-0x205 actor presentation and owner-record mode. */
void Func_020011b4(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;

    Func_0808a018();
    Func_0808a138(8, 2);
    Func_0808a010(20);
    *(int *)(workspace + 448) = 512;
    Func_0808a260(53, 31);
    *(u8 *)(0x02000240 + 0x22b) = 3;
    Func_0808a250(36, 1);
    Func_0808a020();
}
