#include "types.h"

struct MusicPlayerPitchState {
    u8 unknown_00[0x1c];
    u16 pitch_scale;
    u16 pitch;
    u16 scaled_pitch;
    u8 unknown_22[0x12];
    u32 ident;
};

void MusicPlayer_SetPitchAndUpdateFrequency(
    struct MusicPlayerPitchState *player,
    s32 value)
{
    u16 pitch = value;

    if (player->ident == 0x68736d53) {
        player->pitch = pitch;
        player->scaled_pitch = (pitch * player->pitch_scale) >> 8;
    }
}
