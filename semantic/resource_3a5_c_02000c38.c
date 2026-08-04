#include "types.h"

extern void Func_0808a138();
extern void Func_0808a260();
extern void Func_0808a250();

/* Put actor 8 and actor 53 into their scene-91 presentation variants. */
void Func_02000c38(void)
{
    Func_0808a138(8, 2);
    Func_0808a260(91, 5);
    *(u8 *)(0x02000240 + 0x22b) = 3;
    Func_0808a250(53, 5);
}
