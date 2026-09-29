#include "SERIAL_RUNTIME.H"


s32 SerialRuntime_BeginTransferA(s32 value, s32 transfer_value)
{
    volatile s32 *active;
    struct SerialTransferState *state;
    volatile u16 *ime;
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
        state = &gSerialTransfer;
    } while (0);
    active = &SERIAL_ACTIVE_A;
    if (busy == 0)
        goto begin_transfer;
    value = -1;
    goto transfer_complete;

begin_transfer:
    ime = &REG_IME;
    saved_interrupt_master = *ime;
    *ime = (u16)(u32)ime; /* its own address, 0x208: bit 0 clear */
    do {
        state->status = 0x80;
        SERIAL_VALUE_A = transfer;
        gSerialBlockSequence = 0;
        *active = value;
        state->active = 1;
    } while (0);
    *ime = saved_interrupt_master;
    value = 0;

transfer_complete:
    return value;
}
