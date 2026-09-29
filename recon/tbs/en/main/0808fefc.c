/* 2026-09-29: alchemy permute (8 minutes) found 1000; its one natural
 * change, the display-control value written as dispcnt | DISPCNT rather
 * than DISPCNT | dispcnt, gives 1200 (18 register-only, 8 operand, 7
 * reordered, 2 inserted, 3 deleted) and is kept. The rest of the 1000
 * candidate reshuffles case 4 around its Value_ zero and 80 through
 * temporaries, which cannot be adopted anyway. */
/* Draft, not exact (2026-09-26): 708 of 708 bytes, 151 differing halfwords.
   Best complete model restored. Preserved experiments: plain halfword
   aggregate carriers with a queue-value snapshot gave 708 / 281;
   symbolic aggregate carriers with the original queue gave 704 / 154;
   explicit callback branches retained both initialization tails and
   moved the display pointer into r8, giving 780 / 360.
   Written from the listing; the sibling DisplayTransition_Finish (same
   state record and switch shape) matched. What lined up: the display
   control write is QueueIoWriteDelay2 written out as a macro so its value
   is read inside the free-entry test; the global display pointer reached
   through a volatile pointer so case 4 reloads it from r1; one call with a
   ternary callback in case 4; case 4's 0 and 80 as Value_ symbols so they
   take r8 and r9 and frames takes sl, as in the ROM.
   Remaining: the ROM's case 4 loads its two constants as short-reach
   (halfword) pool entries, which puts the first literal pool inside case 4
   after the then-branch; here the pool lands after case 3, so every pool
   offset and the jump table's case 4 entry differ, and the three queue
   blocks schedule adds r2, r2, r1 one slot early. Plain u8/u16 locals give
   the short reach but let CSE share 80 with the split stores and move
   display out of r7.
   2026-09-27 bounded rebrief: complete [0808fefc,080901c0), pools included.
   Fresh binary baseline confirms 708/708, 151 differing halfwords and
   104 aligned edits; the complete normalized diff was read.
   H1: transfer resource_371:020039fc's final one-pass IME restoration,
   exact at 69181996f and inspected from 1968849ba. These inline queue
   publications mask IME, fill one entry, then restore before state/callback
   work; the boundary is shared. Predict changed queue scheduling without
   changing the case-4 constants. Gate: all 708 bytes exact and full
   compare-all/test/coverage/verify. Budget: H1 plus at most two evidence-led
   follow-ups, stop by 01:05 Lisbon; preserve each result in this header.
   H1 rejected: binary and complete normalized diff are unchanged, confirmed
   by cmp against the baseline (708/708, 151 halfwords, 104 aligned edits).
   The final-restore scope does not repair this owner's queue address order
   or case-4 constant ownership. No bytes adopted. */
#include "TYPES.H"
#include "IO_WRITE_QUEUE.H"

struct DisplayTransitionState {
    u8 data[0x528];
    s16 value;
    s16 timer;
    u8 unknown_52c[8];
    s16 level;
    s16 step;
    u8 unknown_538[2];
    u8 start;
    u8 end;
    u8 frames;
    u8 phase;
};

struct DisplayWork {
    u8 unknown_00[0x14];
    u16 dispcnt;
    u8 unknown_16[0xea];
    u16 split_top;
    u16 split_bottom;
};

extern volatile u16 RegIme;
extern u8 Value_00000000;
extern u8 Value_00000050;

void *DisplayTransition_AllocateAndClearState(void);
void DisplayTransition_FillTilemapAndSolidTile(s32 color);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void WaitFrames(s32 frames);
void Runtime_SetIrqHandler(s32, s32, void (*)(void));
void Blend_SetDarkenTarget0(s32 duration);
void Blend_SetDarkenTarget16(s32 duration);
void BattleFx_ApplyColorToSourceBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void BattleFx_SetPrimaryBufferValue(unsigned int value);
void DisplayTransition_UpdateScanlineTable(void);
void BattleFx_StartWindowHBlankDma(void);
void DisplayTransition_UpdateFrame(void);
void DisplayTransition_Update(void);
void DisplayTransition_UpdateFromCentre(void);
void DisplayTransition_UpdateScanline(void);

/* QueueIoWriteDelay2 written out: the display control write for the next
   frame, the value read only once a queue entry is free.
   FAKEMATCH: one-pass final restore preserves the publication boundary,
   as in the exact world-map transfer queue. */
#define QUEUE_DISPLAY_CONTROL(value)                                        \
    do {                                                                    \
        volatile u16 *ime;                                                  \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        do {                                                                \
            ime = &RegIme;                                                 \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = 0x04000000;                                    \
            *destination = 0x20000;                                         \
        }                                                                   \
        do { *ime = saved; } while (0);                                     \
    } while (0)

/* Starts a screen transition; DisplayTransition_Finish ends it. The high
   byte of mode picks the effect, the low byte is kept in the state. */
void DisplayTransition_Start(s32 mode, s32 frames)
{
    struct DisplayWork *volatile *work;
    struct DisplayWork *display;
    s32 value;
    s32 kind;

    kind = (mode >> 8) & 0xff;
    work = (struct DisplayWork *volatile *)0x03001e70;
    display = *work;
    value = mode & 0xff;
    switch (kind) {
    case 0:
        Blend_SetDarkenTarget16(0);
        Blend_SetDarkenTarget0(frames);
        WaitFrames(1);
        break;
    case 1:
        BattleFx_ApplyColorToSourceBuffer(0x8000, *(u16 *)0x05000000);
        BattleFx_StartBufferInterpolation(frames);
        WaitFrames(1);
        QUEUE_DISPLAY_CONTROL(display->dispcnt | *(volatile u16 *)0x04000000);
        BattleFx_SetPrimaryBufferValue(0);
        return;
    case 2: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->value = value;
        state->timer = 0;
        state->level = 63;
        state->step = 1;
        Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
        Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
        WaitFrames(1);
        QUEUE_DISPLAY_CONTROL(display->dispcnt | *(volatile u16 *)0x04000000);
        state->start = 0;
        state->end = 32;
        state->frames = frames;
        state->phase = 0;
        return;
    }
    case 3: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->value = value;
        state->timer = 32;
        DisplayTransition_FillTilemapAndSolidTile(15);
        WaitFrames(1);
        Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
        QUEUE_DISPLAY_CONTROL(display->dispcnt | *(volatile u16 *)0x04000000);
        state->start = 0;
        state->end = 32;
        state->frames = frames;
        state->phase = 0;
        return;
    }
    case 4: {
        struct DisplayTransitionState *state;
        u16 start;
        u16 zero;

        display = *work;
        state = DisplayTransition_AllocateAndClearState();
        zero = (u16)(s32)&Value_00000000;
        start = (u8)(u32)&Value_00000050;
        display->split_top = 80;
        display->split_bottom = 80;
        WaitFrames(1);
        Scheduler_AddOrUpdateCallback(value == 0 ? DisplayTransition_Update : DisplayTransition_UpdateFromCentre, 0xc80);
        Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
        state->start = start;
        state->end = zero;
        state->frames = frames;
        state->phase = zero;
        break;
    }
    }
    QUEUE_DISPLAY_CONTROL(display->dispcnt | *(volatile u16 *)0x04000000);
}
