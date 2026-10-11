#include "serial_runtime.h"
#include "dma.h"
#include "io_reg.h"
#include "../irqlock.h"
#include "../iowrite.h"

void SerialRuntime_Initialize(void)
{
    u32 interrupt_enable;
    /* FAKEMATCH: keep the fill word's address before its store. */
    volatile u32 zero;
    s32 index;
    struct SerialRuntime *state;

    IRQ_LOCK(interrupt_enable);
    Runtime_SetIrqHandler(7, 0, SerialRuntime_HandleTransferInterrupt);
    Runtime_SetIrqHandler(6, 0, SerialRuntime_HandleTransferInterrupt);
    REG_IME = 0;
    REG_IE &= 0xff3f;
    if ((REG_IF & 0x80) != 0)
        REG_IF = 0x80;
    if ((REG_IF & 0x40) != 0)
        REG_IF = 0x40;
    REG_RCNT = 0x8000;
    REG_RCNT = 0;
    REG_SIOCNT = 0x1000;
    REG_RCNT = 0;
    REG_SIOCNT = 0x2000;
    REG_SIOCNT16 = REG_SIOCNT16 | 0x4003;
    state = &gSerialRuntime;
    REG_IME = 1;
    zero = 0;
    Dma_Set(&zero, state, 0x85000058, REG_DMA3);
    state->send_index = -1;
    state->send_buffer[0] = (u16 *)(state->storage + 0);
    state->send_buffer[1] = (u16 *)(state->storage + 32);
    for (index = 0; index <= 1; index++) {
        state->incoming_buffer[index] = (u16 *)(state->storage + 64 + index * 96);
        state->ready_buffer[index] = (u16 *)(state->storage + 96 + index * 96);
        state->pending_buffer[index] = (u16 *)(state->storage + 128 + index * 96);
    }
    REG_IME = 0;
    REG_IE |= 0x80;
    REG_IME = 1;
    gSerialExchangeActive = 1;
    gLinkExchangeState = 0;
    gSerialSendSource = 0;
    gSerialSendSize = 0;
    gSerialReceiveDest = 0;
    gSerialReceivedSize = 0;
    BattleLink_ResetTransferState();
    IRQ_UNLOCK(interrupt_enable);
}

void SerialRuntime_EnableTransferTimer(void)
{
    struct SerialRuntime *state = &gSerialRuntime;

    if (state->mode != 0)
        state->transfer_enabled = 1;
}

void SerialRuntime_DisableTransfer(void)
{
    struct SerialRuntime *state;

    REG_IME = 0;
    REG_IE = REG_IE & 0xff3f;
    REG_IME = 1;
    REG_SIOCNT16 = 0x2003;
    REG_TM3CNT = 0x0000c963;
    REG_IF = 0xc0;
    state = &gSerialRuntime;
    state->transfer_enabled = 0;
}

s32 SerialRuntime_PollAndTransfer(void *send, void *receive)
{
    volatile u32 *siocnt;
    u32 control;
    s32 status;
    s32 result;

    siocnt = &REG_SIOCNT;
    control = *siocnt;
    switch (gSerialRuntime.phase) {
    case 0: {
        u8 mask30 = control & 0x30;
        u8 mode;

        if (mask30 == 0 && (mode = control & 0x88) == 8) {
            u8 serial_error = control & 4;

            if (serial_error == 0 && gSerialRuntime.send_index == -1) {
                u32 ie;

                /* FAKEMATCH: the do/while blocks and the separate enable variable are meaningless, as in SerialRuntime_PollStatus. */
                do {
                    REG_IME = serial_error;
                } while (0);
                ie = REG_IE;
                do {
                    ie &= ~0x80;
                    ie |= 0x40;
                    REG_IE = ie;
                    {
                        u8 enable = 1;
                        REG_IME = enable;
                    }
                    *((volatile s8 *)siocnt + 1) &= ~0x40;
                    REG_IF = 0xc0;
                    REG_TM3CNT = 0xc963;
                } while (0);
                gSerialRuntime.mode = mode;
            }
        } else if (mask30 == 0) {
            break;
        }
        gSerialRuntime.phase = 1;
    }
    case 1:
        SerialRuntime_CollectReceivedPayloads(receive);
        SerialRuntime_PrepareSendPacket(send);
        break;
    }

    gSerialRuntime.sequence++;
    status = gSerialRuntime.current_mask | (gSerialRuntime.received_mask << 8);
    if (gSerialRuntime.mode == 8)
        status |= 0x80;
    result = SerialRuntime_AddParentFlag(status);
    if (((control << 26) >> 30) > 1)
        result |= 0x2000;
    return result;
}

