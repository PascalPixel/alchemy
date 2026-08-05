#include "types.h"

/* Complete blend-alpha setter through return and its two pool words. */
void Func_020018d0(void)
{
    u16 value = 0x0607;

    *(volatile u16 *)0x04000052 = value;
}
