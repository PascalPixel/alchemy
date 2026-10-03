#include "AUDIO_ENGINE.H"

/* The separately labelled word for Sound_CommandTable's slot 34. */
extern void (*Data_02004088)(s32);

void AudioCommand_InvokeSlot34(s32 argument)
{
    /* FAKEMATCH: retain the existing entry-cell word load and one-word
       callback transport. Indexing the actual table adds an instruction
       before the load at the same complete 20-byte extent. */
    Data_02004088(argument);
}
