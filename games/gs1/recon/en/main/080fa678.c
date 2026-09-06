#include "audio_engine_symbols.h"
#include "types.h"

/* Correct indirect-call model; the canonical compiler selects a different veneer. */

typedef u32 (*AudioCommandFn)(u32 context);

void AudioCommand_InvokeSlot34(u32 context)
{
    AudioCommandFn handler = *(AudioCommandFn *)0x02004088;

    handler(context);
}
