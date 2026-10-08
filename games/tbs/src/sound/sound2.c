#include "audio_engine.h"

extern void (*Data_02004088)(struct SoundNote *);

void AudioCommand_InvokeSlot34(struct SoundNote *note)
{
    /* FAKEMATCH: the entry-cell load and one-word call avoid an extra instruction from table indexing. */
    Data_02004088(note);
}

extern void (*Data_0200408c)(void *);

void AudioCommand_InvokeSlot35(void *block)
{
    /* FAKEMATCH: the entry-cell load and one-word call avoid an extra instruction from table indexing. */
    Data_0200408c(block);
}
