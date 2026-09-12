#include "types.h"

void MusicPlayer_SetPitchAndUpdateFrequency(s32 address, u16 value);

void MusicCommand_SetPitchAndUpdateFrequency(u16 value)
{
    MusicPlayer_SetPitchAndUpdateFrequency(0x02004290, value);
}
