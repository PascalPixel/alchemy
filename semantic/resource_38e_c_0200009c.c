typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_38e owner at 0x0200009c, 92 bytes: exported ENTRY 3 - hand the
 * caller the scene-script table for this visit, patching two of its bytes first
 * when the corresponding event flags are set.
 *
 * The image's offset 0 is an exported-entry veneer table
 * (`ldr r4,[pc,#0] / bx r4 / .word <target>`); its fourth word is 0x0200809d,
 * which under the proven 0x02008000 link base is file offset 0x009c plus the
 * Thumb bit.  So this owner is a root, reached from outside the overlay, and
 * `bun tools/overlay_call_targets.ts resource_38e` confirms no `bl` inside the
 * overlay targets it.
 *
 * Complete owner: `push {lr}` at 0x0200009c through the single epilogue at
 * 0x020000da.  It ends `pop {r1} / bx r1`, so r0 survives and IS the result -
 * and both arms set r0 to a table address immediately before reaching it.
 *
 * Pool map from a control-flow walk: 0x0200009c-0x020000dd is reached as
 * instructions, 0x020000de-0x020000f7 is not - one alignment halfword plus six
 * words (0x02000240, 0x00000022, 0x0000084f, 0x02008c7c, 0x00000845,
 * 0x02008c64).  66 code + 26 pool = 92, exactly the inventory row's figures.
 *
 * Calls: 2 sites, matching the advertised 2:
 *   Func_080770c0  x2   0xb2 0xc4
 *
 * Both returned addresses are EVEN, so under the proven link base they are
 * in-image DATA, not functions: file offsets 0x0c7c and 0x0c64.  The overlay's
 * data band there holds three-word scene-script records of the shape
 * (selector/flag, callback, parameter) - the same table that names this
 * overlay's 0x020001e4 and 0x0200045c owners through the odd words 0x020081e5
 * and 0x0200845d.  So the two arms select between two script tables, and
 * 0x02000240[224] == 0x22 - the same scene test the entry-0 root at 0x020004bc
 * makes - is what chooses.
 *
 * The two patched bytes are inside the selected table, at 0x02008c7c + 118 and
 * 0x02008c7c + 70, i.e. file offsets 0x0cf2 and 0x0cc2.  Writing into the
 * overlay image is deliberate here: the flags decide once, at table-hand-out
 * time, rather than at every record.
 *
 * 0x02000240 is the shared cross-overlay scene workspace and index 224 is the
 * documented cell.  It is a RAM address, not a file offset - it is below the
 * 0x02008000 link band.
 */

/* The shared cross-overlay scene workspace. */
extern s16 Data_02000240[];

/* Old-style declaration: overlay imports vary in arity between call sites. */
s32 Func_080770c0();                 /* read an event flag */

u8 *Func_0200009c(void)
{
    u8 *table;

    if (Data_02000240[224] != 0x22) {
        return (u8 *)0x02008c64;
    }

    table = (u8 *)0x02008c7c;

    if (Func_080770c0(0x84f) != 0) {
        table[118] = 1;
    }

    if (Func_080770c0(0x845) != 0) {
        table[70] = 0;
    }

    return table;
}
