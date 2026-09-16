#include "TYPES.H"

struct SoundTrack {
    u8 update;
    u8 unk01[7];
    u8 pitch_hi;
    u8 pitch_lo;
    s8 key_offset_a;
    s8 key_offset_b;
    s8 tune;
    u8 fine;
    s8 bend;
    u8 bend_range;
    u8 out_volume_a;
    u8 out_volume_b;
    u8 volume;
    u8 volume_scale;
    s8 pan;
    s8 pan_offset;
    s8 mod_amount;
    u8 unk17;
    u8 mod_target;
    u8 unk19[0x37];
};

struct SoundPlayer {
    u8 unk00[4];
    u32 status;
    u8 track_count;
    u8 unk09[0x1b];
    u16 fade_period;
    u16 fade_counter;
    u16 fade_volume;
    u8 unk2a[2];
    struct SoundTrack *tracks;
};

void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);

void MusicPlayer_StepFade(struct SoundPlayer *player)
{
    s32 count;
    struct SoundTrack *track;

    if (player->fade_period == 0)
        return;
    if (--player->fade_counter != 0)
        return;
    player->fade_counter = player->fade_period;

    if (player->fade_volume & 2) {
        if ((player->fade_volume += 16) >= 256) {
            player->fade_volume = 256;
            player->fade_period = 0;
        }
    } else {
        if ((s16)(player->fade_volume -= 16) <= 0) {
            count = player->track_count;
            track = player->tracks;
            while (count > 0) {
                MusicTrack_Stop(player, track);
                if (!(player->fade_volume & 1))
                    track->update = 0;
                count--;
                track++;
            }
            if (player->fade_volume & 1)
                player->status |= 0x80000000;
            else
                player->status = 0x80000000;
            player->fade_period = 0;
            return;
        }
    }

    count = player->track_count;
    track = player->tracks;
    while (count > 0) {
        if (track->update & 0x80) {
            track->volume_scale = player->fade_volume >> 2;
            track->update |= 3;
        }
        count--;
        track++;
    }
}
