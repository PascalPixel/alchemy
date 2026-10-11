#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "callback_scheduler.h"
#include "irq.h"
#include "types.h"
#include "scene.h"
#include "resource.h"
#include "resource_ids.h"
#include "global_cells.h"
#include "cartboot.h"
#include "ram_buffer.h"
#include "serial_runtime.h"
#include "frame.h"
#include "keystate.h"
#include "input.h"
#include "oam.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "irqlock.h"

extern u8 gDebugMode;
extern u8 gCpuLoadDisplayEnabled;
extern u8 gOamCopyEnabled;

void WaitFrames(s32 frames);

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define HALT_ON_GAMEPAK_IRQ 1
#endif

void System_Initialize(void)
{
    volatile u32 zero;

    {
        volatile u16 *dma0 = REG_DMA0;

        dma0[5] &= 0xc5ff;
        dma0[5] &= 0x7fff;
        dma0[5];
    }
    /* FAKEMATCH: the do-while wraps pin the register writes in place. */
    do { u32 value = WAITCNT_GAME; REG_WAITCNT = value; } while (0);
    zero = 0;
    Dma_Set((const void *)&zero, (void *)0x03000000, 0x85001e00, REG_DMA3);
    Runtime_InitializeHeap();
    Runtime_InstallIwramAndIrqs();
    *(u32 *)&gIoWriteQueue.count = 0;
    gDebugTextEnabled = 0;
    gDebugMode = 0;
    gCpuLoadDisplayEnabled = 0;
    Resource_LoadWorkHeader();
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    REG_DISPCNT = 0x140;
    Runtime_SetIrqHandler(0, 1, (void (*)(void))System_VBlankHandler);
#if defined(HALT_ON_GAMEPAK_IRQ)
    Runtime_SetIrqHandler(13, 1, IwramHalt);
#endif
    do { u32 value = KEYCNT_SOFT_RESET; REG_KEYCNT = value; } while (0);
    Audio_InitializeRuntimeDefaultsFar();
    Resource_InitializeTable();
    Scheduler_ResetTaskTable();
    gCartridgeResetMarker = 0;
    gOamCopyEnabled = 1;
    gSleepDisabled = 0;
    WaitFrames(10);
    Game_ResetForNewGameFar(0);
}

void Runtime_IdleForever(void)
{
    for (;;) {
        (void)*(volatile u32 *)&gKeyState;
        WaitFrames(1);
    }
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)

s32 System_GetLanguage(void)
{
    s32 language;

    switch (Rom_LanguageCode) {
    case 'D':
        language = 1;
        break;
    case 'F':
        language = 2;
        break;
    case 'S':
        language = 3;
        break;
    case 'I':
        language = 4;
        break;
    default:
        language = 0;
        break;
    }
    return language;
}
#endif

void RuntimeDispatch_ReservedNoOpA(void)
{
}

void RuntimeDispatch_ReservedNoOpB(void)
{
}

void RuntimeDispatch_ReservedNoOpC(void)
{
}

void RuntimeDispatch_ReservedNoOpD(void)
{
}

void RuntimeDispatch_ReservedNoOpE(void)
{
}

s32 RuntimeDispatch_ReturnZero(void)
{
    return 0;
}

struct ResourceWork {
    u32 header[3];
    u32 cursor;
};

void Resource_LoadWorkHeader(void)
{
    const void *header;
    struct ResourceWork *work;

    header = (const void *)Resource_GetTableEntry((u32)&ResourceId_BuildStamp);
    work = (struct ResourceWork *)&IwramHi_Data1;
    Dma_Set(header, work, 0x84000003, REG_DMA3);
    work->cursor = 0;
}

void RuntimeDispatch_NoOpHook(void)
{
}

void *Resource_GetTableEntry(s32 index)
{
    return Resource_DirectoryTable[index];
}

