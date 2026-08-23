/*
 * VENEER AUDIT NOTE (2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
 *
 * This file is byte-exact, so the indirect-call shape below is intentionally
 * retained.  The call reaches command-table slot 34 through __call_via_r1;
 * spelling it as a normal function-pointer call may make the compiler choose
 * a different veneer register.
 */
#include "audio_engine_symbols.h"
#include "types.h"

u32 Func_080072e8(u32, s32);

void AudioCommand_InvokeSlot34(u32 context)
{
    s32 handler;
    int slot;

    slot = 0x02004088;
    slot = *((s32 *)slot);
    handler = slot;
    Func_080072e8(context, handler);
}
