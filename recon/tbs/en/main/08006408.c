#include "serial_runtime_family.h"

/* SerialRuntime_BeginTransferB: NONMATCHING, 11 of 40 halfwords differ.
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
    s32 result;

    active = &SERIAL_ACTIVE_B;
    do {
        current = *active;
    } while (0);
    active = &SERIAL_ACTIVE_B;
    state = SERIAL_TRANSFER;
    if (current != 0) {
        result = -1;
    } else {
        saved_interrupt_master = Data_04000208;
        Data_04000208 = (u16)&Data_04000208;
        do {
            state->status = 0x81;
            SERIAL_VALUE_B = current;
            state->active = 1;
            *active = value;
            SERIAL_RESULT = 0;
        } while (0);
        Data_04000208 = saved_interrupt_master;
        result = 0;
    }
    return result;
}
