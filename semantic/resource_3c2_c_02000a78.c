#include "types.h"

/*
 * Resource 3c2 overlay entry point 0 at 0x02000a78.
 *
 * Overlay layout, established before drafting (see the link-base note below):
 *   0x0000-0x002f  six 8-byte EXPORT veneers (`ldr r4,[pc,#0] / bx r4 /
 *                  .word <entry>+1`).  Entry 0 is this owner:
 *                    0x0000 -> 0x02008a79 = Func_02000a78 + Thumb bit
 *                    0x0008 -> 0x02008031 = Func_02000030 + 1
 *                    0x0010 -> 0x02008041 = Func_02000040 + 1
 *                    0x0018 -> 0x02008049 = Func_02000048 + 1
 *                    0x0020 -> 0x02008a55 = Func_02000a54 + 1
 *                    0x0028 -> 0x02008039 = Func_02000038 + 1
 *   0x0030-0x0047  three `ldr r0,[pc,#0] / bx lr` data getters returning the
 *                  in-image tables at file offsets 0xc3c / 0xda4 / 0xdd4.
 *   0x0048-0x0b33  overlay code.
 *   0x0b34-0x0c3b  import veneer table (8 bytes per entry).
 *   0x0c3c-        in-image data.
 *
 * Link base: 0x02008000, proven three ways.  0x02008a79 and 0x02008049 are
 * known function starts (0x0a78, 0x0048) plus the Thumb bit, and the even
 * words 0x02008c3c / 0x02008da4 / 0x02008dd4 land inside the 0x13cc-byte image
 * at value - 0x8000.  Note 0x02000240 below is therefore NOT in-image: it is
 * the cross-overlay RAM global block at a fixed RAM address, matching its use
 * in other overlays (resource_370 spells it `(u8 *)0x02000240` too).  It is
 * unrelated to this overlay's own code at file offset 0x240.
 *
 * Owner is complete: `push {r5, r6, lr}` + `push {r5, r6}` (sl, r8) prologue
 * at 0x02000a78, single interworking epilogue at 0x02000b28.  A literal pool
 * occupies 0x02000b18-0x02000b27, i.e. it ends mid-row immediately before that
 * epilogue; the `b.n 0x02000b28` at 0x02000b16 jumps over it.
 *
 * Return type: the epilogue is `pop {r1} / bx r1`, N != 0, so r0 survives and
 * is the result.  r0 is set to 0 on the only path, so the owner returns 0.
 *
 * All five call sites resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3c2 0a78 --json`:
 *   0x02000a92 -> veneer 0x0b3c -> Func_080770c8
 *   0x02000aae -> veneer 0x0b64 -> Func_0808a080
 *   0x02000ac2 -> veneer 0x0b64 -> Func_0808a080
 *   0x02000ad0 -> veneer 0x0b64 -> Func_0808a080
 *   0x02000af8 -> veneer 0x0b64 -> Func_0808a080
 * Per target: Func_080770c8 x1, Func_0808a080 x4; 5 = 5 sites.
 *
 * What it does: if the scene selector halfword is 90, raise flag 0x96f, then
 * seed two words of the 0x03001ebc workspace and reconfigure four scene
 * records (12, 13, 20, 21).
 *
 * Uncertainties.  The meanings of workspace words +448/+456, of record byte
 * +89 bit 2, and of the +0x50 sub-record's fields +9, +30 and +38 are not
 * determined here; they are transcribed exactly.  The original materialises
 * the constant 0 twice (sl and, from the pool word at 0x02000b18, r8); both
 * are written as 0.
 */

/* Overlay imports (through the veneer table).  Old-style declarations:
 * overlay imports vary their argument count between call sites. */
void Func_020015d0();
u8 *Func_02001614();
u8 *Func_02001628();
u8 *Func_02001636();
u8 *Func_0200165e();

/* Pointer CELL, not the workspace itself: the original does
 * `ldr r3,[pc] / ldr r1,[r3]`. */
extern u8 *Data_03001ebc;

static void Configure_02000a78(u8 *record)
{
    u8 *sub;

    sub = (u8 *)*(void **)(record + 0x50);
    sub[38] = 0;

    sub = (u8 *)*(void **)(record + 0x50);
    *(s16 *)(sub + 30) = 0x4000;
}

s32 Func_02000a78(void)
{
    u8 *workspace;
    u8 *record;
    u8 *sub;

    if (*(s16 *)((u8 *)0x02000240 + 450) == 90) {
        Func_020015d0(0x96f);
    }

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 521;
    *(s32 *)(workspace + 456) = 24;

    record = Func_02001614(12);
    record[89] |= 4;

    record = Func_02001628(13);
    record[89] |= 4;

    record = Func_02001636(20);
    Configure_02000a78(record);
    /* The +0x50 sub-record is reloaded a third time for the field-9 update. */
    sub = (u8 *)*(void **)(record + 0x50);
    sub[9] = (u8)((sub[9] & ~0x0c) | 4);

    record = Func_0200165e(21);
    Configure_02000a78(record);
    record[85] = 2;
    *(s32 *)(record + 12) = 0;

    return 0;
}
