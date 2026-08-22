#include "resource_397.h"

/*
 * resource_397 owner at 0x020001d0, 2 bytes: `bx lr` alone.
 *
 * SWEEP-D RESIDUE, and the reason sweep D exists. This row is in NO sweep:
 * it is not published anywhere in the image (the full spelling
 * 0x02000000 + 0x1d0 + 0x8000 + Thumb bit = 0x20081d1 appears in no word),
 * no `bl` in the image reaches it, and it has no `push` for a shape scan
 * to key on. It offers no key to sweeps A, B or C. It was found only as
 * unaccounted bytes between two exact-C owners -- 0x020001bc ends at
 * 0x020001ca and 0x020001d4 begins the next -- and the ten bytes in between
 * read as pad, stub, pad, stub, pad.
 *
 * An EMPTY HOOK, the same body resource_3bb's 0x02003228 has and the same
 * one this cohort's two-byte class is made of. It saves nothing, reads
 * nothing, writes nothing and returns at once. Nothing here establishes
 * what it would have done; only that it does nothing.
 *
 * SPAN 2, NOT 4. The `0x0000` halfword before it aligns the entry to four
 * bytes and the one after aligns the next, and neither is part of this
 * body. Recording 4 would claim alignment as code and put a phantom
 * overlap in the next reader's arithmetic.
 */

void Func_020001d0(void)
{
}
