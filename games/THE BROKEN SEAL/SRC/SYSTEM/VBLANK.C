#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MAP_SCROLL.H"
#include "TYPES.H"
#include "DMA.H"
#include "SERIAL_RUNTIME.H"
#include "IO_REG.H"
#include "HEAP_STATE.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"
#include "FRAME.H"
#include "KEYSTATE.H"

extern u16 gLinkStatus;

#define SOUND_PRESET_COUNT 5
#define SOUND_PRESET_SIZE 152


void System_VBlankHandler(void)
{
    u32 keys;
    void (*hook)(void);

    {
        volatile u16 *dma0 = REG_DMA0;

        dma0[5] &= ~DMA_START_HBLANK_REPEAT;
        dma0[5] &= ~DMA_ENABLE;
        dma0[5];
    }
    if (gSerialExchangeActive != 0) {
        u16 *status = &gLinkStatus;
        *status = SerialRuntime_ExchangePayloads(&gSerialTransfer, gSerialPeerPayloads);
        SerialRuntime_StepBlockTransfer();
    }
    MusicPlayer_StepVolumeAndPitchTowardTargetsFar();
    BlendTransition_Update();
    if (gFrameRenderPending != 0) {
        if (gOamCopyEnabled != 0)
            Dma_Set(((union HeapState *)gWorkSlot)->slots[52], OAM, DMA_ENABLE32 | DMA_32BIT | 0x100, REG_DMA3);
        Dma_Set(gBgScroll, REG_BG0HOFS, DMA_ENABLE32 | DMA_32BIT | 4, REG_DMA3);
        IoWriteQueue_FlushPending();
        gFrameRenderPending = 0;
    }
    hook = Data_03001cfc;
    if (hook != 0) {
        Data_03001cfc = 0;
        hook();
    }
    Runtime_InvokeCallbacksByKey(1152);
    keys = REG_KEYINPUT ^ KEYS_MASK;
    {
        u32 pressed = keys & ~gKeysHeld;

        gKeyState = pressed;
        gKeysPressedLatch |= pressed;
    }
    /* FAKEMATCH: the volatile store keeps the held-key reload in the repeat test after the write; the shared cell's declared type is unchanged. */
    *(volatile u32 *)&gKeysHeld = keys;
    if (keys == 0) {
        gKeyRepeatTimer = 19;
        gKeysRepeat = keys;
    } else if (gKeysHeld & (gKeysHeldAtVBlank ^ 0xffff)) {
        gKeyRepeatTimer = -1;
        gKeysRepeat = keys;
    } else if (gKeyRepeatTimer > 0) {
        gKeyRepeatTimer--;
    }
    gKeysHeldAtVBlank = keys;
    gFrameTick++;
    gLagFrameCount++;
    gVBlankDone = 1;
    SoundDriver_EnterMixer();
}

s32 Sound_LoadPresetParameters(s32 preset)
{
    s32 index;

    index = preset;
    if ((u32)preset > SOUND_PRESET_COUNT - 1) {
        index = 0;
    }
    Dma_Set(IwramCodeRecords + index * SOUND_PRESET_SIZE, Data_03000bd8, 0x84000026,
            REG_DMA3);
    return 0;
}

void Sound_ReservedNoOpA(void)
{
}

void Sound_ReservedNoOpB(void)
{
}
