#include "types.h"

/*
 * resource_36f owner at 0x020001c0, 120 bytes: decode this overlay's portrait
 * graphic into a scratch buffer and push it at the hardware.
 *
 * Complete owner.  Prologue `push {r5, r6, lr}` at 0x020001c0; single epilogue
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x0200021c, so the popped word is the
 * return address and the owner is **void** (HANDOVER's interworking-epilogue
 * rule).  Code runs 0x020001c0..0x02000221; the literal pool is
 * 0x02000224..0x02000237 after a 2-byte alignment word, and 0x02000238 is the
 * next owner's prologue.  120 bytes exactly, matching the inventory row.
 *
 * Link base 0x02008000 is proved for this overlay by its offset-0 export table:
 * six `ldr r4,[pc,#0] / bx r4` veneers whose words are 0x02008055, 0x02008031,
 * 0x0200803d, 0x02008045, 0x0200804d and 0x02008039 — every one of them odd and
 * landing on a function start at `word - 0x8000`, and 0x02008055 is
 * `Func_02000054 + 1`, whose byte-exact source is already tracked as
 * `assets/code/resource_36f_c_02000054.c`.  A seventh entry at offset 0x30 is
 * the constant-loader flavour (`ldr r0,[pc,#0] / bx lr`) returning the *data*
 * address 0x020085f8.  So the pool word 0x02008650 read here is the in-image
 * halfword at file offset 0x650, not a RAM global.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_36f
 * 01c0` (an overlay `bl` stores target offset - 2; the disassembler's
 * annotation is wrong).  Six sites, six distinct import veneers:
 *   0x020001c6 -> 0x0548 -> Func_08000170   reserve 1312 bytes of scratch
 *   0x020001da -> 0x0568 -> Func_080001d0   allocate a palette slot
 *   0x020001e2 -> 0x0578 -> Func_08000290   look up archived asset 0x1c
 *   0x020001e8 -> 0x0558 -> Func_080001a8   decompress asset into the scratch
 *   0x02000204 -> 0x0560 -> Func_080001c8   upload the decoded tiles
 *   0x02000218 -> 0x0550 -> Func_08000178   release the scratch
 * The decompress/upload/release trio is the same sequence the tracked
 * `semantic/overlays/resource_3bb_c_020039fc.c` and
 * `semantic/overlays/resource_370_c_02000cfc.c` use, which is where the roles
 * of Func_08000170/0x1a8/0x1d0/0x1c8/0x178 come from — they are not inferred
 * here.
 *
 * The palette slot is *cached* in the in-image halfword at 0x02008650: the
 * sentinel is -1 (built as `movs r2,#1 / negs r2` and compared signed after an
 * `ldrsh`), and only the first entry into the overlay allocates.  The same
 * halfword is re-read for the tile upload, so an already-cached slot is reused.
 *
 * The 32 bytes at the head of the scratch are the decoded palette and go
 * straight to palette RAM by DMA3 (`stmia r3!,{r0,r1,r2}` with r3 = 0x040000d4,
 * control 0x84000008 = enable | 32-bit | 8 words); the tiles start 32 bytes in.
 * The `subs r3,#12` after the burst merely rewinds the auto-incremented
 * register and is dead, exactly as HANDOVER records for the DMA3 idiom.  The
 * `while (DMA3CNT & 0x80000000)` spin below is that same transfer being waited
 * on before the scratch is released.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
            /* reserve N bytes of scratch, return handle */
           /* release a scratch handle */
           /* decompress an archived asset into scratch */
            /* allocate a palette slot */
           /* upload decoded tiles for a palette slot */
            /* archived asset by id */

/* In-image halfword: the cached palette slot for this overlay's portrait. */
extern s16 Data_02008650[];

extern s32 Func_02000710();
extern s32 Func_02000744();
extern u8 * Func_0200075c();
extern void Func_02000742();
extern void Func_02000766();
extern void Func_0200076a();
void Func_020001c0(void)
{
    s32 handle;
    u8 *source;

    handle = Func_02000710(1312);       /* 164 << 3 == 32 palette + 1280 tile */

    if (Data_02008650[0] == -1) {
        Data_02008650[0] = (s16)Func_02000744();
    }

    source = Func_0200075c(0x1c);
    Func_02000742(source, handle);

    /* DMA3: 8 words of palette from the head of the scratch. */
    *(volatile u32 *)0x040000d4 = (u32)handle;
    *(volatile u32 *)0x040000d8 = 0x050003e0;
    *(volatile u32 *)0x040000dc = 0x84000008;

    Func_02000766(Data_02008650[0], 1280, handle + 32);   /* 160 << 3 */

    while ((*(volatile u32 *)0x040000dc & 0x80000000) != 0) {
    }

    Func_0200076a(handle);
}
