#include "audio_engine.h"

void MusicPlayer_SetModulationDepth(struct SoundPlayer *player, s32 mask_arg, s32 value_arg)
{
    u16 mask = mask_arg;
    u8 value = value_arg;
    s32 count;
    struct SoundTrack *track;
    u32 bit;

    if (player->lock != SOUND_LOCK)
        return;

    player->lock++;
    count = player->track_count;
    track = player->tracks;
    bit = 1;
    while (count > 0) {
        if (mask & bit) {
            if (track->flags & SOUND_TRACK_ACTIVE) {
                track->mod_depth = value;
                if (!track->mod_depth)
                    MusicTrack_ClearModulation(track);
            }
        }
        count--;
        track++;
        bit <<= 1;
    }
    player->lock = SOUND_LOCK;
}

void MusicPlayer_SetLfoSpeed(struct SoundPlayer *player, u32 selected, u32 value)
{
    u16 mask = selected;
    u8 speed = value;
    s32 count;
    struct SoundTrack *track;
    u32 bit;

    if (player->lock != SOUND_LOCK)
        return;

    player->lock++;
    count = player->track_count;
    track = player->tracks;
    bit = 1;
    while (count > 0) {
        if (mask & bit) {
            if (track->flags & SOUND_TRACK_ACTIVE) {
                track->lfo_speed = speed;
                if (!track->lfo_speed)
                    MusicTrack_ClearModulation(track);
            }
        }
        count--;
        track++;
        bit <<= 1;
    }
    player->lock = SOUND_LOCK;
}
