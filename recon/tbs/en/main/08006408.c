#include "serial_runtime_family.h"

/* SerialRuntime_BeginTransferB: NONMATCHING, 15 of 40 halfwords differ.
 * Complete 80-byte draft (2026-09-26). Reusing value for the return and
 * separating the destination store closes the busy-path/register mismatch.
 * A word-width registered zero prevents reuse of current but leaves its
 * pool slot last and loads it before the result-cell pointer. A plain or
 * block-local byte zero folds to current (72 bytes); narrowing the link
 * zero forces an early pool (82 bytes). Explicit begin/complete labels do
 * not change that lowering. Retained the complete word-zero version.
 * The busy path's -1 is if-converted above the branch into r0, so value is
 * copied to r7 (push {r5, r6, r7}) and stored from there; the reference keeps
 * value in r0, stores it before loading SERIAL_RESULT's zero from the pool and
 * sets -1 only after the branch. Returning through value itself (as
 * SerialRuntime_BeginTransferA does) stops the if-conversion but lets the
 * shared HImode zero take r0 first. */
s32 Func_08006408(s32 value)
{
    volatile s32 *active;
    struct SerialTransferState *state;
    u32 saved_interrupt_master;
    s32 current;

    active = &SERIAL_ACTIVE_B;
    do {
        current = *active;
    } while (0);
    active = &SERIAL_ACTIVE_B;
    state = SERIAL_TRANSFER;
    if (current == 0)
        goto begin_transfer;
    value = -1;
    goto transfer_complete;

begin_transfer:
    {
        saved_interrupt_master = Data_04000208;
        Data_04000208 = (u16)&Data_04000208;
        do {
            state->status = 0x81;
            SERIAL_VALUE_B = current;
            state->active = 1;
            /* FAKEMATCH: keep the destination store before the byte clear. */
            do {
                *active = value;
            } while (0);
            /* FAKEMATCH: keep the result clear as a distinct linked zero. */
            do {
                value = (u32)Data_00000000;
                SERIAL_RESULT = value;
            } while (0);
        } while (0);
        Data_04000208 = saved_interrupt_master;
        value = 0;
    }
transfer_complete:
    return value;
}
