#include "types.h"

/*
 * Read the low three bits of the runtime word at 0x03001e40 and, on two of the
 * eight values, forward the incoming argument. The 40-byte owner includes its
 * alignment halfword and one pool word. The argument is never materialised in
 * the body, yet it is live at both call sites, so the parameter must stay. The
 * meaning of the selector is not established; the other values fall through
 * deliberately.
 */

extern s32 Data_03001e40;

/* Two call words that reach the same target. */
extern void Func_02006912();
extern void Func_0200691e();

void SceneState_ForwardByRuntimeSelector(s32 arg)
{
    s32 sel = Data_03001e40 & 7;

    if (sel == 0) {
        Func_02006912(arg, 2);
    } else if (sel == 2) {
        Func_0200691e(arg, 0);
    }
}
