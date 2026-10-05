#include "AUDIO_ENGINE.H"

/* The separately labelled word for Sound_CommandTable's slot 34. */
extern void (*Data_02004088)(struct SoundNote *);

void AudioCommand_InvokeSlot34(struct SoundNote *note)
{
    /* FAKEMATCH: the entry-cell load and one-word call avoid an extra instruction from table indexing. */
    Data_02004088(note);
}
