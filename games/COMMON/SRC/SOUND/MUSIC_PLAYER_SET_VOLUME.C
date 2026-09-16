#include "TYPES.H"

#define SOUND_LOCK 0x68736D53

struct SoundTrack {
    u8 flags;
    u8 unk01[0x0a];
    s8 key_offset_b;
    u8 unk0c;
    u8 fine;
    u8 unk0e[5];
    u8 volume_scale;
    u8 unk14;
    s8 pan_offset;
    u8 unk16[0x3a];
};

struct SoundPlayer {
    u8 unk00[8];
    u8 track_count;
    u8 unk09[0x23];
    struct SoundTrack *tracks;
    u8 unk30[4];
    u32 lock;
};

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
