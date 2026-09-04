#include "audio_engine.h"

#define CgbChannel_Unlink Func_080f9a30
void CgbChannel_Unlink(struct CgbChannel *channel)
{
    struct MusicTrackState *track = channel->track;

    if (track == 0) {
        return;
    }

    {
        struct CgbChannel *next = channel->next_channel;
        struct CgbChannel *previous = channel->previous_channel;

        if (previous != 0) {
            previous->next_channel = next;
        } else {
            track->channel = next;
        }
        if (next != 0) {
            next->previous_channel = previous;
        }

        /* Preserve the compiler's r1 null carrier after its final list use. */
        next = 0;
        channel->track = (struct MusicTrackState *)next;
    }
}
