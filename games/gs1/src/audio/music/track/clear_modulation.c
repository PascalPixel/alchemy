#include "types.h"

struct MusicTrackChannelState {
    u8 flags;
    u8 unknown01[21];
    u8 pending22;
    u8 unknown17;
    u8 mode24;
    u8 unknown19;
    u8 pending26;
};

void MusicTrack_ClearModulation(struct MusicTrackChannelState *channel)
{
    channel->pending26 = 0;
    channel->pending22 = 0;
    if (channel->mode24 == 0)
        channel->flags |= 12;
    else
        channel->flags |= 3;
}
