#include "types.h"

/*
 * Select one of three overlay-local scene records from the signed state code
 * at 0x02000400. The complete owner includes alignment and its seven-word pool
 * through 0x02000dcb.
 */
u8 *Func_02000d80(void)
{
    s16 state = *(volatile s16 *)0x02000400;

    if (state == 0x5e) return (u8 *)0x0200a2c4;
    if (state == 0x5f) return (u8 *)0x0200a39c;
    return (u8 *)0x0200a234;
}
