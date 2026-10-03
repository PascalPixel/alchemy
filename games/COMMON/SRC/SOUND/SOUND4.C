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
        /* FAKEMATCH: retain the original separate zero-test value; using
           the stored value shrinks this routine from 116 to 108 bytes and
           replaces the native high-register saves with a stack spill. */
        s32 check = value;

        do {
            if ((mask & bit) && (track->flags & SOUND_TRACK_ACTIVE)) {
                track->mod_depth = value;
                if (check == 0)
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
            /* FAKEMATCH: retain the original byte zero-test copy; using
               stored_value shrinks this routine from 116 to 108 bytes and
               replaces the native high-register saves with a stack spill. */
            u8 test_value = stored_value;

            do {
                if ((selected_bits & mask) != 0 && (track->flags & SOUND_TRACK_ACTIVE) != 0) {
                    track->lfo_speed = stored_value;
                    if (test_value == 0) {
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
