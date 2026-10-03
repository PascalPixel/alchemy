#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "IO_REG.H"
#include "SYSTEM.H"
#include "IRQ.H"
#include "CARTBOOT.H"
#include "RAM_BUFFER.H"
#include "SERIAL_RUNTIME.H"
#include "FRAME.H"
#include "KEYSTATE.H"
#include "INPUT.H"
#include "OAM.H"

#define Io_Write16(v, reg)                                                     \
    do { \
        /* FAKEMATCH: the word temporary and one-pass boundary retain measured halfword-value construction and pool-load scheduling. */ \
        u32 value_ = (u16)(v);                                                 \
        *(reg) = value_;                                                       \
    } while (0)

static __inline__ void System_SoftReset(void)
{
    /* FAKEMATCH: a void-returning entry pointer places the target in r1;
       the value-returning pointer preserves the measured r0 call. */
    s32 (*reset)(void) = (s32 (*)(void))Cart_Restart;

    Data_03007800 = 0x19670704;
    Io_Write16(0, &REG_IME);
    reset();
}

#define VBlankIntrWait()                                                       \
    {                                                                          \
        *(volatile u16 *)&Data_03001d28 &= ~1;                                 \
        do {                                                                   \
            /* CAMELOT_ASM: Halt, which no C compiles to */                    \
            __asm__ volatile("swi 2");                                         \
        } while (!(*(volatile u16 *)&Data_03001d28 & 1));                       \
    }

/* Installs or removes the handler for one interrupt with IME off: sets
   the IE bit, the DISPSTAT enable (and the VCOUNT target for IRQ 2), and
   the IWRAM table entry, which falls back to the no-op handler. */
void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler)
{
    if (irq <= 13) {
        volatile u16 *ime_reg = &REG_IME;
        u32 ime;
        u32 bit;
        u32 ie;

        ime = *ime_reg;
        /* FAKEMATCH: the address value leaves IME bit 0 clear and keeps
           the measured address-register store. A plain zero store adds
           a literal load and splits the pool (124 to 136 bytes). */
        *ime_reg = (u32)ime_reg;
        /* FAKEMATCH: an empty do-while keeps the IE address load after the
           IME write */
        do {
        } while (0);
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
        REG_IME = ime;
    }
}

/* Runs frame callbacks, rendering and input while preserving a caller's
   active IWRAM stack across interrupt work. */
