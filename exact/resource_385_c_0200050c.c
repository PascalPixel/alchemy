#include "types.h"

/*
 * Complete actor-11 dialogue wrapper through its one-word pool.
 *
 * Call symbols resolved directly against the raw region in
 * assets/code/resource_385_overlay.s (lines 753-765, still unadopted at the
 * time of writing): `bl sub_0200167a`, `bl sub_02001674`, then
 * `bl .L_020009d6` -- the third call targets a local branch label at
 * 0x020009d6, not the guessed 0x020004b4.
 */
void Func_0200167a(s32 dialogue);
void Func_02001674(s32 actor, s32 mode, s32 value);
void Func_020009d6(s32 actor);

void Func_0200050c(void)
{
    Func_0200167a(0x1cae);
    Func_02001674(11, 0, 2);
    Func_020009d6(11);
}
