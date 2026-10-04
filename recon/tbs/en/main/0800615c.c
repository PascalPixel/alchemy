/* 2026-10-04 sun finisher 8: score 14 as written (from 32), 4 with two
 * tagged empty asm barriers (after the flags[4] store and after the
 * channel-flag clear); not adopted.
 * Found: the snapshot is a u8[8] (BLKmode, so it lives in the frame from the
 * start); as a u32[2], struct or u64 it sat in a pseudo until ADDRESSOF put
 * &snapshot in a register at the first store, GCSE carried that register
 * from the top and the allocator gave lr/r8/r10 the wrong pseudos. The
 * packet sum is indexed (packet[k]) and the payload is payload + i * 24, so
 * both are loop givs incremented in the native order; the channel-flag
 * word is volatile (the receive interrupt writes it), which keeps its clear
 * before the IME store.
 * Remaining (with the barriers): after the flag word is read the ROM sets
 * lr = sp before the snapshot store and the 1 for IME before its address
 * load; sched2 here puts the store and the address load first. Without the
 * barriers the IME-disable block also schedules its address load above
 * "sub sp", where the ROM keeps the original order. Tried: a channel pointer
 * before/after the store, a u32 temporary read before the clear, the enable
 * value set earlier, volatile snapshot stores (address forced into a reg),
 * every subset of up to three barriers over eight statement gaps. */
#include "SERIAL_RUNTIME.H"
#include "DMA.H"

u8 SerialRuntime_CollectReceivedPayloads(void *payload)
{
    u8 flags[8];
    s32 i;
    u32 k;
    u16 *packet;
    u16 *swap;
    s32 sum;
    u8 *channel;

    *(u32 *)&flags[4] = 0;
    REG_IME = 0;
    for (i = 0; i <= 3; i++) {
        swap = gSerialRuntime.pending_buffer[i];
        gSerialRuntime.pending_buffer[i] = gSerialRuntime.ready_buffer[i];
        gSerialRuntime.ready_buffer[i] = swap;
    }
    channel = flags;
    *(u32 *)flags = *(volatile u32 *)gSerialRuntime.channel_flags;
    *(volatile u32 *)gSerialRuntime.channel_flags = 0;
    {
        /* FAKEMATCH: a separate enable value lets -1 be derived from the 1 still in a register (1520 without). */
        s32 enable = 1;
        REG_IME = enable;
    }
    gSerialRuntime.current_mask = 0;
    for (i = 0; i <= 1; i++) {
        packet = gSerialRuntime.pending_buffer[i];
        sum = 0;
        for (k = 0; k <= 13; k++)
            sum += packet[k];
        if (channel[i] == 1 && (s16)sum == -1) {
            Dma_Set(gSerialRuntime.pending_buffer[i] + 2, (u8 *)payload + i * 24, 0x84000006, (volatile u32 *)DMA3);
            gSerialRuntime.current_mask |= 1 << i;
        }
        if ((s16)sum == -1)
            gSerialRuntime.pending_buffer[i][1] = ~gSerialRuntime.pending_buffer[i][1];
    }
    gSerialRuntime.received_mask |= gSerialRuntime.current_mask;
    return gSerialRuntime.current_mask;
}