void WaitFrames(s32 frames)
{
    u32 i;
    /* FAKEMATCH: keeping the saved stack pointer in sl forces measured index/count spills. */
    register u8 *base __asm__("sl");
    u32 line;
    s32 j;
    s16 dispcnt;
    s16 backdrop;

    /* FAKEMATCH: volatile accesses retain the measured rereads of the idle,
       sleep, shoulder-combo and debug-repeat cells after their stores. */
    /* CAMELOT_ASM: reads the stack pointer */
    __asm__ volatile("mov %0, sp" : "=r"(base));
    if ((u32)base < (u32)Ram_FrameWaitStackTop) {
        gSavedStackSize = (u32)Ram_FrameWaitStackTop - (u32)base;
        Dma_Set(base, gSavedStack, (gSavedStackSize >> 2) | 0x84000000, REG_DMA3);
        /* CAMELOT_ASM: moves the stack pointer */
        __asm__ volatile("mov sp, %0" : : "r"((u32)Ram_FrameWaitStackTop));
    }
    for (i = 0; i < frames; i++) {
        gSchedulerStatus = 1;
        Runtime_InvokeCallbacksByKey(0xc80);
        gSchedulerStatus = 0;
        Render_BuildOamList(Runtime_AllocateHeapBlock(52, 0x400));
        Data_03001e44 = 1;
        if (Data_03001f58) {
            line = *(u16 *)&REG_VCOUNT;
            if (line >= 160)
                line -= 160;
            else
                line += 68;
            line += (Data_03001ccc - 1) << 8;
            if (gCpuLoadTimer == 0)
                gCpuLoadPeak = 0;
            else
                gCpuLoadTimer--;
            if (gCpuLoadPeak < line) {
                gCpuLoadPeak = line;
                gCpuLoadTimer = 30;
            }
        }
        if ((*(volatile u8 *)&Data_03001ca0) == 0) {
            if (gOptionMirror) {
                if (gKeysHeld)
                    (*(volatile u16 *)&gPostLoadCounter) = 0;
                else {
                    (*(volatile u16 *)&gPostLoadCounter)++;
                    if ((*(volatile u16 *)&gPostLoadCounter) > 0x2a30)
                        (*(volatile u8 *)&gSleepRequested) = 1;
                }
            }
            if (gKeysHeld == KEYS_SHOULDERS) {
                (*(volatile u16 *)&gSleepComboFrames)++;
                if ((*(volatile u16 *)&gSleepComboFrames) >= 180) {
                    (*(volatile u16 *)&gSleepComboFrames) = 0;
                    (*(volatile u8 *)&gSleepRequested) = 1;
                }
            } else {
                (*(volatile u16 *)&gSleepComboFrames) = 0;
            }
        }
        if (gDebugMode) {
            for (;;) {
                if (gDebugPaused) {
                    if ((*(volatile u32 *)&gKeysRepeat) & (KEY_A | KEY_B | KEY_SELECT))
                        break;
                    if (gKeysHeld & KEYS_DPAD)
                        break;
                    if ((*(volatile u32 *)&gKeysRepeat) & KEY_START) {
                        gDebugPaused = 0;
                        break;
                    }
                } else {
                    if (gKeysHeld != (KEY_START | KEY_SELECT))
                        break;
                    gDebugPaused = 1;
                }
                /* CAMELOT_ASM: Halt in VBlankIntrWait */
                VBlankIntrWait();
                Input_UpdateKeyRepeatAndDirection();
                if (gResetRequested) {
                    gResetRequested = 0;
                    System_SoftReset();
                }
            }
        }
        gLagFramesShown = Data_03001ccc;
        Data_03001ccc = 0;
        /* CAMELOT_ASM: Halt in VBlankIntrWait */
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
        if ((*(volatile u8 *)&gSleepRequested) && (*(volatile u8 *)&Data_03001ca0) == 0) {
            dispcnt = REG_DISPCNT;
            backdrop = PLTT_BACKDROP;
            if ((*(volatile u8 *)&gSleepRequested) == 1) {
                Io_Write16(0, &REG_DISPCNT);
                Io_Write16(0x7fff, &PLTT_BACKDROP);
                for (j = 0; j < 60; j++)
                    /* CAMELOT_ASM: Halt in VBlankIntrWait */
                    VBlankIntrWait();
                gSleepActive = 1;
                Io_Write16(0xc300, &REG_KEYCNT);
                Bios_SoundBiasOff();
                /* CAMELOT_ASM: Stop, which no C compiles to */
                __asm__ volatile("swi 3");
                Bios_SoundBiasOn();
                Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
                gSleepActive = 0;
                Io_Write16(dispcnt, &REG_DISPCNT);
                Io_Write16(backdrop, &PLTT_BACKDROP);
                for (j = 0; j < 10; j++)
                    /* CAMELOT_ASM: Halt in VBlankIntrWait */
                    VBlankIntrWait();
                (*(volatile u8 *)&gSleepRequested) = 0;
                (*(volatile u16 *)&gPostLoadCounter) = 0;
            } else {
                (*(volatile u8 *)&gSleepRequested)--;
            }
        }
        if (gResetRequested) {
            gResetRequested = 0;
            System_SoftReset();
        }
    }
    if (gSavedStackSize) {
        /* CAMELOT_ASM: reads the stack pointer */
        __asm__ volatile("mov %0, sp" : "+r"(base) : : "memory");
        base -= gSavedStackSize;
        /* FAKEMATCH: the operand-only asm flushes CSE, preserving the
           measured reload of the saved-stack-size address. */
        /* CAMELOT_ASM: moves the stack pointer */
        __asm__ volatile("mov sp, %0" : : "r"(base));
        Dma_Set(gSavedStack, base, (gSavedStackSize >> 2) | 0x84000000, REG_DMA3);
        while (((volatile struct DmaRegisters *)REG_DMA3)->control & DMA_ENABLE32)
            ;
        gSavedStackSize = 0;
    }
}

void Runtime_SetMainState19(void)
{
    Data_03001b00 = 19;
}
