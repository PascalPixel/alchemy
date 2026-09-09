#include "types.h"

/*
 * Mode task setup for resource_3bb.  It seeds a second per-instance task's
 * state and picks which of five addresses that task dispatches through.  The
 * 144-byte owner includes its 15-word literal pool.
 */

extern u16 Data_0200cc30;
extern u16 Data_0200cc04;
extern u16 Data_0200cc3c;
extern s32 Data_0200cc40;
extern u16 Data_0200cc98;
extern u16 Data_0200cc0c;
extern s32 Data_0200cc10;

extern s32 Data_0200ca1e;   /* Default handler. */
extern s32 Data_0200c0ce;   /* Handler for mode 2. */
extern s32 Data_0200ca4a;   /* Handler for mode 4. */
extern s32 Data_0200c0f6;   /* Handler for mode 3 with param set. */
extern s32 Data_0200cac8;   /* Handler for mode 3 with param zero. */

void Func_02006700(s32 taskAddress, s32 frameBudget);

/*
 * 0x0200a451 is the installed callback, a routine in this overlay.  The branch
 * chain picks one of five addresses by mode, consulting param only when mode is
 * 3; whether those five are code or per-mode records is not resolved, so they
 * are typed as plain addresses.  The four stores that follow reset the rest of
 * the task's state block, three as halfwords and one as a word.
 */
void Func_02002940(u32 mode, u32 param)
{
    s32 handler;

    Data_0200cc30 = (u16)mode;
    Data_0200cc04 = (u16)(param << 4);

    {
        s32 budget = 0xc80;
        s32 task = 0x0200a451;
        Func_02006700(task, budget);
    }

    handler = (s32)&Data_0200ca1e;
    if (mode == 2) {
        handler = (s32)&Data_0200c0ce;
    }
    if (mode == 4) {
        handler = (s32)&Data_0200ca4a;
    }
    if (mode == 3) {
        if (param != 0) {
            handler = (s32)&Data_0200c0f6;
        } else {
            handler = (s32)&Data_0200cac8;
        }
    }

    Data_0200cc3c = 0;
    Data_0200cc40 = handler;
    Data_0200cc98 = 0;
    Data_0200cc0c = 0;
    Data_0200cc10 = 0;
}
