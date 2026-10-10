#include "audio_engine.h"

void AudioCommand_InvokeSlot34(struct SoundNote *note)
{
    void (*func)(struct SoundNote *) = (void (*)(struct SoundNote *))*(&Sound_CommandTable[34]);

    func(note);
}

void AudioCommand_InvokeSlot35(void *block)
{
    void (*func)(void *) = (void (*)(void *))*(&Sound_CommandTable[35]);

    func(block);
}
