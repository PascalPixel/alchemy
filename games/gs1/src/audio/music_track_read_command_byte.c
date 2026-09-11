#include "scene.h"
#include "audio_engine.h"
#include "audio_engine_symbols.h"
#include "types.h"

/* audio/misc/read_track_command_byte.c */
void MusicTrack_ReadCommandByte(
    struct MusicPlayerState *unused_player,
    struct MusicTrackState *track)
{
    const u8 *command = track->command;

    track->command = command + 1;
    *(volatile const u8 *)command;
}

/* audio/misc/pcm_key_to_frequency.c */
s32 Math_UmulHigh32(s32, s32);
extern u8 gRom[];
extern u32 gRom2[];

s32 Pcm_KeyToFrequency(void *wave, u8 key, u8 pitch)
{
    u32 key_index;
    u32 pitch_scale;
    u32 lower_frequency;
    u32 upper_frequency;

    key_index = key;
    pitch_scale = pitch << 24;
    if (key_index > 178) {
        key_index = 178;
        pitch_scale = 255 << 24;
    }

    lower_frequency = gRom[key_index];
    lower_frequency =
        gRom2[lower_frequency & 15] >> (lower_frequency >> 4);
    upper_frequency = gRom[key_index + 1];
    upper_frequency =
        gRom2[upper_frequency & 15] >> (upper_frequency >> 4);

    return Math_UmulHigh32(*(s32 *)((u8 *)wave + 4),
        lower_frequency
            + Math_UmulHigh32(
                upper_frequency - lower_frequency,
                pitch_scale));
}

void Audio_NoopStub(void)
{
}
