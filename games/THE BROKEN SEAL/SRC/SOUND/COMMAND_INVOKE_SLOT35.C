#include "AUDIO_ENGINE.H"

/* The separately labelled word for Sound_CommandTable's slot 35. */
extern void (*Data_0200408c)(s32);

void AudioCommand_InvokeSlot35(s32 argument)
{
    /* FAKEMATCH: retain the existing entry-cell word load and one-word
       callback transport. Indexing the actual table adds an instruction
       before the load at the same complete 20-byte extent. */
    Data_0200408c(argument);
}
