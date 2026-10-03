#include "TYPES.H"
#include "DMA.H"
#include "SERIAL_RUNTIME.H"
#include "IO_REG.H"
#include "HEAP_STATE.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"

extern u16 gSerialExchangeActive;

/* VBlank runs the link exchange */
extern u16 gLinkStatus;

/* link exchange status */
extern u8 Data_03001e44;

/* display registers pending */
extern u8 gOamCopyEnabled;

/* OAM buffer pending */
extern u8 gBgScroll[];
extern void (*Data_03001cfc)(void);

/* one-shot VBlank hook */
extern volatile u32 gKeysHeld;

/* keys held */
extern u32 gKeyState;

/* keys newly pressed */
extern u32 gKeysPressedLatch;

/* presses since last read */
extern s32 Data_03001b00;

/* key repeat delay */
extern u32 gKeysRepeat;

/* key repeat keys */
extern u32 Data_03001d0c;

/* keys held last frame */
extern s32 gFrameTick;

/* frame counter */
extern u16 Data_03001ccc;
extern u16 Data_03001d28;
u16 SerialRuntime_ExchangePayloads(void *send, void *receive);
void SerialRuntime_StepBlockTransfer(void);
void Func_080f9018(void);
void BlendTransition_Update(void);
void Func_080006fc(void);

#define SOUND_PRESET_COUNT 5
#define SOUND_PRESET_SIZE 152
extern u8 Data_08000404[];

/* five 152-byte sound presets in ROM */
extern u8 Data_03000bd8[];

/* VBlank seen */

/* VBlank interrupt handler: stops H-blank DMA 0, runs the link exchange,
   sound and blend updates, flushes the OAM and display register buffers,
   runs the one-shot hook and the frame callbacks, then samples the keys. */
void System_VBlankHandler(void)
{
    u32 keys;
    void (*hook)(void);

    {
        /* Stop the H-blank scroll DMA; the read waits for it to settle. */
        volatile u16 *dma0 = REG_DMA0;

        dma0[5] &= ~DMA_START_HBLANK_REPEAT;
        dma0[5] &= ~DMA_ENABLE;
        dma0[5];
    }
    if (gSerialExchangeActive != 0) {
        u16 *status = &gLinkStatus;
        *status = SerialRuntime_ExchangePayloads((void *)&gSerialTransfer, (void *)gSerialPeerPayloads);
        SerialRuntime_StepBlockTransfer();
    }
    Func_080f9018();
    BlendTransition_Update();
    if (Data_03001e44 != 0) {
        if (gOamCopyEnabled != 0)
            Dma_Set(((union HeapState *)gWorkSlot)->slots[52], OAM, DMA_ENABLE32 | DMA_32BIT | 0x100, REG_DMA3);
        Dma_Set(gBgScroll, REG_BG0HOFS, DMA_ENABLE32 | DMA_32BIT | 4, REG_DMA3);
        IoWriteQueue_FlushPending();
        Data_03001e44 = 0;
    }
    hook = Data_03001cfc;
    if (hook != 0) {
        Data_03001cfc = 0;
        hook();
    }
    Runtime_InvokeCallbacksByKey(0x480);
    keys = REG_KEYINPUT ^ KEYS_MASK;
    {
        u32 pressed = keys & ~gKeysHeld;

        gKeyState = pressed;
        gKeysPressedLatch |= pressed;
    }
    gKeysHeld = keys;
    if (keys == 0) {
        Data_03001b00 = 19;
        gKeysRepeat = keys;
    } else if (gKeysHeld & (Data_03001d0c ^ 0xffff)) {
        Data_03001b00 = -1;
        gKeysRepeat = keys;
    } else if (Data_03001b00 > 0) {
        Data_03001b00--;
    }
    Data_03001d0c = keys;
    gFrameTick++;
    Data_03001ccc++;
    Data_03001d28 = 1;
    Func_080006fc();
}

/* active sound parameters in IWRAM */
s32 Sound_LoadPresetParameters(s32 preset)
{
    s32 index;

    index = preset;
    if ((u32)preset > SOUND_PRESET_COUNT - 1) {
        index = 0;
    }
    Dma_Set(Data_08000404 + index * SOUND_PRESET_SIZE, Data_03000bd8, 0x84000026,
            (volatile u32 *)0x040000d4);
    return 0;
}

/* Two empty routines between the preset loader and the key interrupt
   setup; nothing in the image calls either by name. */
void Sound_ReservedNoOpA(void)
{
}

void Sound_ReservedNoOpB(void)
{
}
