/*
 * VENEER AUDIT NOTE (2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
 *
 * This file is byte-exact, so the indirect-call shape below is intentionally
 * retained.  The call reaches command-table slot 35 through __call_via_r1;
 * spelling it as a normal function-pointer call may make the compiler choose
 * a different veneer register.
 */
#include "audio_engine_symbols.h"
#include "types.h"

u32 Func_080072e8(u32, s32);

void AudioCommand_InvokeSlot35(u32 state_block)
{
    int slot;
    s32 handler;

    slot = 0x0200408C;
    slot = *((s32 *)slot);
    handler = slot;
    Func_080072e8(state_block, handler);
}
