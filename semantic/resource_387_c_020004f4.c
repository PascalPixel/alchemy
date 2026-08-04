typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_387 owner at 0x020004f4, 392 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay_driver.ts resource_387), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Unconditional head: raise flag 324, hold ten ticks, Func_0808a5e0(170),
 * put record 11 in mode 2 and SET its +35 byte to 2 outright, then OR
 * bit 4 into record 8's +89 and bit 3 into record 15's +89.  Note the
 * +35 write here is a plain store, where the later blocks OR into the
 * same field -- transcribed as compiled.
 *
 * Then four flag gates, each drawing its own panel set:
 * - 0x865: panel (74, 11, 1, 1, 73, 11);
 * - 0x860: place record 8 at (0x880000, 0xc40000), OR 2 into its +35,
 *   mode 2, and two panels (39, 12, 3, 1, 8, 12) and (43, 11, 3, 1,
 *   12, 11);
 * - 0x861 else 0x862: place record 9 at (0x1080000, 0x1380000) with
 *   panel (48, 18, 1, 2, 16, 18), or at (0x1180000, 0x1380000) with
 *   panel (47, 18, 1, 2, 16, 18).  These two are mutually exclusive --
 *   the 0x862 test runs only when 0x861 is clear;
 * - 0x863: place record 10 at (0x1780000, 0x1180000), set its +35 to 2
 *   and its +85 to 0, seat it with Func_080091e0(record, 0), and panel
 *   (54, 17, 1, 1, 23, 17).
 *
 * Returns 0.  This driver reads neither the scene id nor the
 * sub-selector -- it is driven purely by story flags, unlike its
 * siblings.
 *
 * Complete owner: `push {r5, lr}` at 0x020004f4 with `sub sp, #8`
 * through `movs r0, #0 / add sp, #8 / pop {r5} / pop {r1} / bx r1` at
 * 0x02000660-0x02000668, alignment halfword, then the four-word
 * literal pool 0x0200066c-0x0200067b (0x865, 0x861, 0x862, 0x863);
 * the next function's `push {lr}` follows at 0x0200067c.  Returns a
 * literal 0.
 *
 * Cross-checked per the 2026-08-01 ruling: tools/m2c_guard.ts clears
 * this row of the call-through-register hazard and reports full call
 * coverage against this draft.
 *
 * Uncertainty: every flag id is literal; record fields +35, +85 and
 * +89 are recorded by offset, roles open; the panel six-tuples are
 * positional.
 */

extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091e0(void *record, s32 arg1);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a5e0(s32 arg0);

s32 Func_020004f4(void)
{
    Func_080770c8(324);
    Func_0808a010(10);
    Func_0808a5e0(170);
    Func_0808a100(11, 2);
    Func_0808a080(11)[35] = 2;
    Func_0808a080(8)[89] |= 16;
    Func_0808a080(15)[89] |= 8;
    if (Func_080770c0(0x865) != 0) {
        Func_080091c0(74, 11, 1, 1, 73, 11);
    }
    if (Func_080770c0(0x860) != 0) {
        Func_0808a0f0(8, 0x880000, 0xc40000);
        Func_0808a080(8)[35] |= 2;
        Func_0808a100(8, 2);
        Func_080091c0(39, 12, 3, 1, 8, 12);
        Func_080091c0(43, 11, 3, 1, 12, 11);
    }
    if (Func_080770c0(0x861) != 0) {
        Func_0808a0f0(9, 0x1080000, 0x1380000);
        Func_080091c0(48, 18, 1, 2, 16, 18);
    } else if (Func_080770c0(0x862) != 0) {
        Func_0808a0f0(9, 0x1180000, 0x1380000);
        Func_080091c0(47, 18, 1, 2, 16, 18);
    }
    if (Func_080770c0(0x863) != 0) {
        Func_0808a0f0(10, 0x1780000, 0x1180000);
        Func_0808a080(10)[35] = 2;
        Func_0808a080(10)[85] = 0;
        Func_080091e0(Func_0808a080(10), 0);
        Func_080091c0(54, 17, 1, 1, 23, 17);
    }
    return 0;
}
