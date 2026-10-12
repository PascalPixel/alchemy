#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "serial_runtime.h"
#include "irqlock.h"

void SerialRuntime_RemoveIrqHandlers(void)
{
    gSerialExchangeActive = 0;
    Runtime_SetIrqHandler(7, 0, 0);
    Runtime_SetIrqHandler(6, 0, 0);
}


u32 SerialRuntime_WaitForStatusMask(s32 mask)
{
    if ((mask & gLinkStatus) != mask) {
        do {
            WaitFrames(1);
        } while ((mask & gLinkStatus) != mask);
    }
    return (REG_SIOCNT << 0x1A) >> 0x1E;
}

s32 SerialRuntime_BeginTransferA(void *source, s32 size)
{
    struct SerialTransferState *state = &gSerialTransfer;
    u32 saved;

    if (gSerialSendSource != 0)
        return -1;
    IRQ_LOCK(saved);
    state->status = 0x80;
    gSerialSendSource = (s32)source;
    gSerialSendSize = size;
    gSerialBlockSequence = 0;
    state->active = 1;
    IRQ_UNLOCK(saved);
    return 0;
}

s32 SerialRuntime_BeginTransferB(void *destination)
{
    struct SerialTransferState *state = &gSerialTransfer;
    u32 saved;

    if (gSerialReceiveDest != 0)
        return -1;
    IRQ_LOCK(saved);
    state->status = 0x81;
    gSerialReceiveDest = (s32)destination;
    gSerialReceivedSize = 0;
    state->active = 1;
    gSerialBlockSequence = 0;
    IRQ_UNLOCK(saved);
    return 0;
}
