#include "AUDIO_ENGINE_SYMBOLS.H"
#include "TYPES.H"

/* Correct indirect-call model; the canonical compiler selects a different veneer. */

typedef u32 (*AudioCommandFn)(u32 context);

void AudioCommand_InvokeSlot35(u32 state_block)
{
    AudioCommandFn handler = *(AudioCommandFn *)0x0200408C;

    handler(state_block);
}
