#include "types.h"

/*
 * Sound-sequence flow control: return from a subroutine.
 *
 * The counterpart of the 0x080f9ae0 call handler.  The nesting depth at
 * track+0x02 is decremented and the stream position saved at that slot of the
 * three-deep return stack at track+0x44 is reinstated as track+0x40.
 *
 * At depth zero there is nothing to return to and the handler does nothing at
 * all — not even advancing the stream — so a sequence that returns without a
 * matching call re-executes this command forever rather than running off the
 * end of the track.  That is the behaviour of the image and is reproduced as
 * written.
 */

struct Track_080f9afc {
    u8 status;
    u8 unknown_01;
    u8 nesting_depth;
    u8 unknown_03[61];
    const u8 *command;
    const u8 *return_stack[3];
};

void Func_080f9afc(void *player, struct Track_080f9afc *track)
{
    u8 depth = track->nesting_depth;

    if (depth == 0)
        return;

    depth--;
    track->nesting_depth = depth;
    track->command = track->return_stack[depth];
}
