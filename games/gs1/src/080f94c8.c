#include "types.h"

void MusicPlayer_SetPitchAndUpdateFrequency(s32 address, u16 value);

void Func_080f94c8(u16 value)
{
    MusicPlayer_SetPitchAndUpdateFrequency(0x02004290, value);
}
