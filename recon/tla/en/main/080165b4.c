/* Draft: SerialRuntime_CollectReceivedPayloads, 2026-10-12: The Broken Seal's
 * source (system/link/link.c) as it stands.
 * differ 4 of 106: the store of the last flag byte (strb r0, [r3, #3]) and the
 * load of the serial state's address (ldr r2, =) swap places. */
#include "serial_runtime.h"
#include "dma.h"
#include "io_reg.h"
#include "iowrite.h"

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
        swap = gSerialRuntime.pendingBuffer[i];
        gSerialRuntime.pendingBuffer[i] = gSerialRuntime.readyBuffer[i];
        gSerialRuntime.readyBuffer[i] = swap;
    }
    *(u32 *)flags = *(u32 *)gSerialRuntime.channelFlags;
    *(u32 *)gSerialRuntime.channelFlags = 0;
    Io_Write16(1, &REG_IME);
    gSerialRuntime.currentMask = 0;
    for (i = 0; i <= 1; i++) {
        packet = gSerialRuntime.pendingBuffer[i];
        sum = 0;
        for (k = 0; k <= 13; k++)
            sum += packet[k];
        if (flags[i] == 1 && (s16)sum == -1) {
            Dma_Set(gSerialRuntime.pendingBuffer[i] + 2, (u8 *)payload + i * 24, 0x84000006, REG_DMA3);
            gSerialRuntime.currentMask |= 1 << i;
        }
        if ((s16)sum == -1)
            gSerialRuntime.pendingBuffer[i][1] = ~gSerialRuntime.pendingBuffer[i][1];
    }
    gSerialRuntime.receivedMask |= gSerialRuntime.currentMask;
    return gSerialRuntime.currentMask;
}
