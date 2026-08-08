#include "types.h"

/*
 * resource_3be owner at 0x02001394, 240 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools-rs/overlay-driver resource_3be), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Two independent scene gates on the id at 0x02000240+448, and
 * nothing else:
 * - scene 152: raise flag 324, and if flag 0x9a0 is set park actor 11
 *   at the origin;
 * - scene 158: with sub-selector 1, pop panel (108, 17, 1, 1, 107,
 *   17); if flag 0x9a2 is set, place actor 8 at (0x1b80000,
 *   0x1340000), put it in mode 2 and pop panel (29, 19, 1, 1, 27,
 *   19); if flag 0x9a5 is set, park actor 9 and place actor 10 at
 *   (0x2b80000, 0x1200000) in mode 2; then always seat record 12 with
 *   Func_080091e0(record, 0).
 * Returns 0.  Note this driver does NOT write the workspace +448 cell
 * that its siblings open with -- transcribed as compiled.
 *
 * Complete owner: `push {r5, lr}` at 0x02001394 with `sub sp, #8`
 * through `movs r0, #0 / add sp, #8 / pop {r5} / pop {r1} / bx r1` at
 * 0x02001464-0x0200146c, alignment halfword, then the five-word
 * literal pool 0x02001470-0x02001483 (0x02000240, 0x98, 0x9e, 0x9a2,
 * 0x9a5); the overlay's import-veneer table begins at 0x02001484.
 *
 * Cross-checked against m2c per the 2026-08-01 ruling: the guard
 * clears this row of the call-through-register hazard, and m2c agreed
 * on every value, including both fixed-point coordinate pairs.  It
 * rendered the scene id as the absolute 0x02000400 and emitted an
 * invalid `*(void *)` on the second read of it; the named-offset form
 * is used here so the row stays readable against its siblings.
 *
 * Uncertainty: flag ids and scene ids are literal; the panel
 * six-tuples are positional, roles open.
 */

extern s16 Data_02000240[];
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091e0(void *record, s32 arg1);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a100(s32 id, s32 arg1);

s32 Func_02001394(void)
{
    if (Data_02000240[224] == 152) {
        Func_080770c8(324);
        if (Func_080770c0(0x9a0) != 0) {
            Func_0808a0f0(11, 0, 0);
        }
    }
    if (Data_02000240[224] == 158) {
        if (Data_02000240[225] == 1) {
            Func_080091c0(108, 17, 1, 1, 107, 17);
        }
        if (Func_080770c0(0x9a2) != 0) {
            Func_0808a0f0(8, 0x1b80000, 0x1340000);
            Func_0808a100(8, 2);
            Func_080091c0(29, 19, 1, 1, 27, 19);
        }
        if (Func_080770c0(0x9a5) != 0) {
            Func_0808a0f0(9, 0, 0);
            Func_0808a0f0(10, 0x2b80000, 0x1200000);
            Func_0808a100(10, 2);
        }
        Func_080091e0(Func_0808a080(12), 0);
    }
    return 0;
}
