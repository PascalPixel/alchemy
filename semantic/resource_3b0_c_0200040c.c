#include "types.h"
/* Two-placement scripted camera owner at 0x0200040c, 92 bytes. The preceding
 * eight ranked bytes belong to the prior pool and are deliberately unclaimed. */
void Func_0808a018(); void Func_0808a210(); void Func_080000c0();
u8 *Func_0808a228(); void Func_08009128(); void Func_0808a0f0();
void Func_020004b0(void); void Func_0808a020();
void Func_0200040c(void)
{
    u8 *record;
    s32 permuted_2;
    permuted_2 = Func_0808a228(); record[0x55] = 0;
    Func_0808a018(); Func_0808a210(-1, -1, -1, 0); Func_080000c0(1);
    Func_0808a210(0x00a40000, 0x00400000, 0x01410000, 0);
    record  = permuted_2;
    Func_08009128(); Func_080000c0(1); Func_0808a0f0(0, 0, 0);
    Func_020004b0(); Func_0808a020();
}
