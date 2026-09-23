#include "AUDIO_ENGINE_SYMBOLS.H"
#include "TYPES.H"

typedef u32 (*AudioCommandFn)(u32 context);
u32 Func_080072e8(u32 context, AudioCommandFn handler);
#define Runtime_CallViaR1 Func_080072e8

void AudioCommand_InvokeSlot34(u32 context)
{
    Runtime_CallViaR1(context, *(AudioCommandFn *)0x02004088);
}
