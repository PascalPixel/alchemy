#include "types.h"
#include "scene.h"
#include "abi/audio/command/update_toggle_mask.h"

extern u8 gOv[];

void AudioCommand_UpdateToggleMask(u32 command)
{
    u32 toggle = command & 0x80;

    command &= 0x7f;
    if (toggle != 0)
        gOv[0] ^= command;
    else
        gOv[0] = command;
}
