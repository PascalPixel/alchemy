typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 372 cutscene step at 0x02000278 (108 bytes, 10 calls).
 *
 * Complete owner: `push {lr}` at 0x02000278, matching `pop {r0} / bx r0`
 * interworking return at 0x020002cc.  Bytes 0x020002d0-0x020002e3 are the
 * literal pool; the byte-exact reconstruction in
 * `assets/code/resource_372_overlay.s` spells them as `.4byte`/`.2byte` data
 * and they are reached only through `ldr rN, [pc, #imm]`.
 *
 * `pop {r0} ; bx r0` — r0 holds the popped return address, so this owner
 * returns nothing.
 *
 * Call-target convention: every `bl` in this overlay is named by the address
 * its call site computes, which is exactly what the byte-exact siblings
 * `assets/code/resource_372_c_020002e4.c` and `..._02000400.c` already do.
 * Those two are near-identical bodies whose `bl`s resolve to *different*
 * addresses for what is provably the same callee (0x02004ba8 vs 0x02004cba,
 * 0x0200498a vs 0x02004a9c, 0x02000474 vs 0x02000582), so an encoded overlay
 * target is a stable identity for an import, not a place to disassemble.
 * That applies to 0x020017b6 and 0x02000426 below as well: both land inside
 * other owners' bodies, and the established in-tree reading treats them as
 * import identities rather than as hidden-context entries.
 *
 * Uncertainties: the interfaces of the imports are unknown, hence the
 * old-style declarations (the same import can be reached under two names and
 * with different argument counts elsewhere in this overlay).  Small integer
 * pool constants (0x206, 0x835, 0x831, 0x325) are event/flag identifiers in
 * the byte-exact sources' `Value_xxxxxxxx` spelling; here they are written as
 * plain constants because the semantic reconstruction makes no pool-placement claim.
 */

/* Imports, old-style: interfaces not yet known. */
void Func_080f9010();
void Func_08009178();
void Func_0200150c();
void Func_080770c8();
void Func_0808a0c8();
void Func_0808a010();
void Func_0200015c();

/* Used for their return values. */
s32 Func_080770c0();

/* In-image data block referenced by the pool word 0x0200d7a0. */
extern u8 Data_0200d7a0[];

void Func_02000278(void)
{
    if (Func_080770c0(0x206) == 0) {
        Func_080f9010(158);
        Func_08009178(Data_0200d7a0, 45, 39);
    }

    /* 0x835 and 0x831 are tested in sequence; either one non-zero takes the
     * second branch.  Note the second test is only reached when the first
     * returned 0. */
    if (Func_080770c0(0x835) == 0 && Func_080770c0(0x831) == 0) {
        Func_0200150c();
        Func_080770c8(0x206);
    } else {
        /* r1 is built as 131 << 1 = 0x106; r2 comes from the pool. */
        Func_0808a0c8(0, 0x106, 0x325);
        Func_0808a010(3);
        Func_0200015c(6);
    }
}
