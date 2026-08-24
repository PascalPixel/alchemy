#include "types.h"

extern u8 Data_02003040[];

void AudioCommand_UpdateToggleMask(u32 command)
{
    u32 toggle = command & 0x80;

    command &= 0x7f;
    if (toggle != 0)
        Data_02003040[0] ^= command;
    else
        Data_02003040[0] = command;
}
