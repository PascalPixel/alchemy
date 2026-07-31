typedef unsigned char u8;
typedef unsigned short u16;

/*
 * resource_397 owner at 0x0200015c, 88 bytes: re-arm the alpha blend this
 * resource is drawn with.
 *
 * Complete owner: `push {lr}` at 0x0200015c through `pop {r0} / bx r0` at
 * 0x020001b0-0x020001b2.  The return address is popped into r0, so the owner
 * is `void` (HANDOVER section 0, epilogue rule).  No argument register is read
 * before being written, so it takes none.  The single `b.n 0x020001b0` at
 * 0x0200018c hops the 34-byte literal pool at 0x0200018e-0x020001af, which is
 * inside the advertised span and is not code -- and which the inventory also
 * reports as three phantom rows (0x0200016e, 0x0200017c, 0x0200018e), all of
 * them the pool decoding as plausible Thumb.
 *
 * One call site, matching the inventory's calls=1.  Resolved with
 * `bun tools/overlay_call_targets.ts resource_397 015c`:
 *   0x02000160 -> veneer 0x034c -> Func_0808a300
 * The printed annotation 0x020004ae is the usual `bl` decoding artefact.
 *
 * NEAR-TWIN of the root at 0x02000200, and that is the cheapest evidence
 * available for both.  Instruction for instruction the two are the same
 * routine over the same constants -- Func_0808a300(9), BLDCNT = 0x3f42,
 * BLDALPHA = 0x0c04, then +0x534 = 0x3f3f, +0x536 = 31, +0x52a = 10 into the
 * display workspace.  The root does two things this one does not: it publishes
 * the scene id 32 first, and it calls Func_020002e0 and returns 0 last.  So
 * this owner is the "re-arm only" half of the root -- what a scene calls when
 * it comes back to the resource without re-entering it.
 *
 * The one *spelling* difference between the twins is worth recording because
 * it hides the identity: the root reaches the display workspace as
 * `*(0x03001ebc + 16)`, while this owner loads the pool word 0x03001ecc and
 * dereferences it directly.  0x03001ecc IS 0x03001ebc + 16 -- the same slot of
 * the same pointer table, reached two ways inside one overlay.  Read without
 * noticing, the twins look like they touch two unrelated globals.
 *
 * Uncertainties are exactly the root's: the three workspace halfwords are
 * written and never read here, and the argument 9 to Func_0808a300 is not
 * resolved.
 */

/* Overlay import (via the veneer table).  Old-style declaration: an overlay
 * import can be reached with different argument counts at different sites. */
void Func_0808a300();

#define BLDCNT_0200015c   (*(volatile u16 *)0x04000050)
#define BLDALPHA_0200015c (*(volatile u16 *)0x04000052)

void Func_0200015c(void)
{
    u8 *display = *(u8 **)0x03001ecc;   /* = 0x03001ebc + 16 */

    Func_0808a300(9);

    BLDCNT_0200015c = 0x3f42;      /* alpha-blend BG1 over every other layer */
    BLDALPHA_0200015c = 0x0c04;    /* EVA = 4, EVB = 12 */

    *(u16 *)(display + 0x534) = 0x3f3f;
    *(u16 *)(display + 0x536) = 31;
    *(u16 *)(display + 0x52a) = 10;
}
