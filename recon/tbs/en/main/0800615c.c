/* 2026-10-04 sun100 main-7: score 855 (from 1660), 228/228 bytes with pool.
 * Direct gSerialRuntime fields (no runtime pointer) reproduce the native
 * sym+4 channel-flags constant, the state copy and the r9 reloads in both
 * branches; the indexed swap loop gives the native sym+64 walk. Remaining:
 * global alloc gives -1 lr, the state copy r8 and &snapshot sl where the
 * ROM has &snapshot in lr, -1 in r8 and the copy in r10, because our
 * &snapshot is set at the first word store (ADDRESSOF) instead of after
 * the swap loop; struct, u64 and initializer snapshots did not move it.
 * Earlier history: 1660 with a runtime pointer and an inline IME setter.
 */
#include "SERIAL_RUNTIME.H"
#include "DMA.H"

u8 SerialRuntime_CollectReceivedPayloads(void *payload)
{
    u32 flags[2];
    s32 i;
    u32 k;
    u16 *packet;
    u16 *swap;
    s32 sum;
    u8 *dst = payload;
    u8 *channel;

    flags[1] = 0;
    REG_IME = 0;
    for (i = 0; i <= 3; i++) {
        swap = gSerialRuntime.pending_buffer[i];
        gSerialRuntime.pending_buffer[i] = gSerialRuntime.ready_buffer[i];
        gSerialRuntime.ready_buffer[i] = swap;
    }
    flags[0] = *(u32 *)gSerialRuntime.channel_flags;
    channel = (u8 *)flags;
    *(u32 *)gSerialRuntime.channel_flags = 0;
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
            sum += *packet++;
        if (channel[i] == 1 && (s16)sum == -1) {
            Dma_Set(gSerialRuntime.pending_buffer[i] + 2, dst, 0x84000006, (volatile u32 *)DMA3);
            gSerialRuntime.current_mask |= 1 << i;
        }
        if ((s16)sum == -1)
            gSerialRuntime.pending_buffer[i][1] = ~gSerialRuntime.pending_buffer[i][1];
        dst += 24;
    }
    gSerialRuntime.received_mask |= gSerialRuntime.current_mask;
    return gSerialRuntime.current_mask;
}
