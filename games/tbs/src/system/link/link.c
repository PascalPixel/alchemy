#include "types.h"
#include "io_reg.h"
#include "serial_runtime.h"
#include "dma.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "keystate.h"
#include "audio_engine_symbols.h"
#include "cartboot.h"

u8 SerialRuntime_CollectReceivedPayloads(void *payload)
{
    u8 flags[8];
    s32 i;
    u32 k;
    u16 *packet;
    u16 *swap;
    s32 sum;
    s32 enable;

    *(u32 *)&flags[4] = 0;
    /* FAKEMATCH: keeps the frame clear ahead of the interrupt disable; without it the IME address load rises above "sub sp" (score 6). */
    __asm__("");
    REG_IME = 0;
    for (i = 0; i <= 3; i++) {
        swap = gSerialRuntime.pending_buffer[i];
        gSerialRuntime.pending_buffer[i] = gSerialRuntime.ready_buffer[i];
        gSerialRuntime.ready_buffer[i] = swap;
    }
    *(u32 *)flags = *(u32 *)gSerialRuntime.channel_flags;
    /* FAKEMATCH: an unsigned long store does not alias the u32 snapshot, so sched2 puts lr = sp before the snapshot store as the native does (2 without). */
    *(unsigned long *)gSerialRuntime.channel_flags = 0;
    /* FAKEMATCH: a separate enable value set before the barrier loads the 1 ahead of the IME address and lets -1 be derived from it (26 with a literal 1). */
    enable = 1;
    /* FAKEMATCH: holds the IME address load after the 1 (score 6 without). */
    __asm__("");
    REG_IME = enable;
    /* FAKEMATCH: keeps the IME store ahead of the mask clear and the loop setup (score 22 without). */
    __asm__("");
    gSerialRuntime.current_mask = 0;
    for (i = 0; i <= 1; i++) {
        packet = gSerialRuntime.pending_buffer[i];
        sum = 0;
        for (k = 0; k <= 13; k++)
            sum += packet[k];
        if (flags[i] == 1 && (s16)sum == -1) {
            Dma_Set(gSerialRuntime.pending_buffer[i] + 2, (u8 *)payload + i * 24, 0x84000006, REG_DMA3);
            gSerialRuntime.current_mask |= 1 << i;
        }
        if ((s16)sum == -1)
            gSerialRuntime.pending_buffer[i][1] = ~gSerialRuntime.pending_buffer[i][1];
    }
    gSerialRuntime.received_mask |= gSerialRuntime.current_mask;
    return gSerialRuntime.current_mask;
}

void SerialRuntime_HandleTransferInterrupt(void)
{
    struct SerialRuntime *send_state;
    struct SerialRuntime *receive_state;
    struct SerialRuntime *tail_state;
    u16 recv[4];
    volatile u32 *sio_control;
    s32 channel;

    *(u64 *)recv = *(volatile u64 *)REG_SIODATA32;
    sio_control = &REG_SIOCNT;
    send_state = &gSerialRuntime;
    send_state->is_parent = (*sio_control << 25) >> 31;

    if (send_state->send_index == -1) {
        u32 idle_word = 0xfefe;
        u16 *swap;

        ((volatile u16 *)sio_control)[1] = idle_word;
        swap = send_state->send_buffer[1];
        send_state->send_buffer[1] = send_state->send_buffer[0];
        send_state->send_buffer[0] = swap;
    } else if (send_state->send_index >= 0) {
        ((volatile u16 *)sio_control)[1] =
            send_state->send_buffer[1][send_state->send_index];
    }
    receive_state = &gSerialRuntime;
    if (receive_state->send_index <= 14)
        receive_state->send_index++;

    channel = 0;
receive_loop:
    {
        s32 receive_index;
        u16 *incoming;

        if (recv[channel] == 0xfefe &&
            receive_state->receive_index[channel] > 13) {
            receive_state->receive_index[channel] = -1;
        } else {
            receive_index = receive_state->receive_index[channel];
            incoming = receive_state->incoming_buffer[channel];
            incoming[receive_index] =
                recv[channel];
            if (receive_index == 13) {
                u16 *swap = receive_state->ready_buffer[channel];

                receive_state->ready_buffer[channel] = incoming;
                receive_state->incoming_buffer[channel] = swap;
                receive_state->channel_flags[channel] |= 1;
            }
        }
    }
    tail_state = &gSerialRuntime;
    if (tail_state->is_parent != 0)
        tail_state->channel_flags[channel] |= 2;
    if (tail_state->receive_index[channel] <= 14)
        tail_state->receive_index[channel]++;
    channel++;
    receive_state = tail_state;
    if (channel <= 1) goto receive_loop;

    if (tail_state->mode == 8) {
        REG_TM3CNT_H = 0;
        REG_SIOCNT16 = REG_SIOCNT16 | 0x80;
        REG_TM3CNT_H = 0xc0;
    }
}

