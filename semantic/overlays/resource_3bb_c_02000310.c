typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_3bb owner at 0x02000310, 240 bytes (0x02000310-0x020003ff):
 * 232 bytes of code plus the two-word literal pool at 0x020003f8.
 *
 * Prologue at 0x02000310 saves r5, r6, r7, lr and then fp, sl, r9 and r8
 * before `sub sp, #8`; the matching epilogue at 0x020003e6 restores all of
 * them and ends `pop {r0} / bx r0`, so r0 holds the popped return address and
 * the owner is void.  The high registers are pure caches: fp = dx << 16,
 * r9 = dz << 16, sl = the 0xfff00000 rounding mask, r8 = 0x8000.
 *
 * Three arguments.  r0 is copied to r5 and used as a participant id; r1 and r2
 * are spilled to the frame at once and re-read after the two calls that
 * clobber them, which is why they are read from the stack below.  Both are
 * tested with `blt` / `bge`, so they are signed.
 *
 * All 13 `bl` sites are placed and reach seven distinct callees, matching the
 * inventory row's calls=13.  Targets come from tools/overlay_call_targets.ts
 * (target offset = stored displacement + 2), never from the disassembler's
 * annotations - it prints the four identical Func_08009080 halfwords as four
 * different callees.  0x3f50 -> Func_0808a080 (two sites), 0x3f38 ->
 * Func_0808a018, 0x3e78 -> Func_08009150 (two), 0x3e48 -> Func_08009080
 * (four), 0x40b0 -> Func_080f9010 (two), 0x3e80 -> Func_08009158, 0x3f40 ->
 * Func_0808a020.
 *
 * The pool word 0x02000240 is below the overlay's 0x02008000 link base (base
 * witnessed by 0x02008715 = Func_02000714 + 1 in the byte-exact sibling
 * assets/code/resource_3bb_c_02000950.c), so it is a RAM global; the index is
 * built as 250 << 1 = 500, giving the active-subject word at 0x02000434.
 *
 * Shape: place the active subject and the named partner at their current
 * position offset by (dx, dz) - each axis biased by the argument shifted left
 * 16, rounded down to whole units and re-centred by half a unit, with y
 * carried through unrounded - set both records' fields at +48 and +52 to
 * 0x10000 and 0x8000, then play the paired animation.  The partner's second
 * pose is 4 when either component is negative and 3 otherwise.
 *
 * The sole caller found is Func_02000400, which passes a partner id of 32 or
 * 33 and a dx of +/-64 with dz 0.
 *
 * Uncertainties: only the record fields at +8, +12, +16, +48 and +52 are
 * asserted.  Func_08009150's fourth argument is the rounded z, computed into
 * r3 well before the branch; the ordering here is the dataflow, not the
 * instruction schedule.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_0808a080();
void Func_0808a018();
void Func_08009150();
void Func_08009080();
void Func_080f9010();
void Func_08009158();
void Func_0808a020();

void Func_02000310(s32 partner, s32 dx, s32 dz)
{
    u8 *subject;
    u8 *other;
    s32 x;
    s32 z;

    subject = Func_0808a080(*(s32 *)(0x02000240 + 500));
    other = Func_0808a080(partner);

    Func_0808a018();

    x = ((*(s32 *)(subject + 8) + (dx << 16)) & 0xfff00000) + 0x80000;
    z = ((*(s32 *)(subject + 16) + (dz << 16)) & 0xfff00000) + 0x80000;
    *(s32 *)(subject + 48) = 0x10000;           /* 128 << 9 */
    *(s32 *)(subject + 52) = 0x8000;            /* 128 << 8 */
    Func_08009150(subject, x, *(s32 *)(subject + 12), z);
    Func_08009080(subject, 27);

    x = ((*(s32 *)(other + 8) + (dx << 16)) & 0xfff00000) + 0x80000;
    z = ((*(s32 *)(other + 16) + (dz << 16)) & 0xfff00000) + 0x80000;
    *(s32 *)(other + 48) = 0x10000;
    *(s32 *)(other + 52) = 0x8000;
    Func_08009150(other, x, *(s32 *)(other + 12), z);

    if (dx < 0 || dz < 0) {
        Func_08009080(other, 4);
    } else {
        Func_08009080(other, 3);
    }

    Func_080f9010(226);
    Func_08009158(subject);
    Func_080f9010(288);                         /* 144 << 1 */
    Func_08009080(other, 2);
    Func_0808a020();
}
