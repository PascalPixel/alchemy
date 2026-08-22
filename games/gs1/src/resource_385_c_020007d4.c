#include "types.h"

/* Complete actor-13 temporary-acceptance dialogue wrapper through its pool. */
struct Actor_020007d4 {
    u8 reserved00[91];
    u8 accepted;
};

/*
 * Every call is named at its decoded bl site: the two Func_0808a080 reads go
 * through two DIFFERENT per-site import veneers, and the dialogue and
 * Func_020004b4 calls also resolve to their own sites, so the semantic
 * single-name imports cannot reproduce the reference displacements.
 */
extern void Func_02001942(s32 dialogue);                 /* Func_0808a170 */
extern struct Actor_020007d4 *Func_020018e0(s32 actor);  /* Func_0808a080 */
extern void Func_02000ca0(s32 actor);                    /* Func_020004b4 */
extern struct Actor_020007d4 *Func_020018f2(s32 actor);  /* Func_0808a080 */

void Func_020007d4(void)
{
    Func_02001942(0x1cbf);
    Func_020018e0(13)->accepted = 1;
    Func_02000ca0(13);
    Func_020018f2(13)->accepted = 0;
}
