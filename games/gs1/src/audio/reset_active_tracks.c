#include "audio_engine_symbols.h"
#include "types.h"

void AudioCommand_InvokeSlot35(u8 *track);

static __inline__ s32 masked_track_status(s32 mask, s32 status)
{
    return mask & status;
}

void MusicPlayer_ResetActiveTracks(u8 *player)
{
    s32 count = player[8];
    u8 *track = *(u8 **)(player + 44);

    if (count > 0) {
        s32 active_mask = 0x80;

        do {
            register s32 playing_mask;
            s32 status = track[0];

            if (masked_track_status(active_mask, status) != 0) {
                playing_mask = 0x40;

                if (masked_track_status(playing_mask, status) != 0) {
                    AudioCommand_InvokeSlot35(track);
                    track[0] = active_mask;
                    track[15] = 2;
                    track[19] = playing_mask;
                    track[25] = 22;
                    track[36] = 1;
                }
            }
            count--;
            track += 80;
        } while (count > 0);
    }
}
