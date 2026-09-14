#include "AUDIO_ENGINE_SYMBOLS.H"
#include "TYPES.H"

/* Correct indirect-call model; the canonical compiler selects a different veneer. */

typedef u32 (*AudioCommandFn)(u32 context);

void AudioCommand_InvokeSlot34(u32 context)
{
    AudioCommandFn handler = *(AudioCommandFn *)0x02004088;

    handler(context);
}
