#include "system.h"
#include "callback_scheduler.h"
#include "input.h"
#include "oam.h"
#include "serial_runtime.h"
#include "io_reg.h"
#include "frame.h"
#include "cartboot.h"

#define SLEEP_IDLE_FRAMES 10800
#define RGB_WHITE 0x7fff

#define Io_Write16(value_, reg_) do { \
    /* FAKEMATCH: plain halfword stores pool the sleep constants; a word-valued temporary preserves their measured register construction. */ \
    u32 word_ = (u16)(value_); \
    *(reg_) = word_; \
} while (0)

static __inline__ void System_Reset(void)
{
    /* FAKEMATCH: the ordinary indirect call uses r5/r6 and returns through LR; this register carries the native ARM restart target in r4. */
    register u32 entry __asm__("r4");
    gCartridgeResetMarker = 0x19670704;
    AudioEngine_SuspendDirectSoundFar();
    entry = Cart_Restart;
    Io_Write16(0, &REG_IME);
    CAMELOT_ASM("the cartridge restart transfers control without returning.")
    __asm__ volatile("bx %0" : : "r"(entry));
}

static __inline__ void Bios_Stop(void)
{
    CAMELOT_ASM("BIOS Stop has no C instruction.")
    __asm__ volatile("swi 3");
}

void WaitFrames(s32 frames)
{
    u32 i;
    u32 line;
    s32 j;
    s16 display;
    s16 backdrop;

    for (i = 0; i < (u32)frames; i++) {
        gSchedulerStatus = 1;
        Scheduler_RunCallbacksByKey(0x480);
        gSchedulerStatus = 0;
        if (gRenderOamEnabled) {
            Render_BuildOamList(Runtime_AllocateHeapBlock(80, 0x400));
        } else {
            gOamUsage.used = 0;
            gOamUsage.limit = 256;
        }
        gFrameRenderPending = 1;
        if (gCpuLoadDisplayEnabled) {
            line = *(u16 *)&REG_VCOUNT;
            if (line >= 160) line -= 160;
            else line += 68;
            line += (gLagFrameCount - 1) << 8;
            if (gCpuLoadTimer == 0) gCpuLoadPeak = 0;
            else gCpuLoadTimer--;
            if (gCpuLoadPeak < line) {
                gCpuLoadPeak = line;
                gCpuLoadTimer = 30;
            }
        }
        if (gSleepDisabled == 0) {
            if (gAutoSleepEnabled) {
                if (gInput.held) gIdleFrameCount = 0;
                else {
                    gIdleFrameCount++;
                    {
                        u32 idle; u32 limit;
                        limit = SLEEP_IDLE_FRAMES >> 6;
                        idle = gIdleFrameCount;
                        /* FAKEMATCH: under the ARM9TDMI load schedule plain source shifts the idle limit before the volatile counter reload; finish the reload before the limit's shift, with neither value changed. */
                        __asm__ ("" : "+r"(idle) : "r"(limit));
                        if (idle > (limit << 6) + (SLEEP_IDLE_FRAMES & 63)) gSleepRequested = 1;
                    }
                }
            }
            {
                u32 held;
                held = gInput.held;
                /* FAKEMATCH: plain source starts the sleep-button comparison constant before loading held buttons; leave held unchanged while the load goes first. */
                __asm__ volatile("" : : "r"(held));
                if (held == (KEYS_SHOULDERS | KEY_SELECT)) gSleepRequested = 1;
            }
        }
        if (gDebugMode) {
            for (;;) {
                if (gDebugPaused) {
                    if (gInput.repeat & (KEY_A | KEY_B | KEY_SELECT)) break;
                    if (gInput.held & KEYS_DPAD) break;
                    if (gInput.repeat & KEY_START) { gDebugPaused = 0; break; }
                } else {
                    if (gInput.held != (KEY_START | KEY_SELECT)) break;
                    gDebugPaused = 1;
                }
                System_WaitForFrameInterrupt();
                Input_UpdateKeyRepeatAndDirection();
                if (gResetRequested) {
                    gResetRequested = 0;
                    System_Reset();
                }
            }
        }
        gLagFramesShown = gLagFrameCount;
        gLagFrameCount = 0;
        System_WaitForFrameInterrupt();
        Runtime_ReleaseHeapBlock(80);
        Graphics_ResetFrameState();
        gFrameCount++;
        gFrameWaitCount++;
        Input_UpdateKeyRepeatAndDirection();
        if (gSerialExchangeActive) {
            SerialRuntime_PollStatus();
            if (gSerialRuntime.mode) gSerialRuntime.transfer_enabled = 1;
        }
        if (gSleepRequested && gSleepDisabled == 0) {
            display = REG_DISPCNT;
            backdrop = PLTT_BACKDROP;
            if (gSleepRequested == 1) {
                Io_Write16(0, &REG_DISPCNT);
                Io_Write16(RGB_WHITE, &PLTT_BACKDROP);
                for (j = 9; j >= 0; j--) System_WaitForFrameInterrupt();
                while (gInput.held) System_WaitForFrameInterrupt();
                gSleepActive = 1;
                {
                    u32 high; volatile u16 *key;
                    /* FAKEMATCH: the prior direct form loads the wake address before the genuine flag store; end its scratch-register reuse first. */
                    __asm__ volatile("" : : : "r5");
                    high = KEYCNT_SLEEP_WAKE >> 8;
                    key = &REG_KEYCNT;
                    *key = (high << 8) + (KEYCNT_SLEEP_WAKE & 255);
                }
                Bios_SoundBiasOff();
                Bios_Stop();
                Bios_SoundBiasOn();
                Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
                Io_Write16(0, &gSleepActive);
                for (j = 9; j >= 0; j--) System_WaitForFrameInterrupt();
                while (gInput.held) System_WaitForFrameInterrupt();
                Io_Write16(display, &REG_DISPCNT);
                Io_Write16(backdrop, &PLTT_BACKDROP);
                gSleepRequested = 0;
                gIdleFrameCount = 0;
            } else gSleepRequested--;
        }
        if (gResetRequested) {
            gResetRequested = 0;
            System_Reset();
        }
    }
}

void Runtime_SetMainState19(void)
{
    gInput.repeat_timer = 19;
}
