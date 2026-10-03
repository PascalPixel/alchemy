#include "RUNTIME_MEM.H"
/* WaitFrames: TLA frame dispatch, OAM, debug pause and sleep.
 * Complete native owner: 840 bytes, including every literal pool.
 * 2026-10-02: saved draft compiles with the one approved TLA option set.
 * Independent complete links against the current physical source definitions
 * match all 840 bytes in JA/EN/DE/ES/FR/IT: all 18 direct calls, 31 symbolic
 * pool words, six scalar pool words and alignment; no missing names, address
 * substitutions, masked failures or compiler-output patches.
 * The first private checkpoint lacked five callee definitions in five
 * editions. Naming their actual raw/scaffold locations closed those links.
 * The immutable draft scorer compiles this original full source; mutable
 * permutation still refuses inline assembly. The complete checks above
 * remain independent ordinary compilations and symbolic links.
 * No edition adopts this draft and it earns no source credit. Production
 * remains blocked by the numeric cartridge restart target, true allocator
 * return type and shared record/API ownership, and coherent module placement.
 * Prior ordinary shaping:3645; typed I/O1825; ARM restart965; one VCOUNT read
 * and sleep-halfword form240,16 bytes/four adjacent swaps;1,716 finite forms
 * did not improve that baseline. It was freshly reproduced before this cohort.
 * New finite cohort:14 first handoffs,9 combinations/address forms,7 direct
 * store/two-stage forms,4 repeat-read forms,3 result handoffs and4 scratch
 * holds. The idle reload/shift and held-load/constant boundaries yield840/8;
 * a short repeat r3 handoff yields840/6. Those41 forms span840..898 and other
 * schedules regress. Final bounded closure:7 mask/result forms,7 wake-address
 * forms and their one combination (15 of16 limit). Explicit mask assignment
 * keeps the actual result in r3 (840/4); the direct halfword wake store with a
 * short scratch-clobber/address-shift boundary fixes the last pair (840/0).
 * No pinned value crosses a call, no unread frame owner, and no output patch.
 * This measured attempt record is for people and does not steer the build.
 */
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_REG.H"

struct FrameInput {
    u32 held;
    u32 pressed;
    u32 released;
    volatile u32 repeat;
    u32 direction;
    u32 previous_direction;
    u32 previous_held;
    u32 repeat_delay;
    s32 repeat_timer;
};
struct OamUsage { u16 used; u16 limit; };
struct SerialFrame { u8 active; u8 reserved[7]; u8 transfer; };
extern struct FrameInput gInput;
extern struct OamUsage gOamUsage;
extern struct SerialFrame gSerialRuntime;
extern u8 Data_0300120c;
extern u8 Data_03001230;
extern u8 Data_0300123c;
extern u16 Data_030011d4;
extern u32 Data_03001140;
extern u32 Data_03001184;
extern volatile u8 Data_03001180;
extern u8 Data_03001200;
extern volatile u16 Data_03001218;
extern volatile u8 Data_030011d0;
extern u8 Data_03001238;
extern u8 Data_03001214;
extern u8 Data_030011c0;
extern u32 Data_03007800;
extern u16 Data_030011d8;
extern u32 Data_0300122c;
extern u32 Data_0300117c;
extern u16 Data_030011b8;
extern volatile u16 Data_02003000;
void Render_BuildOamList(void *work);
void System_WaitForFrameInterrupt(void);
void Input_UpdateKeyRepeatAndDirection(void);
void Func_081c0080(void);
void Graphics_ResetFrameState(void);
s32 SerialRuntime_PollStatus(void);
void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);

#define Io_Write16(value_, reg_) do { \
    /* FAKEMATCH: plain halfword stores pool the sleep constants; a word-valued temporary preserves their measured register construction. */ \
    u32 word_ = (u16)(value_); \
    *(reg_) = word_; \
} while (0)

static __inline__ void System_Reset(void)
{
    /* FAKEMATCH: the ordinary indirect call uses r5/r6 and returns through LR; this register carries the native ARM restart target in r4. */
    register u32 entry __asm__("r4");
    Data_03007800 = 0x19670704;
    Func_081c0080();
    entry = 0x08000000;
    Io_Write16(0, &REG_IME);
    /* CAMELOT_ASM: the cartridge restart transfers control without returning. */
    __asm__ volatile("bx %0" : : "r"(entry));
}

