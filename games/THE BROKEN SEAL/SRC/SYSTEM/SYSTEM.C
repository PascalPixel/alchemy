#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "IO_REG.H"
#include "GLOBAL_CELLS.H"

typedef void (*InterruptHandler)(void);
extern InterruptHandler Data_030000e0[];
void RuntimeDispatch_ReservedNoOp03008(void);
void Runtime_InvokeCallbacksByKey(s32 key);
u8 *Runtime_AllocateHeapBlock(s32, s32);
void Runtime_CopyAndCallRoutine(void *argument);
void Runtime_ReleaseHeapBlock(s32 slot);
void Graphics_ResetFrameState(void);
s32 SerialRuntime_PollStatus(void);
void Input_UpdateKeyRepeatAndDirection(void);
void Func_08006868(void);
void Func_08006870(void);
extern u32 gSavedStackSize;
extern u8 gSavedStack[];
extern u8 Data_03001e44;
extern u8 Data_03001f58;
extern u16 Data_03001ccc;
extern u16 gLagFramesShown;
extern u32 gCpuLoadTimer;
extern u32 gCpuLoadPeak;
extern volatile u8 Data_03001ca0;
extern u8 gOptionMirror;
extern u32 gKeysHeld;
extern volatile u32 gKeysRepeat;
extern volatile u16 gPostLoadCounter;
extern volatile u8 gSleepRequested;
extern volatile u16 gSleepComboFrames;
extern u8 gDebugMode;
extern u8 gDebugPaused;
extern volatile u16 Data_03001d28;
extern u8 Data_03001cb8;
extern u32 Data_03007800;
extern u32 gFrameCount;
extern u32 gLoadedStateWord;
extern u16 gSerialExchangeActive;
extern u8 gSerialRuntime[];
extern u16 gSleepActive;

/* FAKEMATCH: halfword register writes pass through a u16 argument, so the
   ROM keeps each value in a register (movs, not a pooled halfword) and
   zero-extends the saved signed ones */
#define Io_Write16(v, reg)                                                     \
    do { \
        /* FAKEMATCH: the halfword-to-word register temporary preserves measured value allocation. */ \
        /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
        u32 value_ = (u16)(v);                                                 \
        *(reg) = value_;                                                       \
    } while (0)

static __inline__ void System_SoftReset(void)
{
    s32 (*reset)(void) = (s32 (*)(void))0x08000000;

    Data_03007800 = 0x19670704;
    Io_Write16(0, &REG_IME);
    reset();
}

struct DmaChannel {
    u32 source;
    u32 destination;
    u32 control;
};

#define STACK_TOP 0x03007a00
#define VBlankIntrWait()                                                       \
    {                                                                          \
        Data_03001d28 &= ~1;                                                   \
        do {                                                                   \
            /* CAMELOT_ASM: Halt, which no C compiles to */                    \
            __asm__ volatile("swi 2");                                         \
        } while (!(Data_03001d28 & 1));                                        \
    }

extern u8 Data_03001b00[];

/* Installs or removes the handler for one interrupt with IME off: sets
   the IE bit, the DISPSTAT enable (and the VCOUNT target for IRQ 2), and
   the IWRAM table entry, which falls back to the no-op handler. */
void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler)
{
    if (irq <= 13) {
        volatile u16 *ime_reg = (volatile u16 *)0x04000208;
        u32 ime;
        u32 bit;
        u32 ie;

        ime = *ime_reg;
        *ime_reg = (u32)ime_reg;
        /* FAKEMATCH: an empty do-while keeps the IE address load after the
           IME write */
        do {
        } while (0);
        bit = 1 << irq;
        ie = *(volatile u16 *)0x04000200 & ~bit;
        if (handler != 0)
            ie |= bit;
        *(volatile u16 *)0x04000200 = ie;
        if (irq <= 2) {
            u32 enable = 8 << irq;
            u32 keep = ~enable;
            u32 stat;

            if (irq == 2) {
                enable |= vcount << 8;
                keep &= 0xff;
            }
            stat = *(volatile u16 *)0x04000004 & keep;
            if (handler != 0)
                stat |= enable;
            *(volatile u16 *)0x04000004 = stat;
        }
        if (handler != 0)
            Data_030000e0[irq] = handler;
        else
            Data_030000e0[irq] = RuntimeDispatch_ReservedNoOp03008;
        *(volatile u16 *)0x04000208 = ime;
    }
}

