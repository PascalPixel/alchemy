#include "TYPES.H"

struct SoundPlayer;

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

void MusicTrack_CalcOutput(struct SoundPlayer *player, struct SoundTrack *track)
{
    u32 volume;
    s32 pan;
    s32 pitch;
    s32 bend;

    if (track->update & 1) {
        volume = (u32)(track->volume * track->volume_scale) >> 5;
        if (track->mod_target == 1)
            volume = ((track->mod_amount + 128) * volume) >> 7;
        pan = track->pan * 2 + track->pan_offset;
        if (track->mod_target == 2)
            pan += track->mod_amount;
        if (pan < -128)
            pan = -128;
        else if (pan > 127)
            pan = 127;
        track->out_volume_a = ((pan + 128) * volume) >> 8;
        track->out_volume_b = ((127 - pan) * volume) >> 8;
    }
    if (track->update & 4) {
        bend = track->bend * track->bend_range;
        pitch = (track->tune + bend) * 4
            + (track->key_offset_a << 8) + (track->key_offset_b << 8)
            + track->fine;
        if (track->mod_target == 0)
            pitch += track->mod_amount << 4;
        track->pitch_hi = pitch >> 8;
        track->pitch_lo = pitch;
    }
    track->update &= ~5;
}
