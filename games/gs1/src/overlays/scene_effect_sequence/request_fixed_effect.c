/* Named shorthand for one fixed effect request, in overlay resource_3a0. */

#include "scene_effect_sequence.h"

void Func_020017be();

/*
 * The 16-byte owner at 0x02000314 loads no literal, so it carries no pool
 * word and no alignment halfword.  All three arguments are immediates, and
 * Func_020017be spells the overlay's own relocated call word rather than a
 * runtime address.
 */
void SceneEffect_RequestFixedEffect(void)
{
    Func_020017be(22, 1, 2);
}
