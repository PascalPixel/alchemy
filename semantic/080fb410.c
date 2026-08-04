#include "types.h"

struct Channel_080fb410 {
    u8 flags;
    u8 unknown_01[21];
    u8 pending_22;
    u8 unknown_17;
    u8 mode_24;
    u8 unknown_19;
    u8 pending_26;
};

/*
 * Arm a channel for its next update.  The two pending counters are cleared and
 * a request is posted into the flag byte: mode 0 asks for the long form
 * (bits 2 and 3), any other mode for the short one (bits 0 and 1).
 * Called from 0x080fb47e and 0x080fb4f2.
 */
void Func_080fb410(struct Channel_080fb410 *channel)
{
    s32 request;

    channel->pending_26 = 0;
    channel->pending_22 = 0;

    if (channel->mode_24 == 0)
        request = 12;
    else
        request = 3;

    channel->flags = (u8) (request | channel->flags);
}
