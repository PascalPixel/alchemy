#include "types.h"
#include "serial_runtime.h"
#include "dma.h"

void SerialRuntime_WaitForTransferA(void)
{
    u32 count = 0;

    if (gSerialSendSource != 0) {
        do {
            WaitFrames(1);
            count++;
        } while (count < 600000 && gSerialSendSource != 0);
    }
}

void SerialRuntime_WaitForTransferB(void)
{
    u32 count = 0;

    if (gSerialReceiveDest != 0) {
        do {
            WaitFrames(1);
            count++;
        } while (count < 600000 && gSerialReceiveDest != 0);
    }
}

void SerialRuntime_WaitForTransfers(void)
{
    u32 count;

    count = 0;
    while (gSerialSendSource != 0 || gSerialReceiveDest != 0) {
        WaitFrames(1);
        count++;
        if (count >= 600000)
            return;
    }
}

s32 SerialRuntime_GetActiveTransfers(void)
{
    s32 flags;

    flags = 0;
    if (gSerialSendSource != 0) {
        flags = 1;
    }
    if (gSerialReceiveDest != 0) {
        flags |= 2;
    }
    return flags;
}

void BattleLink_ResetTransferState(void)
{
    SerialRuntime_ResetTransferState();
}

extern volatile u16 gLinkStatus;

struct OneCell { u16 v; };

void SerialRuntime_StepBlockTransfer(void)
{
    struct SerialTransferState *peer;
    u32 id;
    struct SerialTransferState *own;
    u32 gap;
    /* FAKEMATCH: the halfword record retains the measured pooled-one construction; it is not protocol state. */
    struct OneCell one;

    id = ~((REG_SIOCNT << 26) >> 30) & 1;
    one.v = 1;
    own = &gSerialTransfer;
    peer = (struct SerialTransferState *)gSerialPeerPayloads[id];
    if ((gLinkStatus & 3) != 3)
        return;
    {
        s32 *receive = &gSerialReceiveDest;
        s32 dest = *receive;
        if (dest != 0) {
            if (own->flags == 1 && (u8)(peer->peer_flags - 1) <= 1) {
                if (peer->active == (gSerialBlockSequence & 0x7f)) {
                    own->active = 0;
                    switch (peer->peer_flags) {
                    case 1:
                        Dma_Set(peer->payload, (void *)dest, 0x84000005, REG_DMA3);
                        *receive += 20;
                        gSerialReceivedSize += 20;
                        own->status = (own->status + 1) | 0x80;
                        break;
                    case 2:
                        Dma_Set(peer->payload, (void *)dest, 0x84000005, REG_DMA3);
                        gSerialReceivedSize += 20;
                        own->flags = 2;
                        own->status = 0;
                        own->active = one.v;
                        break;
                    }
                    gSerialBlockSequence = (gSerialBlockSequence + 1) & 0x7f;
                } else if (gSerialBlockSequence & 0x80) {
                    if (own->active & 0x80) {
                        own->active = one.v;
                    } else if (own->active == 1) {
                        own->active = 0;
                        gSerialBlockSequence &= 0x7f;
                    }
                } else {
                    own->active = gSerialBlockSequence | 0x80;
                    gSerialBlockSequence |= 0x80;
                }
            } else {
                own->active = 0;
            }
        }
    }
    {
        s32 *send = &gSerialSendSource;
        s32 src = *send;
        if (src != 0) {
            if (peer->flags == 1) {
                if (peer->active & 0x80) {
                    gap = (gSerialBlockSequence - peer->active) & 0x7f;
                    *send = src - gap * 20;
                    gSerialSendSize += gap * 20;
                    gSerialBlockSequence -= gap;
                    gSerialBlockSequence &= 0x7f;
                }
                if (gSerialSendSize != 0) {
                    Dma_Set((void *)*send, own->payload, 0x84000005, REG_DMA3);
                    gSerialSendSize += -20;
                    if (gSerialSendSize != 0) {
                        own->peer_flags = 1;
                        own->active = gSerialBlockSequence & 0x7f;
                        *send += 20;
                    } else {
                        own->peer_flags = 2;
                        own->active = gSerialBlockSequence & 0x7f;
                        *send += 20;
                    }
                    gSerialBlockSequence = (gSerialBlockSequence + 1) & 0x7f;
                }
            }
            if (own->peer_flags == 2 && peer->flags == 2) {
                gSerialSendSource = 0;
                own->peer_flags = 0;
                own->active = 1;
            }
        }
    }
    if (own->flags == 2) {
        if (peer->peer_flags != 2) {
            gSerialReceiveDest = 0;
            own->flags = 0;
        }
    } else {
        own->flags = 0;
        if (gSerialReceiveDest != 0)
            own->flags = 1;
    }
}
