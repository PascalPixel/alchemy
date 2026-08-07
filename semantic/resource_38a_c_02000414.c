#include "types.h"

/*
 * resource_38a owner at 0x02000414, 176 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay/overlay_driver.ts resource_38a), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Body: write 516 to the workspace's +448 s32, then do nothing at all
 * unless the scene id s16 at 0x02000240+448 is 28.  For that scene,
 * sub-selector 5 only raises flag 0x12f and stops; every other
 * sub-selector sets bit 4 of record 8's +89 and then, only if flag
 * 0x864 is set, places record 8 at (0x15a0000, 0x1240000), seats it
 * with Func_080091e0(record, 0), sets bit 1 of its +35, puts it in
 * mode 2 and pops panel (19, 74, 9, 3, 19, 17).  Returns 0 on every
 * path.
 *
 * Complete owner: `push {lr}` at 0x02000414 with `sub sp, #8` through
 * `movs r0, #0 / add sp, #8 / pop {r1} / bx r1` at
 * 0x020004a6-0x020004ac, alignment halfword, then the five-word
 * literal pool 0x020004b0-0x020004c3 (0x03001ebc, 0x02000240, 0x1c,
 * 0x12f, 0x864); the next function -- a four-instruction leaf that
 * adds a constant to +30 of the record at +80 -- follows at
 * 0x020004c4.  Returns a literal 0.
 *
 * The scene id and sub-selector are the +448 and +450 s16 of the same
 * 0x02000240 global that resource_39e, resource_3a4 and resource_3a6
 * read; +448 selects the scene, +450 sub-selects within it.
 *
 * Uncertainty: the flag ids and the scene id 28 are literal; record
 * fields +35, +89 are recorded by offset with roles open.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;

extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091e0(void *record, s32 arg1);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a100(s32 id, s32 arg1);

s32 Func_02000414(void)
{
    *(s32 *)(Data_03001ebc + 448) = 516;
    if (Data_02000240[224] != 28) {
        return 0;
    }
    if (Data_02000240[225] == 5) {
        Func_080770d0(0x12f);
        return 0;
    }
    Func_0808a080(8)[89] |= 16;
    if (Func_080770c0(0x864) == 0) {
        return 0;
    }
    Func_0808a0f0(8, 0x15a0000, 0x1240000);
    Func_080091e0(Func_0808a080(8), 0);
    Func_0808a080(8)[35] |= 2;
    Func_0808a100(8, 2);
    Func_080091c0(19, 74, 9, 3, 19, 17);
    return 0;
}
