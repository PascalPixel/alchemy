/* Pin-free SerialRuntime_BeginTransferB trial: 27 native instructions,
 * 26 built, 19 common, differ 15 including pool/branch offsets. The compiler
 * reuses busy's zero in r4 for the sequence byte; native loads a zero literal
 * into r0. Assigning value = 0 while initializing zero and making busy a
 * pointer each retain differ 15. Reusing busy for value before the store
 * differs 26 (29 built). A generic output barrier on zero also differs 26;
 * adding the sequence pointer as input or output differs 37 (28 built).
 * A busy output, alone or with value, differs 26. The linked implementation
 * retains its binding. */
#include "serial_runtime.h"
#include "io_reg.h"

s32 SerialRuntime_BeginTransferB(void *destination)
{
    volatile s32 *active;
    struct SerialTransferState *state;
    volatile u16 *ime;
    u32 saved_interrupt_master;
    s32 busy;
    s32 value;

    /* FAKEMATCH: the address carrier also holds the return value. */
    value = (s32)destination;
    state = &gSerialTransfer;
    active = &gSerialReceiveDest;
    busy = *active;
    if (busy == 0)
        goto begin_transfer;
    value = -1;
    goto transfer_complete;

begin_transfer:
    ime = &REG_IME;
    saved_interrupt_master = *ime;
    *ime = (u32)ime;
    /* FAKEMATCH: keep the IME read/write ahead of the transfer stores. */
    do {
        state->status = 0x81;
        gSerialReceivedSize = 0;
        state->active = 1;
        *active = value;
    } while (0);
    {
        volatile u8 *sequence = &gSerialBlockSequence;
        u16 zero = 0;

        *sequence = zero;
    }
    *ime = saved_interrupt_master;
    value = 0;

transfer_complete:
    return value;
}
