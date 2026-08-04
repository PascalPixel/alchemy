/* Store the caller's halfword in the scene workspace field at offset 220. */
#include "types.h"

void Func_020045d0(u16 value)
{
    u8 *workspace = *(u8 **)0x03001f3c;
    *(u16 *)(workspace + 220) = value;
}