void PackedTable_AdjustMarkedOffsets(u16 *entries, s32 byte_length)
{
    s32 entry_count;
    s32 entry_index;

    entry_count = byte_length >> 1;
    entry_index = 1;
    entries++;
    if (entry_index < entry_count) {
        u32 next_marker_mask;
        u32 current_marker_mask;
        u32 payload_mask = 0x7ff;
        next_marker_mask = 0xf800;
        current_marker_mask = 0xf000;
        entries -= 2;
        do {
            u16 current_entry;
            u16 next_entry;
            u32 next_marker_bits;
            u32 current_marker_bits;

            next_entry = entries[2];
            next_marker_bits = next_entry & next_marker_mask;
            entries++;
            if (next_marker_bits == next_marker_mask) {
                current_entry = entries[0];
                current_marker_bits = current_entry & next_marker_bits;
                if (current_marker_bits == current_marker_mask) {
                    u32 packed_offset;
                    u32 adjusted_current;
                    u32 adjusted_next;

                    packed_offset = (((current_entry & payload_mask) << 12)
                        | ((next_entry & payload_mask) * 2)) - (entry_index * 2);
                    adjusted_current = ((packed_offset >> 12) & payload_mask)
                        | current_marker_bits;
                    adjusted_next = ((packed_offset >> 1) & payload_mask)
                        | next_marker_bits;
                    entries[0] = adjusted_current;
                    entries[1] = adjusted_next;
                }
            }
            entry_index++;
        } while (entry_index < entry_count);
    }
}

void Resource_LoadCode(u32 index, void *destination)
{
    void (*routine)(void *, s32);
    s32 end;
    u32 size;

    end = Resource_DecodeType01((const void *)Resource_GetTableEntry(index), destination);
    /* FAKEMATCH: the wrapper keeps the size load after the decode. */
    do {
        size = (u32)Resource_PatchThumbBranchCodeSize;
    } while (0);
    routine = (void (*)(void *, s32))MemAlloc(size);
    Dma_Set((const void *)Resource_PatchThumbBranch, routine, 0x84000000 | (size >> 2), REG_DMA3);
    routine(destination, end);
    MemFree(routine);
}

void Runtime_IgnoreInterrupt(void)
{
}

void Runtime_InstallIwramAndIrqs(void)
{
    volatile u16 *ime;
    s32 zero;
    u8 *iwram;
    s32 value;
    s32 one;

    ime = &REG_IME;
    zero = 0;
    /* FAKEMATCH: the do-while keeps the REG_IME store ahead of the IWRAM address load. */
    do {
        *ime = zero;
    } while (0);
    iwram = IwramIrqMain;
    Dma_Set((const void *)IwramRuntime_Rom, iwram, 0x84000500, REG_DMA3);
    gIrqVector = iwram;
    Dma_Set((const void *)Runtime_IrqHandlers, (void *)gIrqHandlers, 0x8400000e, REG_DMA3);
    REG_DISPSTAT = zero;
    value = 0xc3ff;
    REG_KEYCNT = value;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    value = 0x3001;
#else
    value = 0x1001;
#endif
    REG_IE = value;
    one = 1;
    *ime = one;
}

#define Io_Write16(v, reg)                                                     \
    do { \
        /* FAKEMATCH: the word temporary and one-pass boundary retain measured halfword-value construction and pool-load scheduling. */ \
        u32 value_ = (u16)(v);                                                 \
        *(reg) = value_;                                                       \
    } while (0)

static __inline__ void System_SoftReset(void)
{
    /* FAKEMATCH: a void-returning entry pointer places the target in r1; the value-returning pointer preserves the measured r0 call. */
    s32 (*reset)(void) = (s32 (*)(void))Cart_Restart;

    gCartridgeResetMarker = 0x19670704;
    Io_Write16(0, &REG_IME);
    reset();
}

#define VBlankIntrWait()                                                       \
    {                                                                          \
        gVBlankDone &= ~1;                                                     \
        do {                                                                   \
            CAMELOT_ASM("Halt, which no C compiles to")                    \
            __asm__ volatile("swi 2");                                         \
        } while (!(gVBlankDone & 1));                                          \
    }

