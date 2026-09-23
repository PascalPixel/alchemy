#include "SERIAL_RUNTIME.H"

#define SerialRuntime_BeginTransferA Func_080063bc

s32 SerialRuntime_BeginTransferA(s32 value, s32 transfer_value)
{
    volatile s32 *active;
    struct SerialTransferState *state;
    u32 saved_interrupt_master;
    s32 busy;
    s32 transfer;

    active = &SERIAL_ACTIVE_A;
    busy = *active;
    transfer = transfer_value;
    /* FAKEMATCH: both do/while blocks and the second active assignment are
     * meaningless. Setting active twice raises its allocation priority above
     * transfer, so active gets r5 and transfer r6; the blocks keep the stores
     * in the reference order. */
    do {
        state = SERIAL_TRANSFER;
    } while (0);
    active = &SERIAL_ACTIVE_A;
    if (busy == 0)
        goto begin_transfer;
    value = -1;
    goto transfer_complete;

begin_transfer:
    saved_interrupt_master = Data_04000208;
    Data_04000208 = (u16)&Data_04000208;
    do {
        state->status = 0x80;
        SERIAL_VALUE_A = transfer;
        SERIAL_RESULT = 0;
        *active = value;
        state->active = 1;
    } while (0);
    Data_04000208 = saved_interrupt_master;
    value = 0;

transfer_complete:
    return value;
}
