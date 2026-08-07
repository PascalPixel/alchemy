#include "types.h"

/*
 * BYTE-EXACT and adopted 2026-08-07 with -fthumb-sink-pool-load-to-use: the
 * last residuals were the two `ldr r0, .L' argument loads at 0x02000190 and
 * 0x020001ac, which the reference fetches after the pair of narrowing shifts
 * that set r1 and r2, while the post-reload scheduler hoists them above.
 */

/*
 * resource_3c6 scene-entry step at 0x02000158, 152 bytes
 * (0x02000158-0x020001ef).
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000158 through the single
 * interworking epilogue `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x020001d8.
 * 0x020001de is a two-byte alignment `nop`; 0x020001e0-0x020001ef is this
 * owner's literal pool (0x03001ebc, 0x02009ca8, 0x02009cee, 0x02009cd8),
 * past the `bx` and never reached as an instruction.  The epilogue pops the
 * return address into r0, so the owner is void.  Nothing reads r0-r3 before
 * they are first written, so the owner takes no arguments.
 *
 * LINK BASE 0x02008000 (witnesses recorded in resource_3c6_c_02000078.c), so
 * the even pool words 0x02009ca8/0x02009cd8/0x02009cee are in-image data at
 * file offsets 0x1ca8/0x1cd8/0x1cee, not RAM globals.  0x03001ebc is the
 * overlay's IWRAM workspace pointer cell, the same one the byte-exact
 * resource_3c6_c_020001f0.c reads, and its +0x16c halfword and +0x1c8 word
 * are the same two fields that source uses.
 *
 * Control flow: one two-armed conditional on the +0x16c scene index, joined
 * at 0x020001be.  The arms are the same shape with different constants, so
 * the shared argument computation (the s16 pair loaded from the 0x02009ca8
 * table) is written once above the branch, exactly as the assembly does.
 *
 * Call accounting: 8 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3c6 0158` - 7 distinct targets,
 * all import veneers, no prologue and no `call_via` site.  The multiset of
 * `Func_...(` calls below equals that multiset: Func_08009178 x2 (one per
 * arm) and one each of Func_080f9010, Func_0808a010, Func_0808a080,
 * Func_0808a0d8, Func_0808a248, Func_0808a578.  (The inventory's `calls=8`
 * agrees here.)
 *
 * UNCERTAINTY 1: the pair loaded from the 0x02009ca8 table is read with
 * `ldrsh` and then masked back to 16 bits by `lsls #16 / lsrs #16` before the
 * call, so the sign extension is discarded and the arguments are the raw
 * unsigned halfwords.  Written that way below; whether the table itself is
 * signed is not decidable from this owner.
 * UNCERTAINTY 2: Func_0808a578 and Func_0808a0d8 are called with the same
 * (0, mode, -16) shape in the two arms and differ only in identity and mode
 * (0 versus 2).  They are plainly a related pair, but their interfaces are
 * not established here.
 */

/* Imports, reached through this overlay's veneer table.  Old-style
 * declarations: their interfaces vary by call site across the overlay. */
           /* load/queue an in-image scene resource */
           /* audio/sequence cue by id */
           /* wait n frames */
            /* scene record by slot selector */
           /* entry transition, mode 2 */
           /* scene-index handoff */
           /* entry transition, mode 0 */

extern u8 *Data_03001ebc;       /* overlay workspace pointer, IWRAM cell */

extern s16 Data_02009ca8[];     /* s16 (x, z) pairs, file offset 0x1ca8 */
extern u8 Data_02009cd8[];      /* scene resource, file offset 0x1cd8 */
extern u8 Data_02009cee[];      /* scene resource, file offset 0x1cee */

extern u8 * Func_020016c6();
extern void Func_020017b2();
extern void Func_02001684();
extern void Func_020017c0();
extern void Func_020016a0();
extern void Func_02001734();
extern void Func_020016ea();
extern void Func_020017ce();
void Func_02000158(void)
{
    u8 *workspace = Data_03001ebc;
    s32 scene = *(s16 *)(workspace + 0x16c);
    s16 x = Data_02009ca8[scene * 2];
    s16 z = Data_02009ca8[scene * 2 + 1];

    Func_020016c6(0)[0x55] = 2;
    Func_020017b2(158);

    if (scene == 6) {
        Func_02001684(Data_02009cee, (u16)x, (u16)z);
        Func_020017c0(0, 0, -16);
    } else {
        Func_020016a0(Data_02009cd8, (u16)x, (u16)z);
        Func_02001734(0, 2, -16);
    }

    Func_020016ea(10);
    *(s32 *)(Data_03001ebc + 0x1c8) = 16;
    Func_020017ce(scene);
}
