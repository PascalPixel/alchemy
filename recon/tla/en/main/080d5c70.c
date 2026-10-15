/* Near miss, differ 8 of 113 (ported from The Broken Seal's
   Scene_FadeColorFromWhite; ⚓️ plays cue 298 where it plays 288): the
   native loads the event work before it starts the cue's offset (movs r1,
   #128), and the pool offsets of three loads move with that row. A one-pass
   block or barrier after the load fixes it but moves the frame's sub sp,
   #8 up to the load (differ 8 again); permute (3 min) found nothing. */
#include "types.h"
#include "disptran.h"
#include "eventwrk.h"
#include "io_reg.h"
#include "iowrite.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

void WaitFrames(s32 frames);

s32 Func_080d5c70(s32 unused)
{
    struct EventWork *work;
    s32 i;
    s32 c;

    work = Ram_HeapSlots->event_work;
    Audio_PlayCue(gPartyState.scene_cue);
    Audio_PlayCue(298);
    Audio_PlayCue(147);
    if (work->control_mode == 3) {
        Io_Write16(0x7fff, (u16 *)0x050001e6);
        DisplayTransition_Finish(0x401, 16);
        work->screen_status = 0;
        WaitFrames(16);
        for (i = 0; i < 16; i++) {
            c = 30 - i * 2;
            Io_Write16((c << 10) | (c << 5) | c, (u16 *)0x050001e6);
            WaitFrames(1);
        }
    } else {
        Io_Write16(0x7fff, &PLTT_BACKDROP);
        DisplayTransition_Finish(0x207, 16);
        work->screen_status = 0;
        WaitFrames(16);
        for (i = 0; i < 16; i++) {
            c = 30 - i * 2;
            Io_Write16((c << 10) | (c << 5) | c, &PLTT_BACKDROP);
            WaitFrames(1);
        }
    }
}
