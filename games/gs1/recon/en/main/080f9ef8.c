#include "audio_engine.h"

void MusicTrack_Stop(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    u8 flags = track->flags;

    if (flags & 0x80) {
        struct CgbChannel *channel = track->channel;

        if (channel != 0) {
            s32 clear = 0;

            do {
                if (channel->status_flags != 0) {
                    u8 type = channel->type & 7;

                    if (type != 0) {
                        struct AudioEngineState *audio =
                            *(struct AudioEngineState **)0x03007ff0;
                        audio->cgb_osc_off.handler(type);
                    }
                    channel->status_flags = clear;
                }
                channel->track = (void *)clear;
                channel = channel->next_channel;
            } while (channel != 0);
        }
        track->channel = channel;
    }
}
