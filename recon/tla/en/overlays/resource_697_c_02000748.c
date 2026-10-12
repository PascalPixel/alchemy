/* Draft, differ 2 of 12: the native loads the party base before it builds the render-mode offset; this form builds the offset first. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

extern const u8 Data_02005a7c[];

static __inline__ void Shadow_SetHeight(s32 id, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200070c(id, value);
}

void Func_02000748(s32 a, s32 id)
{
    if (gPartyState.render_mode == 0)
        Func_0200062c(id);
}
