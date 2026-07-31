typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3a9 owner at 0x0200007c, 104 bytes: pick this overlay's script table
 * for the current map, install it when the map is the one this overlay is built
 * around, and hand the table back to the caller.
 *
 * Role known in advance from the offset-0 exported-entry veneer table: its
 * fourth entry is `ldr r4,[pc,#0] / bx r4 / .word 0x0200807d`, and under the
 * proven 0x02008000 link base 0x0200807d is file offset 0x007c plus the Thumb
 * bit — this function.  So it is an exported ROOT, not called from anywhere
 * inside the overlay (`overlay_call_targets.ts` reports zero prologue targets
 * for this whole overlay).
 *
 * Link base 0x02008000, proved by that same table before any disassembly: its
 * six words 0x02008309, 0x02008031, 0x02008075, 0x0200807d, 0x020080e5 and
 * 0x02008071 are ALL odd and all land on function starts at `word - 0x8000`,
 * and three of those starts — 0x0308, 0x0030 and 0x00e4 — are already banked
 * byte-exact as `assets/code/resource_3a9_c_02000308.c`, `..._02000030.c` and
 * `..._020000e4.c`.  Six parity witnesses plus three banked cross-checks from
 * one `overlay_show <ov> 0 -n 96`.  Under the same base the four EVEN words
 * this owner loads (0x0200879c, 0x020088d4, 0x02008a0c, 0x02008784) are
 * in-image DATA at file offsets 0x079c, 0x08d4, 0x0a0c and 0x0784.
 *
 * Complete owner.  Prologue `push {r5, lr}` at 0x0200007c; single epilogue
 * `pop {r5} / pop {r1} / bx r1` at 0x020000c2.  The popped register is r1, not
 * r0, so **r0 survives and IS the result** — the owner returns the selected
 * table.  Code runs 0x0200007c..0x020000c7; the literal pool is
 * 0x020000c8..0x020000e3 and 0x020000e4 is the next (banked) prologue.  104
 * bytes, matching the inventory row.
 *
 * One call site, resolved with `bun tools/overlay_call_targets.ts resource_3a9
 * 007c`: 0x020000ae -> veneer 0x0420 -> Func_0808a038, reached only on the
 * 0x64 map.  The other two arms return their table without installing it, and
 * that asymmetry is preserved.
 *
 * `Data_02000240 + 448` and `+ 450` are the map id and sub-state halfwords of
 * the shared work area — the same two slots the banked
 * `assets/code/resource_3a9_c_02000308.c` reads as `Data_02000240[224]` and the
 * banked `resource_36f` sources read as `[225]`, so the offsets are evidence
 * rather than inference.  0x64 is likewise the map id that banked source
 * compares against.
 *
 * The sub-state window `>= 9 && (<= 15 || == 17)` is a three-compare test with
 * a shared taken-arm, not a range: 16 is excluded from the middle of it.
 * Writing it as `9..17` would fold that hole away.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_0808a038();           /* install a script table */

/* RAM: the shared work area. */
extern s16 Data_02000240[];

/* In-image script tables. */
extern u8 Data_02008784[];
extern u8 Data_0200879c[];
extern u8 Data_020088d4[];
extern u8 Data_02008a0c[];

u8 *Func_0200007c(void)
{
    if (Data_02000240[224] == 0x64) {
        s16 sub = Data_02000240[225];
        u8 *table;

        if (sub >= 9 && (sub <= 15 || sub == 17)) {
            table = Data_020088d4;
        } else {
            table = Data_0200879c;
        }

        Func_0808a038(table);
        return table;
    }

    if (Data_02000240[224] == 0x65) {
        return Data_02008a0c;
    }

    return Data_02008784;
}
