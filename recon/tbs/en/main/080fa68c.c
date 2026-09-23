#include "AUDIO_ENGINE_SYMBOLS.H"
#include "TYPES.H"

typedef u32 (*AudioCommandFn)(u32 context);
u32 Func_080072e8(u32 context, AudioCommandFn handler);
#define Runtime_CallViaR1 Func_080072e8

void AudioCommand_InvokeSlot35(u32 state_block)
{
    Runtime_CallViaR1(state_block, *(AudioCommandFn *)0x0200408C);
}
