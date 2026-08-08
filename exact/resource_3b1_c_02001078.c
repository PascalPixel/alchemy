#include "types.h"

/*
 * resource_3b1 owner at 0x02001078, 204 bytes.  The executable body starts
 * with `push {r5, lr}` and returns through `pop {r5} / pop {r0} / bx r0` at
 * 0x02001118-0x0200111c.  Alignment at 0x0200111e and nine referenced pool
 * words at 0x02001120-0x02001140 complete the owner immediately before the
 * next prologue at 0x02001144.
 *
 * If story flag 0x300 is set, the owner opens a scripted sequence, creates a
 * local object, shows message 0x1ea6, configures that object as slot 2, moves
 * it to actor 0's signed halfword coordinates when actor 0 exists, then
 * releases and closes the sequence.  The local owners at 0x020012dc,
 * 0x02001190, and 0x0200486c are already exact or reviewed C in this overlay.
 *
 * Otherwise flags 0x92b, 0x92a, and 0x929 select the third argument passed to
 * the reviewed local setup owner at 0x02001804.  The three selected variants
 * converge on one machine call site; the default 0x934 variant has a second
 * call site, which is kept as two lexical C calls below.  All seventeen BL
 * sites resolve independently across thirteen targets.
 *
 * Per-site call symbols (law 6): each callee below is named at the
 * bl-site-symbols-decoded address for its own call site (the raw .s
 * gives every bl its own local trampoline, even for same-overlay siblings
 * and even for the same logical callee reused from more than one site), not
 * the generic main-image/local-owner address the earlier draft used.
 */

s32 Func_02002364(void);      /* site 0x1086, was Func_020012dc */
void Func_02002224();         /* site 0x1092, was Func_02001190 */
void Func_0200590c();         /* site 0x109e, was Func_0200486c */

s32 Func_02007520();          /* site 0x107e, was Func_080770c0(0x300) */
void Func_02007556();         /* site 0x108c, was Func_0808a018 */
void Func_02007642();         /* site 0x1098, was Func_0808a170 */
void Func_020075f8();         /* site 0x10a6, was Func_0808a100 */
u8 *Func_02007596();          /* site 0x10ac, was Func_0808a080 */
void Func_020075d8();         /* site 0x10be, was Func_0808a0b8 */
void Func_02007606();         /* site 0x10c4, was Func_0808a0e8 */
void Func_02007618();         /* site 0x10ce, was Func_0808a0f0 */
void Func_020075a4();         /* site 0x10d2, was Func_0808a020 */

s32 Func_0200757c();          /* site 0x10da, was Func_080770c0(0x92b) */
s32 Func_0200758c();          /* site 0x10ea, was Func_080770c0(0x92a) */
s32 Func_0200759c();          /* site 0x10fa, was Func_080770c0(0x929) */
void Func_0200290e();         /* site 0x1108, was Func_02001804 (converged) */
void Func_0200291a();         /* site 0x1114, was Func_02001804 (default 0x934) */

void Func_02001078(void)
{
    s32 object;
    u8 *actor;

    if (Func_02007520(0x300) != 0) {
        object = Func_02002364();
        Func_02007556();
        Func_02002224(object);
        Func_02007642(0x1ea6);
        Func_0200590c(16);
        Func_020075f8(object, 2);

        actor = Func_02007596(0);
        if (actor != 0) {
            Func_020075d8(object, *(s16 *)(actor + 10),
                          *(s16 *)(actor + 18));
        }

        Func_02007606(object);
        Func_02007618(object, 0, 0);
        Func_020075a4();
    } else {
        if (Func_0200757c(0x92b) != 0) {
            Func_0200290e(16, 0x1e91, 0x998);
        } else if (Func_0200758c(0x92a) != 0) {
            Func_0200290e(16, 0x1e91, 0x91f);
        } else if (Func_0200759c(0x929) != 0) {
            Func_0200290e(16, 0x1e91, 0x93d);
        } else {
            Func_0200291a(16, 0x1e91, 0x934);
        }
    }
}
