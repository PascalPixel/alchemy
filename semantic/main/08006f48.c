#include "types.h"

/*
 * Main-image core-band gap at 0x08006f48, 36 bytes (census kind
 * `nonstandard_thumb_branch_module`).
 *
 * The flash-chip descriptor pointer at 0x02004C08 and its `size`
 * field at offset +4 are the same fields the byte-exact
 * src/08006cdc.c already establishes (`struct FlashInfo08006cdc`,
 * `size` at offset 4) -- reused here rather than re-deriving a new
 * struct name.
 *
 * Scans forward from `buf`, counting down `size` while each byte read
 * is 0xff (flash's "erased" value); stops at the first non-0xff byte
 * or when the count reaches zero, and returns however much of `size`
 * is left -- i.e. how many trailing erased bytes remain unscanned,
 * effectively "bytes from here to the end of the erased region minus
 * one" once a written byte is found.
 */

struct FlashInfo08006cdc {
    u8 unknown_00[4];
    s32 size;
    u8 shift;
    u8 unknown_09[7];
    u16 control;
};

s32 Func_08006f48(u8 *buf)
{
    s32 remaining = (*(struct FlashInfo08006cdc **)0x02004c08)->size;

    while (remaining != 0) {
        u8 value = *buf++;
        if (value != 0xff) {
            break;
        }
        remaining--;
    }

    return remaining;
}
