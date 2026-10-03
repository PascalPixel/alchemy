/*
 * SerialRuntime_BeginTransferA: start a byte-counted send from its source.
 * Reference and complete candidate object: 72 bytes, literal pool included.
 * Immutable refresh after physical-name closure: score 120, two reordered
 * instructions. The transfer argument move follows the busy-cursor read;
 * the source-cursor store follows the size/sequence stores.
 * Refreshed against the rebuilt English ELF: every physical name resolves;
 * no unresolved-symbol comparisons remain.
 * Existing body and its measured FAKEMATCH devices are unchanged.
 */
#include "TYPES.H"
#include "SERIAL_RUNTIME.H"


s32 SerialRuntime_BeginTransferA(void *source, s32 transfer_value)
{
    volatile s32 *active;
    struct SerialTransferState *state;
    volatile u16 *ime;
    u32 saved_interrupt_master;
    s32 busy;
    s32 value;
    s32 transfer;

    /* FAKEMATCH: sharing the integer address carrier with the result keeps
       the source in r0 through the busy check. A separate result is hoisted
       into r0 and moves the source to another register. */
    value = (s32)source;
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
