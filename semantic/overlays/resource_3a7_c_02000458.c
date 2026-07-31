/*
 * resource_3a7 tile-cell predicate at 0x02000458, 52 bytes.
 *
 * Complete owner: `push {lr}` at 0x02000458 through `pop {r1} / bx r1` at
 * 0x02000484.  The popped register is r1, NOT r0, so r0 survives the return and
 * IS the result (HANDOVER: "the interworking epilogue tells you the return
 * type").  The owner therefore returns s32, which agrees with its two callers -
 * 0x0200048c and 0x020004d0 both test its result with `cmp r0,#0 / beq`.
 * The word at 0x02000488 (0x000fffff) is this owner's literal pool, past the
 * epilogue and inside the 52-byte span; it is data, not code.
 *
 * Call targets resolved with
 *   bun tools/overlay_show.ts resource_3a7 0458 -n 52 |
 *   bun tools/overlay_call_targets.ts resource_3a7 --annotate
 * One site, matching the inventory's calls=1:
 *   Func_0808a080 x1 <- veneer 0x02001130   (scene-entity record by selector)
 *
 * `>> 20` on the 16.16 coordinates at +8 and +16 is the tile-grid idiom: `>> 16`
 * to whole pixels, then `>> 4` for the 16-pixel grid.  The `if (v < 0) v +=
 * 0xfffff` before each shift is the round-toward-zero bias an arithmetic shift
 * would otherwise get wrong for negative coordinates, so the pair is one
 * truncating division by 0x100000 and is written that way here.
 *
 * The record's coordinate layout - +8 is X and +16 is Z - matches the byte-exact
 * sibling assets/code/resource_3a7_c_020006e4.c, which reads the same record
 * through an `s32 f00/f04/f08/f0c` view and branches on its +12 word.
 *
 * Uncertainty: which axis is which is taken from the overlay's convention (+8 X,
 * +16 Z, +12 Y) rather than proven here; only the cell test (X cell 16, Z cell
 * 12) is evidence.  Nothing depends on the naming.
 */

/* Old-style declaration: overlay imports vary their argument count between
 * call sites, but this one is used for its result. */
typedef signed int s32;

s32 *Func_0808a080();   /* scene-entity record by selector */

s32 Func_02000458(void)
{
    s32 *record = Func_0808a080(10);
    s32 z = record[4];      /* +16, 16.16 */
    s32 x = record[2];      /* +8,  16.16 */
    s32 zCell;
    s32 xCell;

    /* Truncating >> 20: whole pixels then the 16-pixel tile grid. */
    if (z < 0) {
        z += 0x000fffff;
    }
    zCell = z >> 20;

    if (x < 0) {
        x += 0x000fffff;
    }
    xCell = x >> 20;

    if (xCell == 16 && zCell == 12) {
        return 1;
    }
    return 0;
}
