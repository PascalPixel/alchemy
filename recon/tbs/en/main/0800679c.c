/* Not-yet-C: the complete SerialTest_Run extent is 200 bytes, including its
 * literal pool. Diagnostic cartridge reads are not cartridge restart calls.
 * 2026-10-03: Rom_Start and Rom_Start + 0x1000 give 200/200 bytes but score
 * 320 (1 operand, 1 inserted, 2 deleted). The first window uses a pool load
 * instead of mov/lsl, also changing argument order and the pool. No address
 * exception or compiler steering: the routine remains uncredited. */
#include "DMA.H"
#include "SERIAL_RUNTIME.H"
#include "KEYSTATE.H"
#include "AUDIO_ENGINE_SYMBOLS.H"
extern u8 gMapCellBuffer[];
extern const u8 Rom_Start[];

void Audio_PlayCue(s32 cue);
void SerialRuntime_Initialize(void);
void RuntimeWait_BusyLoopTick(void);

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
    Bios_CpuSet((const void *)&zero, (void *)gMapCellBuffer, 0x05000100);
    SerialRuntime_WaitForStatusMask(3);
restart:
    SerialRuntime_BeginTransferB(gMapCellBuffer);
    for (;;) {
        /* VBlank refreshes the held keys between these button samples. */
        if ((*(volatile u32 *)&gKeysHeld) & 1)
            SerialRuntime_BeginTransferA((void *)Rom_Start, 0x280);
        if ((*(volatile u32 *)&gKeysHeld) & 2)
            SerialRuntime_BeginTransferA((void *)(Rom_Start + 0x1000), 0x280);
        if ((*(volatile u32 *)&gKeysHeld) & 8) {
            tick = 9999;
            do {
                tick--;
                RuntimeWait_BusyLoopTick();
            } while (tick >= 0);
        }
        if (SERIAL_ACTIVE_B == 0) {
            Dma_Set((const void *)gMapCellBuffer, (void *)0x06001000, 0x840000a0, (volatile u32 *)0x040000d4);
            goto restart;
        }
        WaitFrames(1);
    }
}
