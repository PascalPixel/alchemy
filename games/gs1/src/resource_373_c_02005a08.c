/* Play sound 0x83 only while the low runtime nibble is clear. */
#include "types.h"

void Func_0200bb66(int sound);

void Func_02005a08(void)
{
    if ((*(u32 *)0x03001e40 & 15) == 0)
        Func_0200bb66(0x83);
}
