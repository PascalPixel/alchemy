#include "types.h"

/*
 * Cancel the track's two pitch-modulation accumulators and ask for a refresh.
 *
 * The running modulation amount at track+0x16 and its phase counter at
 * track+0x1a are both cleared, and then the request bits raised in the track
 * status byte depend on which modulation the track is configured for: the
 * selector at track+0x18 being zero asks for bits 0x0c, any other value for
 * bits 0x03.  The two arms differ only in that mask and share the
 * read-modify-write of the status byte.
 *
 * Called directly by the depth handler at 0x080fa0ce and by the two operand
 * handlers at 0x080fa1d4 and 0x080fa1e8, which invoke it when their new
 * operand is zero.
 */

struct Track_080fa1ac {
    u8 status;
    u8 unknown_01[21];
    u8 modulation_amount;
    u8 unknown_17;
    u8 modulation_selector;
    u8 unknown_19;
    u8 modulation_phase;
};

void Func_080fa1ac(void *player, struct Track_080fa1ac *track)
{
    u8 request;

    track->modulation_amount = 0;
    track->modulation_phase = 0;

    if (track->modulation_selector == 0)
        request = 12;
    else
        request = 3;

    track->status |= request;
}
