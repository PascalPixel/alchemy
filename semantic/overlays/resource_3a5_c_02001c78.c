typedef unsigned short u16;

/*
 * resource_3a5 owner at 0x02001c78, 12 bytes: 6 of code, an alignment
 * halfword, a one-word pool.
 *
 * TAIL LEAF, and the one of its class that sweep A DOES reach -- which is why
 * it is worth recording separately from the three that nothing reached.
 * `overlay_call_targets` resolves FOUR call sites to 0x02001c78 and tags the
 * target `unknown`. That tag is the prologue gate a fourth time: `unknown` is
 * what the classifier emits when a resolved target opens with no `push`, so
 * the tool found this function, counted its callers, and then declined to call
 * it a function. Sweep A saw it and said nothing anyone would act on.
 *
 * Reaching it at all required the bounds path. The whole-overlay walk REFUSES
 * on resource_3a5 with `sites=0`, correctly, because that path walks only
 * unconverted inventory rows; the refusal names its own escape hatch, and
 * `overlay_call_targets.ts resource_3a5 0 26c2` resolves 315 sites. THE BOUNDS
 * ARE HEX. Passing the image length in decimal silently scans the wrong region
 * -- the first check used `0 a14` for a 2,580-byte image and
 * stopped exactly at the candidate.
 *
 * The ROM, verbatim:
 *
 *   ldr r3, [pc, #4]   -> 0x0200b030
 *   strh r0, [r3]
 *   bx lr
 *
 * SPAN IS 12 BYTES, NOT 6. The halfword at 0x2001c7e is alignment before the
 * word-aligned pool, and the pool word 0x0200b030 at 0x2001c80 is reached by
 * this row's own `ldr r3, [pc, #4]` (pc 0x2001c7c after alignment, plus 4).
 * Both bounds are the ROM's: the preceding row ends `pop {r5} / pop {r0} /
 * bx r0` at 0x2001c74, and a `4c00 4720` interworking veneer begins at
 * 0x2001c84.
 *
 * WHAT IT DOES. It stores its first argument, as a HALFWORD, to the fixed
 * address 0x0200b030 -- an address in this overlay's own image, not IWRAM.
 * `strh` and not `str`, so the width is pinned by the instruction.
 *
 * WHAT IT DOES NOT SETTLE. What lives at 0x0200b030. Four callers is enough to
 * know the setter matters and not enough to name the field, and the row itself
 * reads nothing back. The bound is the answer.
 *
 * 0 of 0 callees.
 */

void Func_02001c78(u16 value)
{
    *(u16 *)0x0200b030 = value;
}
