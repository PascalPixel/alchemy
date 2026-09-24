#include "DMA.H"
#include "SERIAL_RUNTIME.H"
#include "AUDIO_ENGINE_SYMBOLS.H"

void Audio_PlayCue(s32 cue);
void SerialRuntime_Initialize(void);
void SerialRuntime_WaitForStatusMask(s32 mask);
void SerialRuntime_BeginTransferA(const void *source, s32 size);
void SerialRuntime_BeginTransferB(void *destination);

#define KEYS_HELD (*(volatile u32 *)0x03001ae8)

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

    Audio_PlayCue(3);
    SerialRuntime_Initialize();
    n = 19;
    tile = (u16 *)0x06002426;
    value = (s16)0xf093; /* palette 15, tile 0x93, counting down */
    do {
        n--;
        *tile = value;
        tile--;
        value--;
    } while (n >= 0);
    zero = 0;
    Bios_CpuSet((const void *)&zero, (void *)0x02010000, 0x05000100);
    SerialRuntime_WaitForStatusMask(3);
restart:
    SerialRuntime_BeginTransferB((void *)0x02010000);
    for (;;) {
        if (KEYS_HELD & 1)
            SerialRuntime_BeginTransferA((const void *)0x08000000, 0x280);
        if (KEYS_HELD & 2)
            SerialRuntime_BeginTransferA((const void *)0x08001000, 0x280);
        if (KEYS_HELD & 8) {
            tick = 9999;
            do {
                tick--;
                RuntimeWait_BusyLoopTick();
            } while (tick >= 0);
        }
        if (SERIAL_ACTIVE_B == 0) {
            Dma_Set((const void *)0x02010000, (void *)0x06001000, 0x840000a0, (volatile u32 *)0x040000d4);
            goto restart;
        }
        WaitFrames(1);
    }
}
