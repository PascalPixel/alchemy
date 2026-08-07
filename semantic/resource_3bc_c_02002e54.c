#include "types.h"

/*
 * resource_3bc owner at 0x02002e54, 148 bytes: decode this overlay's portrait
 * graphic (variant selected by argument) into scratch and push it at the
 * hardware -- the same decompress/upload/release sequence as
 * resource_36f_c_020001c0.c, which is where the roles of
 * Func_08000170/1a8/1c8/1d0/178/08000290 are established (not inferred
 * here).
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7,r8 / push {r7}` at
 * 0x02002e54 through `pop {r3} / mov r8,r3 / pop {r5, r6, r7} / pop {r0} /
 * bx r0` at 0x02002ec6-0x02002ece, followed by the eight-word literal pool
 * 0x02002ed0-0x02002ee7 that closes the 148-byte span; the next owner's
 * prologue is at 0x02002ee8.  One argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed); six call sites,
 * all resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved by hand against overlayImage()/targetOffset()):
 *   0x02002e60 -> veneer 0x02004888 -> Func_08000170(7328)   reserve scratch
 *   0x02002e74 -> veneer 0x020048b8 -> Func_080001d0()       allocate palette slot (only when uncached)
 *   0x02002e88 -> veneer 0x020048d0 -> Func_08000290(0xe7)   archived asset 231
 *   0x02002e8e -> veneer 0x02004898 -> Func_080001a8         decompress into scratch
 *   0x02002eae -> veneer 0x020048b0 -> Func_080001c8         upload decoded tiles
 *   0x02002ec2 -> veneer 0x02004890 -> Func_08000178         release scratch
 *
 * The DMA3 palette burst (`stmia r3!,{r0,r1,r2}`, r3 = 0x040000d4, control
 * 0x84000008 = enable | 32-bit | 8 words) and the busy-wait on DMA3CNT before
 * releasing the scratch handle are the same idiom as the sibling; the
 * `subs r3,#12` afterwards is dead, per HANDOVER's DMA3 note.
 *
 * The argument selects a byte from the in-image table Data_0200ccb0[]
 * (looked up BEFORE the clamp below, so index 8 reads its own real entry)
 * that offsets where in the scratch buffer the palette DMA reads from;
 * separately, the variant is clamped 8 -> 4 before scaling the tile upload's
 * destination (`(variant << 10) + handle + 160`).  Neither the table's
 * values nor the meaning of the variant selector are resolved beyond this.
 */

extern s16 Data_0200d9a4;          /* cached palette slot, sentinel -1 */
extern u8 Data_0200ccb0[];         /* per-variant scratch offset table */

s32 Func_08000170();               /* reserve N bytes of scratch, return handle */
void Func_08000178();              /* release a scratch handle */
void Func_080001a8();              /* decompress an archived asset into scratch */
s32 Func_080001d0();               /* allocate a palette slot */
void Func_080001c8();              /* upload decoded tiles for a palette slot */
u8 *Func_08000290();               /* archived asset by id */

void Func_02002e54(s32 variant)
{
    s32 handle;
    u8 *source;
    u8 tableOffset;

    handle = Func_08000170(0xe5 << 5);   /* 7328 = 32 palette + 224 * 32 tile */

    if (Data_0200d9a4 == -1) {
        Data_0200d9a4 = (s16)Func_080001d0();
    }

    tableOffset = Data_0200ccb0[variant];
    if (variant == 8) {
        variant = 4;
    }

    source = Func_08000290(0xe7);
    Func_080001a8(source, handle);

    /* DMA3: 8 words of palette, source offset by the variant's table byte. */
    *(volatile u32 *)0x040000d4 = (u32)(handle + tableOffset);
    *(volatile u32 *)0x040000d8 = 0x050003e0;
    *(volatile u32 *)0x040000dc = 0x84000008;

    Func_080001c8(Data_0200d9a4, 0x400, (variant << 10) + handle + 160);

    while ((*(volatile u32 *)0x040000dc & 0x80000000) != 0) {
    }

    Func_08000178(handle);
}
