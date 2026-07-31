typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 370 tilemap block writer at 0x02000d74.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000d74 through
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000ddc.  The return address is
 * popped into r0, so nothing is returned: this owner is `void`
 * (HANDOVER section 0, epilogue rule).
 *
 * Literal pool inside the span: 0x02000da4-0x02000daf, three words
 * (0xfed2f001, 0x03001e8c, 0x06002000).  The `b.n 0x02000db0` at 0x02000da2
 * jumps over them, and `assets/code/resource_370_overlay.s` spells them as
 * `.4byte`/`.2byte`.  The first word disassembles as a plausible
 * `bl 0x02002b4c`; it is data, not a call.  Only two real call sites exist,
 * matching the inventory's calls=2.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_370`:
 * 0x02000d80 -> veneer at file offset 0x133c -> Func_08000170, and
 * 0x02000dd8 -> veneer at 0x1344 -> Func_08000178.  They bracket the writes,
 * with the first one's return value carried in r7 into the second, so they
 * read as an acquire/release (or begin/end transfer) pair over the affected
 * video memory.
 *
 * Geometry.  The destination is the 32-entry-wide BG screen block at
 * 0x06002000: the entry offset is `map[14] * 32 + map[12]` halfwords, the
 * inner loop writes 16 consecutive entries, and the outer loop advances by a
 * full 32-entry row eight times.  The identical entries are also written to a
 * shadow copy whose base is the IWRAM pointer cell at 0x03001e8c.
 *
 * Uncertainty: the OR constant's low halfword is 0xf001 (palette 15 plus bit
 * 0).  Only the low 16 bits reach memory, because the value is stored with
 * `strh`; the `lsls #16 / asrs #16` pair before the store is therefore not
 * observable.  Because bit 0 is set, the even tile ids in the 32.. run collapse
 * onto their odd successors -- that is what the bytes do, and it is reproduced
 * rather than "corrected".
 */

/* Overlay imports (via the veneer table).  Old-style declarations: overlay
 * imports vary their argument count between call sites. */
s32 Func_08000170();
void Func_08000178();

struct MapPlacement_02000d74 {
    /* Only +12 and +14 are read by this owner. */
    u16 unknown_00[6];
    u16 column;   /* +12 */
    u16 row;      /* +14 */
};

void Func_02000d74(struct MapPlacement_02000d74 *placement)
{
    u16 *screen;
    u16 *shadow;
    s32 handle;
    s32 entry_offset;
    s32 row;
    s32 column;
    s32 tile;

    /* Loaded before the call, so the pointer read is not part of it. */
    shadow = *(u16 **)0x03001e8c;

    handle = Func_08000170(768);

    /* Halfword index into a 32-entry-wide screen block, converted to bytes. */
    entry_offset = (((s32)placement->row << 5) + (s32)placement->column) * 2;

    screen = (u16 *)((unsigned char *)0x06002000 + entry_offset);
    shadow = (u16 *)((unsigned char *)shadow + entry_offset);

    for (row = 0; row <= 7; row++) {
        tile = row * 16 + 32;
        for (column = 0; column <= 15; column++) {
            u16 value = (u16)((u32)(tile + column) | 0xfed2f001u);

            screen[column] = value;
            shadow[column] = value;
        }
        /* The pointers advance one 32-entry row (16 written + 16 skipped). */
        screen += 32;
        shadow += 32;
    }

    Func_08000178(handle);
}
