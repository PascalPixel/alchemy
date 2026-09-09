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
    u16 value)
{
    if (player->ident == 0x68736d53) {
        player->ident++;
        player->pitch = value;
        player->scaled_pitch = (player->pitch_scale * player->pitch) >> 8;
        player->ident = 0x68736d53;
    }
}
