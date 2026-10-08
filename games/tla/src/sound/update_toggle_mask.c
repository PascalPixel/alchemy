#include "audio_engine.h"

void AudioCommand_UpdateToggleMask(u32 command)
{
    u32 toggle = command & AUDIO_COMMAND_TOGGLE;

    command &= AUDIO_COMMAND_MASK;
    if (toggle != 0)
        Audio_CommandMask ^= command;
    else
        Audio_CommandMask = command;
}
