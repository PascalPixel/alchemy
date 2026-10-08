#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "SERIAL_RUNTIME.H"
#include "KEYSTATE.H"
#include "AUDIO_ENGINE_SYMBOLS.H"
#include "CARTBOOT.H"
#include "IO_REG.H"

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