/* WaitFrames (2026-09-29): exact. The soft reset calls the cartridge entry
   through a pointer returning a value, which keeps the address in r0 as
   the ROM does; a void pointer put it in r1.
   What lined up: sl as a register variable the end's sp read keeps live,
   so i and frames spill; a bare asm (no outputs, no clobbers) for the
   second sp write, which flushes CSE and reloads gSavedStackSize's
   address; the do/while (0) register-write macro, whose loop notes keep
   sched2 from moving pool loads across the stores; volatile idle, combo,
   sleep and key-repeat counters, which the ROM re-reads after each store;
   and the soft reset as one inline with the entry address in a local. */
void WaitFrames(s32 frames)
{
    u32 i;
    /* CAMELOT_ASM: the saved stack pointer stays in sl */
    register u8 *base __asm__("sl");
    u32 size;
    u32 line;
    s32 j;
    s16 dispcnt;
    s16 backdrop;

    /* CAMELOT_ASM: reads the stack pointer */
    __asm__ volatile("mov %0, sp" : "=r"(base));
    if ((u32)base < STACK_TOP) {
        gSavedStackSize = STACK_TOP - (u32)base;
        Dma_Set(base, gSavedStack, (gSavedStackSize >> 2) | 0x84000000, REG_DMA3);
        /* CAMELOT_ASM: moves the stack pointer */
        __asm__ volatile("mov sp, %0" : : "r"(STACK_TOP));
    }
    for (i = 0; i < frames; i++) {
        gSchedulerStatus = 1;
        Runtime_InvokeCallbacksByKey(0xc80);
        gSchedulerStatus = 0;
        Runtime_CopyAndCallRoutine(Runtime_AllocateHeapBlock(52, 0x400));
        Data_03001e44 = 1;
        if (Data_03001f58) {
            line = *(u16 *)0x04000006;
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
        if (Data_03001ca0 == 0) {
            if (gOptionMirror) {
                if (gKeysHeld)
                    gPostLoadCounter = 0;
                else {
                    gPostLoadCounter++;
                    if (gPostLoadCounter > 0x2a30)
                        gSleepRequested = 1;
                }
            }
            if (gKeysHeld == 0x300) {
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
                    if (gKeysRepeat & 7)
                        break;
                    if (gKeysHeld & 0xf0)
                        break;
                    if (gKeysRepeat & 8) {
                        gDebugPaused = 0;
                        break;
                    }
                } else {
                    if (gKeysHeld != 12)
                        break;
                    gDebugPaused = 1;
                }
                /* CAMELOT_ASM: Halt in VBlankIntrWait */
                VBlankIntrWait();
                Input_UpdateKeyRepeatAndDirection();
                if (Data_03001cb8) {
                    Data_03001cb8 = 0;
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
            if (gSerialRuntime[0])
                gSerialRuntime[8] = 1;
        }
        if (gSleepRequested && Data_03001ca0 == 0) {
            dispcnt = REG_DISPCNT;
            backdrop = PLTT_BACKDROP;
            if (gSleepRequested == 1) {
                Io_Write16(0, &REG_DISPCNT);
                Io_Write16(0x7fff, &PLTT_BACKDROP);
                for (j = 0; j < 60; j++)
                    /* CAMELOT_ASM: Halt in VBlankIntrWait */
                    VBlankIntrWait();
                gSleepActive = 1;
                Io_Write16(0xc300, &REG_KEYCNT);
                Func_08006868();
                /* CAMELOT_ASM: Stop, which no C compiles to */
                __asm__ volatile("swi 3");
                Func_08006870();
                Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
                gSleepActive = 0;
                Io_Write16(dispcnt, &REG_DISPCNT);
                Io_Write16(backdrop, &PLTT_BACKDROP);
                for (j = 0; j < 10; j++)
                    /* CAMELOT_ASM: Halt in VBlankIntrWait */
                    VBlankIntrWait();
                gSleepRequested = 0;
                gPostLoadCounter = 0;
            } else {
                gSleepRequested--;
            }
        }
        if (Data_03001cb8) {
            Data_03001cb8 = 0;
            System_SoftReset();
        }
    }
    if (gSavedStackSize) {
        /* CAMELOT_ASM: reads the stack pointer */
        __asm__ volatile("mov %0, sp" : "+r"(base) : : "memory");
        base -= gSavedStackSize;
        /* CAMELOT_ASM: moves the stack pointer */
        __asm__ volatile("mov sp, %0" : : "r"(base));
        Dma_Set(gSavedStack, base, (gSavedStackSize >> 2) | 0x84000000, REG_DMA3);
        while (((volatile struct DmaChannel *)REG_DMA3)->control & 0x80000000)
            ;
        gSavedStackSize = 0;
    }
}

void Runtime_SetMainState19(void)
{
    *(s32 *)((u32)&Data_03001b00) = 0x13;
}
