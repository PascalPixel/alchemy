#include "types.h"

/* Complete actor-11 temporary-acceptance dialogue wrapper through its pool. */
struct Actor_02000754 {
    u8 reserved00[91];
    u8 accepted;
};

/*
 * Every call is named at its decoded bl site: the two Func_0808a080 reads go
 * through two DIFFERENT per-site import veneers, and the dialogue and
 * Func_020004b4 calls also resolve to their own sites, so the semantic
 * single-name imports cannot reproduce the reference displacements.
 */
extern void Func_020018c2(s32 dialogue);                 /* Func_0808a170 */
extern struct Actor_02000754 *Func_02001860(s32 actor);  /* Func_0808a080 */
extern void Func_02000c20(s32 actor);                    /* Func_020004b4 */
extern struct Actor_02000754 *Func_02001872(s32 actor);  /* Func_0808a080 */

void Func_02000754(void)
{
    Func_020018c2(0x1cbd);
    Func_02001860(11)->accepted = 1;
    Func_02000c20(11);
    Func_02001872(11)->accepted = 0;
}
