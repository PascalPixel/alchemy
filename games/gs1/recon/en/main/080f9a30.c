#include "audio_engine.h"

void Func_080f9a30(struct CgbChannel *channel)
{
    struct MusicTrackState *track = channel->track;

    if (track != 0) {
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
        channel->track = 0;
    }
}
