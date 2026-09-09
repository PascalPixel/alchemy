#include "types.h"

/*
 * Story-flag hand-off in overlay resource_3b9: clear one flag, set its
 * immediate neighbour.  It is installed into a script record rather than
 * called, and each Func_ name spells a relocated call word.
 */

extern void Func_02003d7e_a();
extern void Func_02003d7e_b();

/*
 * The two flag ids are adjacent but spelled differently, and the spellings
 * are load-bearing.  565 is 0x235 and comes from the owner's single pool
 * word; 564 is built as movs #141 / lsls #2, so it must stay a plain decimal
 * value rather than another pool constant.  The 24-byte owner covers that
 * pool word and the alignment halfword after it.  What the pair gates is not
 * established.
 */
void SceneState_ApplyFlags565And564(void)
{
    Func_02003d7e_a(0x235);
    Func_02003d7e_b(564);
}
