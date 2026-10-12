/* Draft, differ 2 of 21: the native loads the fifth argument from the stack
 * before it copies the first (ldr r7, [sp, #36]; adds r4, r0, #0); this
 * copies first. In the sched2 dump the two are ready together with equal
 * priority and equal dependents, and the copy comes first in the insn
 * stream. Both are parameter set-up, ahead of any statement, so no barrier
 * fits. UiText_DrawNumberAtOffset and UiText_DrawNumberInWindow are the
 * same; their drafts need this one's declarations to compile. */
#include "text_render_runtime.h"

u8 *UiText_FormatNumber(u8 *output, s32 value, s32 width);

void UiText_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    UiText_DrawString(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}
