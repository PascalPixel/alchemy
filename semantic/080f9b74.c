#include "types.h"

/*
 * Core-drive row 0x080f9b74, 46 bytes (kind nonstandard_thumb_call_module,
 * same procedural mov-ip-lr exclusion as the sibling row 0x080f9a80 --
 * see semantic/main/080f9a80.c's header for why the label alone does not
 * disqualify this from a semantic draft).
 *
 * Reads a byte from cursorHolder's stream cursor (+64), advances the
 * cursor, uses that byte as a 12-byte-stride index into a table whose base
 * pointer lives at tableHolder+48, then runs each of the record's three
 * words through the shared validator established in semantic/main/
 * 080f9a80.c's Func_080f9a9a (candidate = the field's own address,
 * passthrough = the field's value), writing the three results into
 * cursorHolder+36/+40/+44.
 *
 * Complete owner: asm/080f9b74.s in full, `mov ip,lr` through `bx ip`; no
 * literal pool, no branch. Two arguments (r0, r1), void -- the return
 * address survives in ip across all three calls rather than the stack,
 * the same optimisation the sibling row uses.
 */

s32 Func_080f9a9a(u32 candidate, s32 passthrough);

void Func_080f9b74(u8 *tableHolder, u8 *cursorHolder)
{
    u8 *cursor = *(u8 **)(cursorHolder + 64);
    u8 index = *cursor;
    s32 *entry;

    *(u8 **)(cursorHolder + 64) = cursor + 1;

    entry = (s32 *)(*(u8 **)(tableHolder + 48) + (u32)index * 12);

    *(s32 *)(cursorHolder + 36) = Func_080f9a9a((u32)&entry[0], entry[0]);
    *(s32 *)(cursorHolder + 40) = Func_080f9a9a((u32)&entry[1], entry[1]);
    *(s32 *)(cursorHolder + 44) = Func_080f9a9a((u32)&entry[2], entry[2]);
}
