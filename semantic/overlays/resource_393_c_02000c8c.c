typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_393 owner at 0x02000c8c, 104 bytes: recolour the first 224 hardware
 * palette entries through 0x02000cf4 with the caller's selector, leaving two
 * reserved sub-ranges untouched, then snapshot and publish the result.
 *
 * Complete owner: 'push {r5, r6, r7, lr}' and the single interworking epilogue
 * at 0x02000ce6.  Control-flow walk: one backward-branch loop plus two forward
 * skips, all landing before 0x02000cec, so the 8-byte pool at 0x02000cec
 * (0xffef0000, 0x0000ff3f) is code-unreachable.  96 code + 8 pool = 104, the
 * advertised span.
 *
 * Return type from the epilogue rule: 'pop {r0} ; bx r0', so the owner is void.
 *
 * Calls: 5 sites over 5 targets, from
 * 'bun tools/overlay_call_targets.ts resource_393 0c8c'.  0x02000d5c and
 * 0x02000d7c are reported 'unknown' only because they are two-instruction DMA3
 * leaves with no 'push' - both are ordinary balanced calls that end in 'bx lr'.
 * 0x02000d5c banks the work buffer to the overlay's own 0x020090e0 (file offset
 * 0x10e0 under the proven 0x02008000 base) and 0x02000d7c to 0x020097e0; the
 * overlay image is writable EWRAM, so these are real stores, not constants.
 *
 * The loop counter is carried PRE-SHIFTED: r6 holds index << 16 and the index
 * itself is recovered with 'lsrs #16' at the top of every iteration.  Both
 * guards are therefore 16-bit windowed range tests, not magnitude comparisons:
 *
 *   (index - 0x11) << 16 <= 0x60000   ->  index in [0x11, 0x17]  (skip)
 *   (index + 0xff3f) << 16 <= 0x70000 ->  index in [0xc1, 0xc8]  (skip)
 *
 * Read without undoing the shift, the first looks like a comparison against
 * 0xffef0000 and the second like an unrelated large constant.
 *
 * UNCERTAINTY: what the two reserved windows hold is not established - only
 * that they are excluded.  Their sizes (7 and 8 entries) are read off the
 * bounds, not assumed.
 */

#define PALETTE393 ((u16 *)0x05000000)

u16 Func_02000cf4();                /* recolour one entry (colour, selector) */
void Func_02000d5c();               /* bank the work buffer to 0x020090e0 */
void Func_02000d7c();               /* bank the work buffer to 0x020097e0 */
void Func_02000d9c();               /* snapshot both palette banks */
void Func_0808a330();               /* publish the completed work buffer */

void Func_02000c8c(s32 selector)
{
    u32 shifted = 0;                /* index << 16, exactly as the code carries it */

    Func_02000d5c();

    do {
        u32 index = shifted >> 16;

        if ((u32)(shifted + 0xffef0000) > 0x60000 &&
            (u32)((index + 0xff3f) << 16) > 0x70000) {
            u16 *entry = &PALETTE393[index];

            *entry = Func_02000cf4(*entry, selector);
        }
        shifted += 0x10000;
    } while (shifted <= 0x00df0000);

    Func_02000d9c();
    Func_02000d7c();
    Func_0808a330(0x10000, 0);
}
