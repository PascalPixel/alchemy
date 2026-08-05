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
 */

s32 Func_020012dc(void);
void Func_02001190();
void Func_0200486c();
void Func_02001804();

s32 Func_080770c0();
void Func_0808a018();
void Func_0808a020();
void Func_0808a170();
void Func_0808a100();
u8 *Func_0808a080();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_0808a0f0();

void Func_02001078(void)
{
    s32 object;
    u8 *actor;
    s32 variant;

    if (Func_080770c0(0x300) != 0) {
        object = Func_020012dc();
        Func_0808a018();
        Func_02001190(object);
        Func_0808a170(0x1ea6);
        Func_0200486c(16);
        Func_0808a100(object, 2);

        actor = Func_0808a080(0);
        if (actor != 0) {
            Func_0808a0b8(object, *(s16 *)(actor + 10),
                          *(s16 *)(actor + 18));
        }

        Func_0808a0e8(object);
        Func_0808a0f0(object, 0, 0);
        Func_0808a020();
    } else {
        if (Func_080770c0(0x92b) != 0) {
            variant = 0x998;
        } else if (Func_080770c0(0x92a) != 0) {
            variant = 0x91f;
        } else if (Func_080770c0(0x929) != 0) {
        } else {
            variant = 0x93d;
            Func_02001804(16, 0x1e91, 0x934);
            return;
        }

        Func_02001804(16, 0x1e91, variant);
    }
}
