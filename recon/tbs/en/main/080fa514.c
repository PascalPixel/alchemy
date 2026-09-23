#include "AUDIO_ENGINE.H"

#define MusicPlayer_ResetActiveTracks Func_080fa514

void AudioCommand_InvokeSlot35(void *block);

static __inline__ s32 masked_track_status(s32 mask, s32 status)
{
    return mask & status;
}

void MusicPlayer_ResetActiveTracks(struct SoundPlayer *player)
{
    s32 count = player->track_count;
    struct SoundTrack *track = player->tracks;

    if (count > 0) {
        s32 active_mask = 0x80;

        do {
            register s32 playing_mask;
            s32 status = track->flags;

            if (masked_track_status(active_mask, status) != 0) {
                playing_mask = 0x40;

                if (masked_track_status(playing_mask, status) != 0) {
                    AudioCommand_InvokeSlot35(track);
                    track->flags = active_mask;
                    track->bend_range = 2;
                    track->volume_scale = playing_mask;
                    track->lfo_speed = 22;
                    track->voice.kind = 1;
                }
            }
            count--;
            track++;
        } while (count > 0);
    }
}