void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler)
{
    if (irq <= 13) {
        u32 ime;
        u32 bit;
        u32 ie;

        IRQ_LOCK(ime);
        bit = 1 << irq;
        ie = REG_IE & ~bit;
        if (handler != 0)
            ie |= bit;
        REG_IE = ie;
        if (irq <= 2) {
            u32 enable = 8 << irq;
            u32 keep = ~enable;
            u32 stat;

            if (irq == 2) {
                enable |= vcount << 8;
                keep &= 0xff;
            }
            stat = REG_DISPSTAT & keep;
            if (handler != 0)
                stat |= enable;
            REG_DISPSTAT = stat;
        }
        if (handler != 0)
            gIrqHandlers[irq] = handler;
        else
            gIrqHandlers[irq] = Runtime_IgnoreInterrupt;
        IRQ_UNLOCK(ime);
    }
}

void WaitFrames(s32 frames)
{
    u8 *sp;
    u32 i;
    u8 *base;
    u32 line;
    s32 j;
    s16 dispcnt;
    s16 backdrop;

    CAMELOT_ASM("read the hardware stack pointer for the proven stack switch")
    __asm__ volatile("mov %0, sp" : "=r"(sp));

    base = sp;
    if (base < Ram_FrameWaitStackTop) {
        u8 *top = Ram_FrameWaitStackTop;
        u8 *next;

        gSavedStackSize = top - base;
        Dma_Set(base, gSavedStack, (gSavedStackSize >> 2) | DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
        /* FAKEMATCH: needed to match: XOR cancellation defers the top-copy
         * guard until after allocation, retaining top in r4 across DMA.
         * Without this lifetime boundary, top and the DMA control constant
         * exchange r4/r5 (12 aligned differences, same 0x434-byte extent).
         * The redundant guard emits no extra comparison or copy. */
        next = (u8 *)((u32)top ^ (u32)base);
        next = (u8 *)((u32)next ^ (u32)base);
        /* FAKEMATCH: the tied top operand retains its DMA lifetime; an input-only operand adds a copy through ip. */
        CAMELOT_ASM("the stack switch, a write to sp no C can express")
        __asm__ volatile("mov sp, %0" : "+r"(top) : : "sp", "memory");
        do {
            if (top == next)
                break;
            top = next;
        } while (0);
    }
    for (i = 0; i < frames; i++) {
        gSchedulerStatus = 1;
        Runtime_InvokeCallbacksByKey(0xc80);
        gSchedulerStatus = 0;
        Render_BuildOamList(Runtime_AllocateHeapBlock(52, 0x400));
        gFrameRenderPending = 1;
        if (gCpuLoadDisplayEnabled) {
            line = *(u16 *)&REG_VCOUNT;
            if (line >= 160)
                line -= 160;
            else
                line += 68;
            line += (gLagFrameCount - 1) << 8;
            if (gCpuLoadTimer == 0)
                gCpuLoadPeak = 0;
            else
                gCpuLoadTimer--;
            if (gCpuLoadPeak < line) {
                gCpuLoadPeak = line;
                gCpuLoadTimer = 30;
            }
        }
        if (gSleepDisabled == 0) {
            if (gOptionMirror) {
                if (gKeysHeld)
                    gPostLoadCounter = 0;
                else {
                    gPostLoadCounter++;
                    if (gPostLoadCounter > 3 * 60 * 60)
                        gSleepRequested = 1;
                }
            }
            if (gKeysHeld == KEYS_SHOULDERS) {
                gSleepComboFrames++;
                if (gSleepComboFrames >= 180) {
                    gSleepComboFrames = 0;
                    gSleepRequested = 1;
                }
            } else {
                gSleepComboFrames = 0;
            }
        }
        if (gDebugMode) {
            for (;;) {
                if (gDebugPaused) {
                    if ((*(u32 *)&gKeysRepeat) & (KEY_A | KEY_B | KEY_SELECT))
                        break;
                    if (gKeysHeld & KEYS_DPAD)
                        break;
                    /* FAKEMATCH: a volatile read keeps the measured reread of the repeated keys. */
                    if ((*(volatile u32 *)&gKeysRepeat) & KEY_START) {
                        gDebugPaused = 0;
                        break;
                    }
                } else {
                    if (gKeysHeld != (KEY_START | KEY_SELECT))
                        break;
                    gDebugPaused = 1;
                }
                CAMELOT_ASM("Halt in VBlankIntrWait")
                VBlankIntrWait();
                Input_UpdateKeyRepeatAndDirection();
                if (gResetRequested) {
                    gResetRequested = 0;
                    System_SoftReset();
                }
            }
        }
        gLagFramesShown = gLagFrameCount;
        gLagFrameCount = 0;
        CAMELOT_ASM("Halt in VBlankIntrWait")
        VBlankIntrWait();
        Runtime_ReleaseHeapBlock(52);
        Graphics_ResetFrameState();
        gFrameCount++;
        gLoadedStateWord++;
        Input_UpdateKeyRepeatAndDirection();
        if (gSerialExchangeActive) {
            SerialRuntime_PollStatus();
            if (gSerialRuntime.mode)
                gSerialRuntime.transfer_enabled = 1;
        }
        if (gSleepRequested && gSleepDisabled == 0) {
            dispcnt = REG_DISPCNT;
            backdrop = PLTT_BACKDROP;
            if (gSleepRequested == 1) {
                Io_Write16(0, &REG_DISPCNT);
                Io_Write16(0x7fff, &PLTT_BACKDROP);
                for (j = 0; j < 60; j++)
                    CAMELOT_ASM("Halt in VBlankIntrWait")
                    VBlankIntrWait();
                gSleepActive = 1;
                Io_Write16(KEYCNT_SHOULDERS_WAKE, &REG_KEYCNT);
                Bios_SoundBiasOff();
                CAMELOT_ASM("Stop, which no C compiles to")
                __asm__ volatile("swi 3");
                Bios_SoundBiasOn();
                Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
                gSleepActive = 0;
                Io_Write16(dispcnt, &REG_DISPCNT);
                Io_Write16(backdrop, &PLTT_BACKDROP);
                for (j = 0; j < 10; j++)
                    CAMELOT_ASM("Halt in VBlankIntrWait")
                    VBlankIntrWait();
                gSleepRequested = 0;
                gPostLoadCounter = 0;
            } else {
                gSleepRequested--;
            }
        }
        if (gResetRequested) {
            gResetRequested = 0;
            System_SoftReset();
        }
    }
    {
        u32 *size = &gSavedStackSize;

        if (*size) {
            /* FAKEMATCH: needed to match: XOR cancellation keeps the initial base
             * live across the frame loop. A plain sp assignment shrinks the frame
             * from eight bytes to four (65 aligned differences); the one-pass
             * boundary retains the restore copy's order. */
            u8 *restore;

            /* FAKEMATCH: tying the old base to the hardware read keeps the eight-byte frame and removes an extra copy through ip. */
            CAMELOT_ASM("read the hardware stack pointer for its restore copy")
            __asm__ volatile("mov %0, sp" : "+r"(base));
            sp = base;
            restore = (u8 *)((u32)sp ^ (u32)base);

            restore = (u8 *)((u32)restore ^ (u32)base);
            do {
                base = restore;
            } while (0);
            /* FAKEMATCH: needed to match: the one-pass copy guard retains the
             * current-SP copy in sl. A plain restore loses that copy and differs
             * by five aligned instructions (403 native, 402 built, 400 common),
             * shrinking 0x434 bytes to 0x430. The redundant guard ends CSE at the
             * copy without emitting an extra comparison. */
            do {
                if (base == sp)
                    break;
                base = sp;
            } while (0);
            /* FAKEMATCH: needed to match: the volatile reread keeps this saved-size
             * load separate from the conditional read (7 aligned differences
             * without this qualification). */
            base -= *(volatile u32 *)size;
            /* FAKEMATCH: needed to match: the cancelling restore identity keeps
             * the result in base before installing sp. A direct assignment swaps
             * the native high-register copy pair (4 aligned differences, same
             * 0x434-byte extent); the identity restores both without a binding. */
            restore = (u8 *)((u32)base ^ (u32)sp);
            restore = (u8 *)((u32)restore ^ (u32)sp);
            sp = restore;
            CAMELOT_ASM("restore the hardware stack pointer after the proven stack copy")
            __asm__ volatile("mov sp, %0" : : "r"(sp) : "sp", "memory");
            /* FAKEMATCH: clearing the old address and restoring a separate
             * pointer in complementary conditions ends the restore CSE block.
             * The conditions vanish, retaining the size-address reload in r3
             * and its load into r2. Reusing size instead of next_size changes
             * that pair to r2/r2 (4 aligned differences, same 0x434 extent);
             * deleting the empty barrier alone gives 24 differences. */
            size = NULL;
            {
                u32 *next_size = NULL;

                if (sp)
                    next_size = &gSavedStackSize;
                if (!sp)
                    next_size = &gSavedStackSize;
                Dma_Set(gSavedStack, base, (*next_size >> 2) | DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
            }
            while (DMA3->control & DMA_ENABLE32)
                ;
            gSavedStackSize = 0;
        }
    }
}

void Runtime_SetMainState19(void)
{
    gKeyRepeatTimer = 19;
}

#define KEY_REPEAT_FIRST 19
#define KEY_REPEAT_NEXT  6

void Input_UpdateKeyRepeatAndDirection(void)
{
    /* FAKEMATCH: volatile reads and the repeat clear preserve separate input-cell rereads. */
    s32 delay = gKeyRepeatTimer;
    u32 keys;
    u32 axis = 0;
    u32 dirs;
    u32 mask;
    volatile u32 *repeat;

    if (delay <= 0) {
        gKeysRepeat = gKeysHeld;
        keys = *(volatile u32 *)&gKeysRepeat;
        if (delay == 0)
            gKeyRepeatTimer = KEY_REPEAT_NEXT;
        else
            gKeyRepeatTimer = KEY_REPEAT_FIRST;
    } else {
        *(volatile u32 *)&gKeysRepeat = 0;
        keys = gKeysRepeat;
    }

    if (keys != 0) {
        dirs = 0;
        if (keys & KEY_UP)
            dirs = 1;
        if (keys & KEY_DOWN)
            dirs++;
        if (keys & KEY_LEFT)
            dirs++;
        if (keys & KEY_RIGHT)
            dirs++;

        repeat = &gKeysRepeatDirected;
        *repeat = keys;
        switch (dirs) {
        default:
            gKeysDirection = KEYS_HORIZONTAL;
            mask = 0xffff & ~KEYS_DPAD;
            *repeat &= mask;
            break;
        case 0:
            gKeysDirection = KEYS_HORIZONTAL;
            break;
        case 1:
            gKeysDirection = keys & KEYS_DPAD;
            break;
        case 2:
            if ((*(volatile u32 *)&gKeysDirection & *repeat) == 0)
                gKeysDirection = KEYS_HORIZONTAL;
            *repeat &= *(volatile u32 *)&gKeysDirection ^ 0xffff;
            break;
        case 3:
            if (gKeysDirection & KEYS_HORIZONTAL)
                axis = KEYS_HORIZONTAL;
            if (*(volatile u32 *)&gKeysDirection & KEYS_VERTICAL)
                axis = KEYS_VERTICAL;
            mask = 0xffff ^ axis;
            gKeysDirection = keys & mask;
            *repeat &= mask;
            break;
        }
    } else {
        gKeysRepeatDirected = keys;
    }

    *(volatile u32 *)&gKeyState =
        (gKeysHeld ^ gKeysHeldLast)
        & *(volatile u32 *)&gKeysHeld;
    gKeysHeldLast = *(volatile u32 *)&gKeysHeld;
}

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
        *status = SerialRuntime_ExchangePayloads((void *)&gSerialTransfer, (void *)gSerialPeerPayloads);
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
    hook = gVBlankHook;
    if (hook != 0) {
        gVBlankHook = 0;
        hook();
    }
    Runtime_InvokeCallbacksByKey(0x480);
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
    Dma_Set(IwramCodeRecords + index * SOUND_PRESET_SIZE, IwramSoundPresetCode, 0x84000026,
            REG_DMA3);
    return 0;
}

void Sound_ReservedNoOpA(void)
{
}

void Sound_ReservedNoOpB(void)
{
}