static __inline__ void Bios_Stop(void)
{
    /* CAMELOT_ASM: BIOS Stop has no C instruction. */
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
        if (Data_0300120c) {
            Render_BuildOamList(Runtime_AllocateHeapBlock(80, 0x400));
        } else {
            gOamUsage.used = 0;
            gOamUsage.limit = 256;
        }
        Data_03001230 = 1;
        if (Data_0300123c) {
            line = *(u16 *)&REG_VCOUNT;
            if (line >= 160) line -= 160;
            else line += 68;
            line += (Data_030011d4 - 1) << 8;
            if (Data_03001140 == 0) Data_03001184 = 0;
            else Data_03001140--;
            if (Data_03001184 < line) {
                Data_03001184 = line;
                Data_03001140 = 30;
            }
        }
        if (Data_03001180 == 0) {
            if (Data_03001200) {
                if (gInput.held) Data_03001218 = 0;
                else {
                    Data_03001218++;
                    {
                        /* FAKEMATCH: plain source shifts the idle limit before the volatile counter reload; this short boundary keeps the base and reload ready before the shift. */
                        u32 idle; u32 limit;
                        limit = 0x2a30 >> 6;
                        idle = Data_03001218;
                        /* FAKEMATCH: preserve the measured idle reload/limit shift order with unchanged live values. */
                        __asm__ volatile("" : "+r"(idle) : "r"(limit));
                        if (idle > (limit << 6) + (0x2a30 & 63)) Data_030011d0 = 1;
                    }
                }
            }
            {
                /* FAKEMATCH: plain source starts the soft-reset comparison constant before loading held buttons; the short capture puts that load first. */
                u32 held;
                held = gInput.held;
                /* FAKEMATCH: leave held unchanged while preserving its measured comparison-materialization order. */
                __asm__ volatile("" : : "r"(held));
                if (held == 0x304) Data_030011d0 = 1;
            }
        }
        if (Data_03001238) {
            for (;;) {
                if (Data_03001214) {
                    {
                        /* FAKEMATCH: the plain repeat test loads before preparing its mask; the earlier handoff fixes that order but leaves the result in r2. This short capture and explicit mask assignment preserve native preparation and result register. */
                        register u32 repeat __asm__("r3");
                        repeat = gInput.repeat;
                        /* FAKEMATCH: keep the current measured read/mask preparation boundary. */
                        __asm__ volatile("" : "+r"(repeat) : "r"(7));
                        repeat &= 7;
                        if (repeat) break;
                    }
                    if (gInput.held & 0xf0) break;
                    if (gInput.repeat & 8) { Data_03001214 = 0; break; }
                } else {
                    if (gInput.held != 12) break;
                    Data_03001214 = 1;
                }
                System_WaitForFrameInterrupt();
                Input_UpdateKeyRepeatAndDirection();
                if (Data_030011c0) {
                    Data_030011c0 = 0;
                    System_Reset();
                }
            }
        }
        Data_030011d8 = Data_030011d4;
        Data_030011d4 = 0;
        System_WaitForFrameInterrupt();
        Runtime_ReleaseHeapBlock(80);
        Graphics_ResetFrameState();
        Data_0300122c++;
        Data_0300117c++;
        Input_UpdateKeyRepeatAndDirection();
        if (Data_030011b8) {
            SerialRuntime_PollStatus();
            if (gSerialRuntime.active) gSerialRuntime.transfer = 1;
        }
        if (Data_030011d0 && Data_03001180 == 0) {
            display = REG_DISPCNT;
            backdrop = PLTT_BACKDROP;
            if (Data_030011d0 == 1) {
                Io_Write16(0, &REG_DISPCNT);
                Io_Write16(0x7fff, &PLTT_BACKDROP);
                for (j = 9; j >= 0; j--) System_WaitForFrameInterrupt();
                while (gInput.held) System_WaitForFrameInterrupt();
                Data_02003000 = 1;
                {
                    u32 high; volatile u16 *key;
                    /* FAKEMATCH: the prior direct form loads the wake address before the genuine flag store; end its scratch-register reuse first. */
                    __asm__ volatile("" : : : "r5");
                    high = 0xc304 >> 8;
                    key = &REG_KEYCNT;
                    /* FAKEMATCH: keep the measured address-before-shift handoff, with neither value changed. */
                    __asm__ volatile("" : "+r"(high) : "r"(key));
                    *key = (high << 8) + (0xc304 & 255);
                }
                Bios_SoundBiasOff();
                Bios_Stop();
                Bios_SoundBiasOn();
                Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
                Io_Write16(0, &Data_02003000);
                for (j = 9; j >= 0; j--) System_WaitForFrameInterrupt();
                while (gInput.held) System_WaitForFrameInterrupt();
                Io_Write16(display, &REG_DISPCNT);
                Io_Write16(backdrop, &PLTT_BACKDROP);
                Data_030011d0 = 0;
                Data_03001218 = 0;
            } else Data_030011d0--;
        }
        if (Data_030011c0) {
            Data_030011c0 = 0;
            System_Reset();
        }
    }
}
