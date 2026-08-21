#include "types.h"

/*
 * resource_3bf owner at 0x02000c78, 24 bytes.
 *
 * Both wide arguments are built by shifting a byte-sized constant rather than
 * pooled: 0xf8 << 16 and 0xbc << 17.
 */

void Func_02006326();
void Func_020017d6();

void Func_02000c78(void)
{
    Func_02006326(12, 0x00f80000, 0x01780000);
    Func_020017d6();
}
