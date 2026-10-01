/* NONMATCHING: 2026-10-01 brief Wave2 FillWords direct-call attempt.
 * Remaining difference: a direct call changes Menu_CancelSoundTick from ldr r3, .L0+16 to ldr r0, .L0+12 (67/67 assembly lines).
 * Measured with TLA's existing agscc option set; no option changes.
 * Production retains the measured boundary with its reason inside the helper.
 */
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"

/* ☀️'s menu cancel sound and palette entries. ⚓️'s tick also blinks the
   cancel marker's tiles every fourth frame and waits for the window's
   pending transfers. */

struct InputState {
    u32 held;
    u32 pressed;
};

extern struct InputState gInput;
extern u32 Data_0300122c;
extern u8 Data_08105948[];

typedef s32 (*CopyFn)(void *, const void *, s32);
typedef s32 (*FillFn)(void *, s32, u32);

void Audio_PlayCue(s32 cue);
s32 GameFlag_Test(s32);
s32 GameFlag_SetBit(s32);
s32 GameFlag_ClearBit(s32);
s32 UiWork_SetParamNibbleFar(s32);

static __inline__ void CopyWords(const void *src, CopyFn copy, void *dst, s32 size)
{
    /* FAKEMATCH: a direct call changes Menu_CancelSoundTick from ldr r1, .L0+4 to ldr r0, .L0+4 (67/67 assembly lines). */
    copy(dst, src, size);
}

/* FAKEMATCH: ⚓️ loads the fill routine before its destination and value; a
   plain Iwram_FillWords call loads the destination first */


s32 Audio_PlayCueReturnOne(s32 cue)
;

void Menu_CancelSoundTick(void)
{
    u32 frame;
    u32 *wait;
    struct InputState *input;
    u32 mask;

    frame = Data_0300122c;
    if ((frame & 3) == 0) {
        if (frame & 4)
            CopyWords(Data_08105948, Iwram_CopyWords, (void *)0x06002540, 32);
        else
            Iwram_FillWords((void *)0x06002540, 32, 0x44444444);
    }
    input = &gInput;
    mask = 8;
    asm volatile("" : "+l"(input), "+l"(mask)); /* FAKEMATCH: ⚓️ sets the mask between the address and its load */
    if (input->pressed & mask) {
        Audio_PlayCue(113);
        GameFlag_SetBit(0x150);
        Scheduler_RemoveCallback((u32)((s32)Menu_CancelSoundTick));
    }
    wait = (u32 *)(Ram_HeapSlots->window_tiles + 0xE30);
    while ((wait = (u32 *)*wait) != 0)
        ;
}

void Menu_CancelSoundReset(void)
;

void Menu_EnsureCancelSound(void)
;

void UiPalette_SetColor15(void)
;

void UiPalette_SetColor2(void)
;

void UiPalette_SetColor4(void)
;
