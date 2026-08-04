#include "types.h"

/*
 * Complete actor-21 mode-four reset wrapper.
 *
 * Call symbol resolved directly against the raw region in
 * assets/code/resource_385_overlay.s (`push {lr} / movs r0,#21 / movs r1,#0 /
 * movs r2,#4 / bl sub_02001f26 / pop {r0} / bx r0`): the per-site overlay
 * veneer is sub_02001f26, not a main-image address guessed from elsewhere.
 */
void Func_02001f26(s32 actor, s32 mode, s32 value);

void Func_02000d74(void)
{
    Func_02001f26(21, 0, 4);
}
