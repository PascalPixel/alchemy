typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 39e facing-quadrant selector at 0x02000f80 (156 bytes, 9 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02000f80, every branch
 * (0x02000f96, 0x02000fb2, 0x02000fbe, 0x02000fd0, 0x02000fde, 0x02001000)
 * lands inside 0x02000f80-0x02001018, and the walk stops at the interworking
 * return `pop {r0} / bx r0` at 0x02001018-0x0200101a.  So the executable
 * extent is exactly 0x02000f80-0x0200101b (156 bytes).  The popped register is
 * r0, so the popped value is the return address and the owner is `void`.
 *
 * Pool map — the walk never reaches these as instructions, and three of the
 * four disassemble as plausible code (`b.n`, `subs`, `adr`), which is the
 * "a pool word decodes as an instruction" trap:
 *   0x0200101c  0xffffe000   -0x2000
 *   0x02001020  0x3fff0000   the bound, pre-shifted
 *   0x02001024  0x00003fff   the same bound, unshifted
 *   0x02001028  0xffffa000   -0x6000
 * None is an address: all four are below the 0x02008000 link base and all four
 * are used as arithmetic operands.  The next prologue begins at 0x0200102c.
 *
 * All nine call sites were resolved with
 * `bun tools/overlay_call_targets.ts resource_39e 0f80 101c`, and the
 * per-target histogram is Func_0808a080 x1, Func_0808a0d0 x6, Func_0808a1b8
 * x2:
 *   0x02000f84 -> Func_0808a080     0x02000fe6 -> Func_0808a0d0
 *   0x02000f9e -> Func_0808a0d0     0x02000ff0 -> Func_0808a0d0
 *   0x02000fa8 -> Func_0808a0d0     0x02000ffc -> Func_0808a1b8
 *   0x02000fc6 -> Func_0808a0d0     0x02001008 -> Func_0808a0d0
 *                                   0x02001014 -> Func_0808a1b8
 *
 * THREE OF THE FOUR ARMS SHARE ONE Func_0808a1b8 SITE (0x02000ffc) and the
 * fourth has its own (0x02001014).  Writing the close into each arm would put
 * four Func_0808a1b8 calls in the C against two in the assembly — the
 * bracket-close overcount the reconstruction's convention warns about — so the shared
 * tail is spelled with a label and a `goto`, which is what the original
 * control flow is.
 *
 * The selector is the standard facing quantisation: the u16 at record+6 is
 * biased by -0x2000, -0x6000 and +0x6000 in turn and compared unsigned against
 * 0x3fff, so the four arms are the four 90-degree quadrants of the circle in
 * the order [0x2000,0x6000), [0x6000,0xa000), [0xa000,0xe000), rest.  The
 * first comparison is done on the value shifted left by 16 against the
 * pre-shifted bound 0x3fff0000 and the later ones on the value shifted back
 * down against 0x3fff — same test, two spellings, and reading either as a
 * signed magnitude test loses the wrap.
 *
 * The value handed to the shared tail (0x2000, 0x5000, 0xe000, and 0x2000 for
 * the fourth arm) is built with `movs`+`lsls` as a VALUE, not a displacement.
 */

u8 *Func_0808a080();           /* record fetch, returns the record */
void Func_0808a0d0();
void Func_0808a1b8();

void Func_02000f80(void)
{
    u16 facing = *(u16 *)(Func_0808a080(0) + 6);
    s32 amount;

    if ((u16)(facing - 0x2000) <= 0x3fff) {
        Func_0808a0d0(15, 216, 168);
        Func_0808a0d0(15, 224, 168);
        amount = 0x2000;
    } else if ((u16)(facing - 0x6000) <= 0x3fff) {
        Func_0808a0d0(15, 232, 160);
        amount = 0x5000;
    } else if ((u16)(facing + 0x6000) <= 0x3fff) {
        Func_0808a0d0(15, 216, 168);
        Func_0808a0d0(15, 224, 172);
        amount = 0xe000;
    } else {
        Func_0808a0d0(15, 232, 160);
        Func_0808a1b8(15, 0x2000, 20);
        return;
    }

    Func_0808a1b8(15, amount, 20);
}
