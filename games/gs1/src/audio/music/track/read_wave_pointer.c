#include "types.h"

struct MusicPlayerState;

struct MusicTrackState {
    u8 unknown_00[0x28];
    u32 wave_pointer;
    u8 unknown_2c[0x14];
    u8 *command;
};

union Value_080fb6a4 {
    u32 word;
    struct {
        u32 byte0 : 8;
        u32 byte1 : 8;
        u32 byte2 : 8;
        u32 byte3 : 8;
    } bits;
};

void MusicTrack_ReadWavePointer(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    union Value_080fb6a4 value;
    u8 *source = track->command;

    value.bits.byte0 = source[0];
    value.bits.byte1 = source[1];
    value.bits.byte2 = source[2];
    value.bits.byte3 = source[3];
    track->wave_pointer = value.word;
    track->command = source + 4;
}
