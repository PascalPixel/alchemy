/* WaitFrames: TLA frame dispatch, OAM, debug pause and sleep.
 * Current raw extent: 840 bytes including all literal pools.
 * 2026-10-02: best complete native-listing score 240: four reordered
 * instructions, no remaining operand/register changes. EN full linked extent
 * 840/840 bytes, 16 bytes different at +0xd2, +0xe4, +0x17a and +0x244
 * (four adjacent instruction swaps); every scalar/pool/call byte outside
 * those ranges equals the owned EN ROM. Other five native editions pending.
 * Ordinary source 3645; typed I/O writes 1825; ARM restart transfer 965;
 * plain sleep halfword and one-read VCOUNT 240. 1,716 finite rewrites and
 * local comparison/block changes did not improve it. Three additional
 * tagged scheduling boundaries regressed to 3195 and were reverted.
 * No edition links this draft; all six native extents remain scaffolded.
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
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Render_BuildOamList(void *work);
void Func_080134b0(void);
void Func_080138b4(void);
void Func_081c0080(void);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08013ffc(void);
s32 Func_08016430(void);
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
                    if (Data_03001218 > 0x2a30) Data_030011d0 = 1;
                }
            }
            if (gInput.held == 0x304) Data_030011d0 = 1;
        }
        if (Data_03001238) {
            for (;;) {
                if (Data_03001214) {
                    if (gInput.repeat & 7) break;
                    if (gInput.held & 0xf0) break;
                    if (gInput.repeat & 8) { Data_03001214 = 0; break; }
                } else {
                    if (gInput.held != 12) break;
                    Data_03001214 = 1;
                }
                Func_080134b0();
                Func_080138b4();
                if (Data_030011c0) {
                    Data_030011c0 = 0;
                    System_Reset();
                }
            }
        }
        Data_030011d8 = Data_030011d4;
        Data_030011d4 = 0;
        Func_080134b0();
        Runtime_ReleaseHeapBlock(80);
        Func_08013ffc();
        Data_0300122c++;
        Data_0300117c++;
        Func_080138b4();
        if (Data_030011b8) {
            Func_08016430();
            if (gSerialRuntime.active) gSerialRuntime.transfer = 1;
        }
        if (Data_030011d0 && Data_03001180 == 0) {
            display = REG_DISPCNT;
            backdrop = PLTT_BACKDROP;
            if (Data_030011d0 == 1) {
                Io_Write16(0, &REG_DISPCNT);
                Io_Write16(0x7fff, &PLTT_BACKDROP);
                for (j = 9; j >= 0; j--) Func_080134b0();
                while (gInput.held) Func_080134b0();
                Data_02003000 = 1;
                Io_Write16(0xc304, &REG_KEYCNT);
                Bios_SoundBiasOff();
                Bios_Stop();
                Bios_SoundBiasOn();
                Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
                Io_Write16(0, &Data_02003000);
                for (j = 9; j >= 0; j--) Func_080134b0();
                while (gInput.held) Func_080134b0();
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
