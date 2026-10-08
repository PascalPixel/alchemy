#include "serial_runtime.h"

s32 SerialRuntime_BeginTransferB(void *destination)
{
    volatile struct SerialTransferState *state = &gSerialTransfer;
    u32 saved;

    if (gSerialReceiveDest != 0)
        return -1;

    saved = REG_IME;
    REG_IME = 0;
    state->status = 0x81;
    gSerialReceiveDest = (s32)destination;
    gSerialReceivedSize = 0;
    state->active = 1;
    gSerialBlockSequence = 0;
    REG_IME = saved;
    return 0;
}
