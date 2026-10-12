/* Draft, differ 2 of 12: the native loads the party base before it builds the render-mode offset; this form builds the offset first. A party pointer local changes nothing; an asm barrier after the base fixes that order but moves the id copy to the top (differ 4). Tried 2026-10-12: a one-pass block around the base, and the id copied to a local after the barrier: the copy still leads (differ 4). */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

extern const u8 Data_02005a7c[];

static __inline__ void Shadow_SetHeight(s32 id, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200070c(id, value);
}

void Func_02000728(s32 a, s32 id)
{
    if (gPartyState.render_mode == 0)
        Func_02000660(id);
}
