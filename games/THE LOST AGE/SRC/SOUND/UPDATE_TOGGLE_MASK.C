#include "AUDIO_ENGINE.H"

/* Sets the audio command mask, or toggles the given bits when bit 7 of the
   command is set. */
void AudioCommand_UpdateToggleMask(u32 command)
{
    u32 toggle = command & AUDIO_COMMAND_TOGGLE;

    command &= AUDIO_COMMAND_MASK;
    if (toggle != 0)
        Audio_CommandMask ^= command;
    else
        Audio_CommandMask = command;
}
