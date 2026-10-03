#include "AUDIO_ENGINE.H"

/* These table entries use the library's one-word work-block transport. */

void AudioCommand_InvokeSlot35(s32 argument)
{
    ((void (*)(s32))Sound_CommandTable[35])(argument);
}