s32 SerialRuntime_PollStatus(void)
{
    volatile u32 *siocnt;
    u32 control;
    s32 status;
    s32 result;

    siocnt = &REG_SIOCNT;
    control = *siocnt;
    if (gSerialRuntime.phase == 0) {
        u8 mode = control & 0x88;

        if (mode == 8) {
            u8 serial_error = control & 4;

            if (serial_error == 0 && gSerialRuntime.send_index == -1) {
                u32 ie;

                /* FAKEMATCH: one-pass scopes place IE after the IME write and the mask after the IE read; fresh enable reuses r3 for ~0x40. */
                do {
                    REG_IME = serial_error;
                } while (0);
                ie = REG_IE;
                do {
                    ie &= ~0x80;
                    ie |= 0x40;
                    REG_IE = ie;
                    {
                        u8 enable = 1;
                        REG_IME = enable;
                    }
                    *((volatile s8 *)siocnt + 1) &= ~0x40;
                    REG_IF = 0xc0;
                    REG_TM3CNT = 0xc963;
                } while (0);
                gSerialRuntime.mode = mode;
            }
            gSerialRuntime.phase = 1;
        }
        gSerialRuntime.sequence++;
    }

    status = gSerialRuntime.current_mask | (gSerialRuntime.received_mask << 8);
    if (gSerialRuntime.mode == 8)
        status |= 0x80;
    result = SerialRuntime_AddParentFlag(status);
    if (((control << 26) >> 30) > 1)
        result |= 0x2000;
    return result;
}
//--------------------------------------------------------------------------
// sio_xchg.c
// 通信：毎フレームのデータの受け渡しと、送るパケットの組み立て
//--------------------------------------------------------------------------

#include "serial_runtime.h"
#include "dma.h"
#include "io_reg.h"
#include "edition.h"

//--------------------------------------------------------------------------
// phase 1 なら受けたデータを receive に集め、send から次のパケットを作る
// 戻り値は通信の状態
//   ビット 0〜7  : current_mask（mode 8 ならビット 7 も立てる）
//   ビット 8〜15 : received_mask
//   0x1000       : 親
//   0x2000       : SIOCNT のマルチプレイ ID が 2 以上
//--------------------------------------------------------------------------
s32 SerialRuntime_ExchangePayloads(void *send, void *receive)
{
	volatile u32 *siocnt;
	u32			  cnt;
	s32			  st;
	s32			  ret;

	siocnt = &REG_SIOCNT;
	cnt = *siocnt;
	if (gSerialRuntime.phase == 1)
	{
		SerialRuntime_CollectReceivedPayloads(receive);
		SerialRuntime_PrepareSendPacket(send);
		gSerialRuntime.sequence++;
	}
	st = gSerialRuntime.current_mask | (gSerialRuntime.received_mask << 8);
	if (gSerialRuntime.mode == 8)
		st |= 0x80;
	ret = SerialRuntime_AddParentFlag(st);
	if (((cnt << 26) >> 30) > 1) // ビット 4・5
		ret |= 0x2000;
	return ret;
}

//--------------------------------------------------------------------------
// 送るパケットを send_buffer[0] に組み立てる
// [0] 番号、[1] まだ受けていない相手のマスク、[2] チェックサム、[4] データ 24 バイト
// チェックサムは先頭 28 バイトを 16 ビットずつ足して反転したもの
//--------------------------------------------------------------------------
void SerialRuntime_PrepareSendPacket(void *payload)
{
	volatile struct SerialRuntime *sr;
	u32							   i;
	u32							   sum;

	sr = SERIAL_RUNTIME;
	{
		u8 *pkt;
		u8	cur;
		u8	recv;

		pkt = (u8 *)sr->send_buffer[0];
		pkt[0] = sr->sequence;
		cur = sr->current_mask;
		recv = sr->received_mask;
		sum = 0;
		pkt[1] = recv ^ cur;
		*(u16 *)(pkt + 2) = sum;
		Dma_Set(payload, pkt + 4, DMA_ENABLE32 | DMA_32BIT | 6, REG_DMA3);
	}
	{
		u16 *pkt;
		u32	 v;

		pkt = sr->send_buffer[0];
		for (i = 0; i <= 13; i++)
		{
			v = *pkt;
			pkt++;
			sum += v;
		}
	}
	{
		u32					 inv;
		struct SerialPacket *pkt;

		inv = ~sum;
		pkt = (struct SerialPacket *)sr->send_buffer[0];
		pkt->checksum = inv;
	}

	// タイマー 3 を止め、送る位置を戻してから動かしなおす
	if (sr->mode != 0)
		REG_TM3CNT_H = 0;
	sr->send_index = -1;
	if (sr->mode != 0 && sr->transfer_enabled != 0)
		REG_TM3CNT_H = 0xc0; // 開始・割り込み
}
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

    *(u32 *)&flags[4] = 0;
    Io_Write16(0, &REG_IME);
    for (i = 0; i <= 3; i++) {
        swap = gSerialRuntime.pending_buffer[i];
        gSerialRuntime.pending_buffer[i] = gSerialRuntime.ready_buffer[i];
        gSerialRuntime.ready_buffer[i] = swap;
    }
    *(u32 *)flags = *(u32 *)gSerialRuntime.channel_flags;
    *(u32 *)gSerialRuntime.channel_flags = 0;
    Io_Write16(1, &REG_IME);
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
        /* FAKEMATCH: the zero in r0, see above. An ordinary u16 (also
         * register-qualified) reuses busy's zero in r4, removes the literal
         * zero load/pool word and leaves 5 aligned instructions different.
         * An inline u16 zero-return helper differs by 24; an inline byte
         * store with a u16 first parameter leaves the same 5. */
        register u16 zero asm("r0") = 0;
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
        if (gKeysHeld & 1)
            SerialRuntime_BeginTransferA((void *)Cart_Restart, 0x280);
        if (gKeysHeld & 2)
            SerialRuntime_BeginTransferA((void *)(Rom_Start + 0x1000), 0x280);
        if (gKeysHeld & 8) {
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
