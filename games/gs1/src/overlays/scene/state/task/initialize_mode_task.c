#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/task/initialize_mode_task.h"

/*
 * Mode task setup for resource_3bb.  It seeds a second per-instance task's
 * state and picks which of five addresses that task dispatches through.  The
 * 144-byte owner includes its 15-word literal pool.
 */

extern u16 gOv;
extern u16 gOv2;
extern u16 gOv3;
extern s32 gOv4;
extern u16 gOv5;
extern u16 gOv6;
extern s32 gOv7;

extern s32 gOv8;   /* Default handler. */
extern s32 gOv9;   /* Handler for mode 2. */
extern s32 gOv10;   /* Handler for mode 4. */
extern s32 gOv11;   /* Handler for mode 3 with param set. */
extern s32 gOv12;   /* Handler for mode 3 with param zero. */

/*
 * 0x0200a451 is the installed callback, a routine in this overlay.  The branch
 * chain picks one of five addresses by mode, consulting param only when mode is
 * 3; whether those five are code or per-mode records is not resolved, so they
 * are typed as plain addresses.  The four stores that follow reset the rest of
 * the task's state block, three as halfwords and one as a word.
 */
void State_Run(u32 mode, u32 param)
{
    s32 handler;

    gOv = (u16)mode;
    gOv2 = (u16)(param << 4);

    {
        s32 budget = 0xc80;
        s32 task = 0x0200a451;
        State_Apply(task, budget);
    }

    handler = (s32)&gOv8;
    if (mode == 2) {
        handler = (s32)&gOv9;
    }
    if (mode == 4) {
        handler = (s32)&gOv10;
    }
    if (mode == 3) {
        if (param != 0) {
            handler = (s32)&gOv11;
        } else {
            handler = (s32)&gOv12;
        }
    }

    gOv3 = 0;
    gOv4 = handler;
    gOv5 = 0;
    gOv6 = 0;
    gOv7 = 0;
}
