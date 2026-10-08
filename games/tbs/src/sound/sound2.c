#include "audio_engine.h"

extern void (*Sound_Command34)(struct SoundNote *);

void AudioCommand_InvokeSlot34(struct SoundNote *note)
{
    /* FAKEMATCH: the entry-cell load and one-word call avoid an extra instruction from table indexing. */
    Sound_Command34(note);
}

extern void (*Sound_Command35)(void *);

void AudioCommand_InvokeSlot35(void *block)
{
    /* FAKEMATCH: the entry-cell load and one-word call avoid an extra instruction from table indexing. */
    Sound_Command35(block);
}
