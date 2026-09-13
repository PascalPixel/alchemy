// Approved reference provenance: adapted from pret/pokeemerald
// src/m4a.c:TrkVolPitSet at commit 83df84e40623b79281f2397faa611cbf044170bd.
// Names and layout were checked against this ROM region and exact compiler output.
#include "audio_engine_symbols.h"
#include "types.h"

struct MusicPlayerState;

struct VolumePitchTrackState {
    u8 flags;
    u8 wait;
    u8 pattern_level;
    u8 repeat_count;
    u8 gate_time;
    u8 key;
    u8 velocity;
    u8 running_status;
    u8 key_mod;
    u8 pitch_mod;
    s8 key_shift;
    s8 key_shift_extra;
    s8 tune;
    u8 pitch_extra;
    s8 bend;
    u8 bend_range;
    u8 right_volume;
    u8 left_volume;
    u8 volume;
    u8 volume_scale;
    s8 pan;
    s8 pan_extra;
    s8 modulation_delta;
    u8 modulation;
    u8 modulation_type;
};

void MusicTrack_UpdateVolumePitch(
    struct MusicPlayerState *unused,
    struct VolumePitchTrackState *track)
{
    if (track->flags & 1) {
        s32 volume;
        s32 pan;

        volume = (u32)(track->volume * track->volume_scale) >> 5;

        if (track->modulation_type == 1)
            volume =
                (u32)(volume * (track->modulation_delta + 128)) >> 7;

        pan = 2 * track->pan + track->pan_extra;

        if (track->modulation_type == 2)
            pan += track->modulation_delta;

        if (pan < -128)
            pan = -128;
        else if (pan > 127)
            pan = 127;

        track->right_volume = (u32)((pan + 128) * volume) >> 8;
        track->left_volume = (u32)((127 - pan) * volume) >> 8;
    }

    if (track->flags & 4) {
        s32 bend = track->bend * track->bend_range;
        s32 pitch = (track->tune + bend)
                  * 4
                  + (track->key_shift << 8)
                  + (track->key_shift_extra << 8)
                  + track->pitch_extra;

        if (track->modulation_type == 0)
            pitch += 16 * track->modulation_delta;

        track->key_mod = pitch >> 8;
        track->pitch_mod = pitch;
    }

    track->flags &= ~(1 | 4);
}
