/*
 * resource_39c @ 0x02000030 (22 bytes, no pool).
 *
 * Writes the low two bits of value into bits 2-3 of the byte at +9 in the
 * record reached through argument +80.  The six 8-byte import veneers before
 * this leaf remain structural code and are not represented as ordinary C.
 */
#include "types.h"

void Func_02000030(void *record, s32 value)
{
    u8 *inner = *(u8 **)((u8 *)record + 80);
    s32 result = ((value & 3) << 2) | (inner[9] & ~0x0c);

    inner[9] = (u8)result;
}
