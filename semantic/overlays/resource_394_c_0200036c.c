typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;

/*
 * Resource 394 per-scanline scroll callback at 0x0200036c.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x0200036c and the matching
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x020003a0.  84-byte row: 58 bytes of
 * code, an alignment halfword at 0x020003a6, and six pool words filling
 * 0x020003a8-0x020003bf.  Control-flow walk: the branch targets are
 * 0x0200037e and 0x02000392, both inside the body, and the next owner's entry
 * is at 0x020003c0.  The return address is popped into r0, so the owner is
 * `void`.
 *
 * ROLE, named in advance by a pool word.  The already-converted
 * semantic/overlays/resource_394_c_020003f0.c installs `(void *)0x0200836d`,
 * which is 0x0200_036c plus the Thumb bit under the proven 0x02008000 link
 * base — so this row is that scene's registered per-frame hook, and its
 * signature is fixed as `void f(void)` before any disassembly.
 *
 * It reloads the three background scroll register pairs at 0x04000014,
 * 0x04000018 and 0x0400001c from one of two twelve-byte sources: the settled
 * values at 0x03001ad4, or the displaced copy at 0x0200a0d0 that tracked
 * byte-exact assets/code/resource_394_c_020003c0.c builds from the same
 * twelve bytes by adding 0xb0 to each vertical component.  The displaced copy
 * is selected on a probability: only while VCOUNT reads 0xe3 or is at most
 * 0x34, and then only when a scaled draw from Func_080000f8 falls under the
 * intensity word at 0x0200a0dc, which resource_394_c_020003f0.c drives.
 *
 * One call site, matching the row's advertised count.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
s32 Func_080000f8(); /* pseudorandom draw */

void Func_0200036c(void)
{
    const s32 *source = (const s32 *)0x03001ad4;
    u16 scanline = *(u16 *)0x04000006;

    if (scanline == 0xe3 || scanline <= 0x34) {
        if ((u32)(Func_080000f8() * 100) >> 16 < *(u32 *)0x0200a0dc) {
            source = (const s32 *)0x0200a0d0;
        }
    }

    *(s32 *)0x04000014 = source[0];
    *(s32 *)0x04000018 = source[1];
    *(s32 *)0x0400001c = source[2];
}
