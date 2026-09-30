#include "AUDIO_ENGINE.H"
#include "TYPES.H"

#define SOUND_LOCK 0x68736D53

/* audio/music/track/clear_modulation.c */
struct MusicTrackChannelState {
    u8 flags;
    u8 unknown01[21];
    u8 pending22;
    u8 unknown17;
    u8 mode24;
    u8 unknown19;
    u8 pending26;
};

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

void MusicTrack_ClearModulation(struct MusicTrackChannelState *channel)
{
    channel->pending26 = 0;
    channel->pending22 = 0;
    if (channel->mode24 == 0)
        channel->flags |= 12;
    else
        channel->flags |= 3;
}
