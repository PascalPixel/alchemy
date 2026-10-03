#include "AUDIO_ENGINE.H"

void MusicPlayer_SetModulationDepth(struct SoundPlayer *player, s32 mask_arg, s32 value_arg)
{
    s32 mask = (u16)mask_arg;
    s32 value = (u8)value_arg;
    s32 count;
    struct SoundTrack *track;
    u32 bit;

    if (player->lock != SOUND_LOCK)
        return;

    player->lock += 1;
    count = player->track_count;
    track = player->tracks;
    bit = 1;
    if (count > 0) {
        do {
            if ((mask & bit) && (track->flags & SOUND_TRACK_ACTIVE)) {
                track->mod_depth = value;
                if (value == 0)
                    MusicTrack_ClearModulation(track);
            }
            count--;
            track++;
            bit <<= 1;
        } while (count > 0);
    }
    player->lock = SOUND_LOCK;
}

void MusicPlayer_SetLfoSpeed(struct SoundPlayer *player, u32 selected, u32 value)
{
    u16 selected_bits = selected;
    u8 stored_value = value;

    if (player->lock == SOUND_LOCK) {
        s32 count;
        struct SoundTrack *track;
        u32 mask;

        player->lock = player->lock + 1;
        count = player->track_count;
        track = player->tracks;
        mask = 1;

        if (count > 0) {
            do {
                if ((selected_bits & mask) != 0 && (track->flags & SOUND_TRACK_ACTIVE) != 0) {
                    track->lfo_speed = stored_value;
                    if (stored_value == 0) {
                        MusicTrack_ClearModulation(track);
                    }
                }
                count--;
                track++;
                mask <<= 1;
            } while (count > 0);
        }

        player->lock = SOUND_LOCK;
    }
}