void SerialRuntime_RemoveIrqHandlers(void)
{
    gSerialExchangeActive = 0;
    Runtime_SetIrqHandler(7, 0, 0);
    Runtime_SetIrqHandler(6, 0, 0);
}

extern volatile u16 gLinkStatus;

u32 SerialRuntime_WaitForStatusMask(s32 mask)
{
    if ((mask & gLinkStatus) != mask) {
        do {
            WaitFrames(1);
        } while ((mask & gLinkStatus) != mask);
    }
    return (REG_SIOCNT << 0x1A) >> 0x1E;
}

s32 SerialRuntime_BeginTransferA(void *source, s32 transfer_value)
{
    volatile s32 *active;
    struct SerialTransferState *state;
    volatile u16 *ime;
    u32 saved_interrupt_master;
    s32 busy;
    s32 value;
    s32 transfer;

    /* FAKEMATCH: sharing the integer address carrier with the result keeps the source in r0 through the busy check. A separate result is hoisted into r0 and moves the source to another register. */
    value = (s32)source;
    active = &gSerialSendSource;
    busy = *active;
    transfer = transfer_value;
    /* FAKEMATCH: assigning active twice selects r5 for active and r6 for transfer; one-pass blocks preserve store order. */
    do {
        state = &gSerialTransfer;
    } while (0);
    active = &gSerialSendSource;
    if (busy == 0)
        goto begin_transfer;
    value = -1;
    goto transfer_complete;

begin_transfer:
    ime = &REG_IME;
    saved_interrupt_master = *ime;
    *ime = (u32)ime;
    do {
        state->status = 0x80;
        gSerialSendSize = transfer;
        gSerialBlockSequence = 0;
        *active = value;
        state->active = 1;
    } while (0);
    *ime = saved_interrupt_master;
    value = 0;

transfer_complete:
    return value;
}

s32 SerialRuntime_BeginTransferB(void *destination)
{
    volatile s32 *active;
    struct SerialTransferState *state;
    volatile u16 *ime;
    u32 saved_interrupt_master;
    s32 busy;
    s32 value;

    /* FAKEMATCH: sharing the integer address carrier with the result keeps the destination in r0 through the busy check. A separate result is hoisted into r0 and makes the function save another register. */
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
    /* FAKEMATCH: the one-pass block keeps the IME read and write ahead of the transfer stores, as in SerialRuntime_BeginTransferA. */
    do {
        state->status = 0x81;
        gSerialReceivedSize = 0;
        state->active = 1;
        *active = value;
    } while (0);
    {

        volatile u8 *sequence = &gSerialBlockSequence;
        register u16 zero asm("r0") = 0; /* FAKEMATCH: the zero in r0, see above */
        *sequence = zero;
    }
    *ime = saved_interrupt_master;
    value = 0;

transfer_complete:
    return value;
}

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

extern u8 gMapCellBuffer[];
extern const u8 Rom_Start[];

void AudioCommand_PlayFar(s32 cue);

void RuntimeWait_BusyLoopTick(void)
{
}

void SerialTest_Run(void)
{
    u16 *tile;
    s32 value;
    s32 n;
    s32 tick;
    volatile u32 zero;

    AudioCommand_PlayFar(3);
    SerialRuntime_Initialize();
    n = 19;
    tile = (u16 *)BG_SCREEN_BLOCK(4) + 16 * 32 + 19;
    value = (s16)0xf093;
    do {
        n--;
        *tile = value;
        tile--;
        value--;
    } while (n >= 0);
    zero = 0;
    Bios_CpuSet((const void *)&zero, (void *)gMapCellBuffer, 0x05000100);
    SerialRuntime_WaitForStatusMask(3);
restart:
    SerialRuntime_BeginTransferB(gMapCellBuffer);
    for (;;) {
        if ((*(volatile u32 *)&gKeysHeld) & 1)
            SerialRuntime_BeginTransferA((void *)Cart_Restart, 0x280);
        if ((*(volatile u32 *)&gKeysHeld) & 2)
            SerialRuntime_BeginTransferA((void *)(Rom_Start + 0x1000), 0x280);
        if ((*(volatile u32 *)&gKeysHeld) & 8) {
            tick = 9999;
            do {
                tick--;
                RuntimeWait_BusyLoopTick();
            } while (tick >= 0);
        }
        if (gSerialReceiveDest == 0) {
            Dma_Set((const void *)gMapCellBuffer, (void *)0x06001000, 0x840000a0, REG_DMA3);
            goto restart;
        }
        WaitFrames(1);
    }
}
