#include "TYPES.H"

extern u8 Audio_CommandMask;

/* Sets the audio command mask, or toggles the given bits when bit 7 of the
   command is set. */
void AudioCommand_UpdateToggleMask(u32 command)
{
    u32 toggle = command & 0x80;

    command &= 0x7f;
    if (toggle != 0)
        Audio_CommandMask ^= command;
    else
        Audio_CommandMask = command;
}
