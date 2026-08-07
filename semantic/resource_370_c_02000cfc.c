#include "types.h"

/*
 * Resource 370 tilemap block writer at 0x02000cfc.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, r8 / push {r7}` at
 * 0x02000cfc through `pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} /
 * bx r0` at 0x02000d6a.  The return address is popped into r0, so this owner
 * returns nothing (HANDOVER section 0, epilogue rule).
 *
 * Literal pool inside the span: 0x02000d34-0x02000d3f, three words
 * (0xfe9af001, 0x03001e8c, 0x06002000), jumped over by the `b.n 0x02000d40`
 * at 0x02000d32 and spelled as `.4byte`/`.2byte` in
 * `assets/code/resource_370_overlay.s`.  The first word disassembles as a
 * plausible `bl 0x02002a6c` and is data, not a call.  The owner has exactly
 * three real call sites, matching the inventory's calls=3.
 *
 * Targets resolved with `bun tools/lib/overlay_call_targets.ts resource_370`:
 * 0x02000d0e -> Func_08000170, 0x02000d18 -> Func_080001a8,
 * 0x02000d66 -> Func_08000178.
 *
 * This is the near-twin of the owner at 0x02000d74 in the same overlay: same
 * destination geometry, same shadow copy, same acquire/release pair.  The two
 * differences are that this one publishes its second argument through
 * Func_080001a8 before writing, and that the entry ids are laid out from the
 * source sheet width at placement offset +8 instead of a fixed 16-wide run.
 *
 * Geometry: the destination is the 32-entry-wide BG screen block at
 * 0x06002000, starting at halfword index `placement[14] * 32 + placement[12]`;
 * 16 entries are written per row for 8 rows, with the same values mirrored
 * into the shadow map whose base is the IWRAM pointer cell at 0x03001e8c.
 *
 * Uncertainty: as in the twin, the OR constant's observable part is its low
 * halfword 0xf001 (palette 15 plus bit 0); the surrounding `lsls #16 /
 * asrs #16` is discarded by the `strh`.
 */

/* Overlay imports (via the veneer table).  Old-style declarations: overlay
 * imports vary their argument count between call sites. */
s32 Func_08000170();
void Func_080001a8();
void Func_08000178();

struct MapPlacement_02000cfc {
    u16 unknown_00[4];
    u16 sheet_width; /* +8: entry stride between successive source rows */
    u16 unknown_0a;
    u16 column;      /* +12 */
    u16 row;         /* +14 */
};

void Func_02000cfc(struct MapPlacement_02000cfc *placement, void *source)
{
    u16 *screen;
    u16 *shadow;
    s32 handle;
    s32 entry_offset;
    u32 row;
    u32 column;

    /* Read before the first call, so it is not part of that call. */
    shadow = *(u16 **)0x03001e8c;

    handle = Func_08000170(768);
    Func_080001a8(source, handle);

    entry_offset = (((s32)placement->row << 5) + (s32)placement->column) * 2;

    screen = (u16 *)((unsigned char *)0x06002000 + entry_offset);
    shadow = (u16 *)((unsigned char *)shadow + entry_offset);

    for (row = 0; row <= 7; row++) {
        for (column = 0; column <= 15; column++) {
            u16 value = (u16)(((u32)placement->sheet_width * (u32)row +
                               (u32)column) | 0xfe9af001u);

            screen[column] = value;
            shadow[column] = value;
        }
        /* Advance one full 32-entry row (16 written + 16 skipped). */
        shadow += 32;
        screen += 32;
    }

    Func_08000178(handle);
}
