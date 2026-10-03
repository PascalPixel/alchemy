#include "AUDIO_ENGINE.H"
#include "TYPES.H"

void MusicPlayer_SetPitchAndUpdateFrequency(struct SoundPlayer *player, u16 scale)
{
    if (player->lock == SOUND_LOCK) {
        player->lock++;
        player->tempo_scale = scale;
        player->tempo_step = (player->tempo * player->tempo_scale) >> 8;
        player->lock = SOUND_LOCK;
    }
}

void MusicPlayer_SetVolume(struct SoundPlayer *player, u16 mask, u16 volume)
{
    s32 i;
    struct SoundTrack *track;
    u32 bit;

    if (player->lock != SOUND_LOCK)
        return;
    player->lock++;
    i = player->track_count;
    track = player->tracks;
    bit = 1;
    while (i > 0) {
        if (mask & bit) {
            if (track->flags & 0x80) {
                track->volume_scale = volume >> 2;
                track->flags |= 3;
            }
        }
        i--;
        track++;
        bit <<= 1;
    }
    player->lock = SOUND_LOCK;
}

void MusicPlayer_SetPitch(struct SoundPlayer *player, u16 mask, s16 pitch)
{
    s32 i;
    struct SoundTrack *track;
    u32 bit;

    if (player->lock != SOUND_LOCK)
        return;
    player->lock++;
    i = player->track_count;
    track = player->tracks;
    bit = 1;
    while (i > 0) {
        if (mask & bit) {
            if (track->flags & 0x80) {
                track->key_offset_b = pitch >> 8;
                track->fine = pitch;
                track->flags |= 12;
            }
        }
        i--;
        track++;
        bit <<= 1;
    }
    player->lock = SOUND_LOCK;
}

void MusicPlayer_SetPan(struct SoundPlayer *player, u16 mask, s8 pan)
{
    s32 i;
    struct SoundTrack *track;
    u32 bit;

    if (player->lock != SOUND_LOCK)
        return;
    player->lock++;
    i = player->track_count;
    track = player->tracks;
    bit = 1;
    while (i > 0) {
        if (mask & bit) {
            if (track->flags & 0x80) {
                track->pan_offset = pan;
                track->flags |= 3;
            }
        }
        i--;
        track++;
        bit <<= 1;
    }
    player->lock = SOUND_LOCK;
}

void MusicTrack_ClearModulation(struct SoundTrack *track)
{
    track->lfo_phase = 0;
    track->mod_amount = 0;
    if (track->mod_target == 0)
        track->flags |= 12;
    else
        track->flags |= 3;
}
