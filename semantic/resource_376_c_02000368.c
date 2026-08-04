#include "types.h"

/*
 * resource_376 owner at 0x02000368, 372 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay_driver.ts resource_376), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Opens by writing 521 to the workspace's +448 s32 (the recurring
 * 'displacement 448 / adds #73' additive store), then three
 * independent gates:
 *
 * - flag 0x834 set: park records 8 through 22 at the origin -- fifteen
 *   Func_0808a0f0(id, 0, 0) calls, unrolled in the reference and left
 *   unrolled here -- then Func_0808a2c8, set to 1 the u16 at +0x1f84
 *   of the second pointer in the cell block, and run Func_0808a2d8,
 *   Func_080000c0(30), Func_0808a360, Func_0808a370, Func_0808a2e0;
 * - flag 0x87a set: with sub-selector 6 and flag 0x81d clear run
 *   Func_02000658; then set bit 7 of record 10's +89;
 * - sub-selector 2 with flag 0x815 set: place record 13 at
 *   (0x1c60000, 0x960000), seat it with Func_080091e0(record, 0), put
 *   it in mode 5 and run Func_08009188(4).
 *
 * Returns 0.
 *
 * Complete owner: `push {r5, lr}` at 0x02000368 through `movs r0, #0 /
 * pop {r5} / pop {r1} / bx r1` at 0x020004b6-0x020004bc, alignment
 * halfword, then the seven-word literal pool 0x020004c0-0x020004db;
 * the next function's `push {lr}` follows at 0x020004dc.  Returns a
 * literal 0.
 *
 * The second pointer at 0x03001ec8 is spelled `((u8 **)&Data_03001ebc)[3]`
 * per the resource_377_c_020003f8.c precedent, which reads the same
 * cell and sets the same u16 at +0x1f84 to 1.
 *
 * Cross-checked per the 2026-08-01 ruling: tools/m2c_guard.ts clears
 * this row of the call-through-register hazard and reports full call
 * coverage against this draft.
 *
 * Uncertainty: flag ids and sub-selector values are literal; the role
 * of the +0x1f84 u16 and of record 10's +89 bit 7 are open.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;

extern void Func_02000658(void);

extern void Func_080000c0(s32 arg0);
extern void Func_08009188(s32 arg0);
extern void Func_080091e0(void *record, s32 arg1);
extern s32 Func_080770c0(s32 flagId);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a2c8(void);
extern void Func_0808a2d8(void);
extern void Func_0808a2e0(void);
extern void Func_0808a360(void);
extern void Func_0808a370(void);

s32 Func_02000368(void)
{
    *(s32 *)(Data_03001ebc + 448) = 521;
    if (Func_080770c0(0x834) != 0) {
        Func_0808a0f0(8, 0, 0);
        Func_0808a0f0(9, 0, 0);
        Func_0808a0f0(10, 0, 0);
        Func_0808a0f0(11, 0, 0);
        Func_0808a0f0(12, 0, 0);
        Func_0808a0f0(13, 0, 0);
        Func_0808a0f0(14, 0, 0);
        Func_0808a0f0(15, 0, 0);
        Func_0808a0f0(16, 0, 0);
        Func_0808a0f0(17, 0, 0);
        Func_0808a0f0(18, 0, 0);
        Func_0808a0f0(19, 0, 0);
        Func_0808a0f0(20, 0, 0);
        Func_0808a0f0(21, 0, 0);
        Func_0808a0f0(22, 0, 0);
        Func_0808a2c8();
        *(u16 *)(((u8 **)&Data_03001ebc)[3] + 0x1f84) = 1;
        Func_0808a2d8();
        Func_080000c0(30);
        Func_0808a360();
        Func_0808a370();
        Func_0808a2e0();
    }
    if (Func_080770c0(0x87a) != 0) {
        if (Data_02000240[225] == 6 && Func_080770c0(0x81d) == 0) {
            Func_02000658();
        }
        Func_0808a080(10)[89] |= 0x80;
    }
    if (Data_02000240[225] == 2 && Func_080770c0(0x815) != 0) {
        Func_0808a0f0(13, 0x1c60000, 0x960000);
        Func_080091e0(Func_0808a080(13), 0);
        Func_0808a100(13, 5);
        Func_08009188(4);
    }
    return 0;
}
