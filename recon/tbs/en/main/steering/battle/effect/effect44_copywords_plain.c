/* NONMATCHING: 2026-10-01 brief Wave2 CopyWords plain-source attempt.
 * Removing this one source device changes BattlePresentation_ProcessPendingGraphicsTransfer, BattleFx_FlushPendingGraphicsTransfer.
 * Remaining difference: a direct call changes BattlePresentation_ProcessPendingGraphicsTransfer from push {r5, r6, lr} to push {r5, r6, r7, lr} (122/122 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * Production retains the measured helper with a body-local FAKEMATCH reason.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "BATTLE_EFFECT_WORK.H"
#include "RAM_BUFFER.H"

extern u8 gWorkSlot[];
u32 Random16(void);
void WaitFrames(s32 frames);

extern u8 gBattleFxWork[];
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))


static __inline__ void FillWords(void *destination, s32 size, s32 value)
{
    /* FAKEMATCH: a direct call changes BattlePresentation_ProcessPendingGraphicsTransfer from mov r0, r5 to lsl r1, r1, #7 (122/122 assembly lines). */
    Iwram_FillWords(destination, size, value);
}

void ColorBuffer_BackupAndHalve(void *source, void *destination, s32 size);
void ColorBuffer_BackupAndScaleThreeQuarters(void *source, void *destination, s32 size);
void ColorBuffer_BackupAndDarken(void *source, s32 amount, void *destination, s32 size);
void ColorBuffer_BackupAndBrighten(void *source, s32 amount, void *destination, s32 size);

/* Battle effect: wipe the 128 by 128 canvas (16 by 16 tiles of 8 by 8
   bytes) with a ragged front. Each of the 128 lanes waits a random 0..63
   steps; the front then accelerates every frame, across the canvas when
   mode is 1 and down it otherwise, writing 1 - value into every pixel it
   passes. */
void BattleEffect_WipeCanvas(s32 mode, s32 value)
;

/* Battle presentation: once per frame, flush the effect canvas to VRAM when
   an effect marked it pending, in the transfer mode the effect chose (plain
   copy, copy and clear, one of two blends, or a fade by amount); otherwise
   count the frames since the last flush. */
void BattlePresentation_ProcessPendingGraphicsTransfer(void)
{
    void **heap_cache = (void **)gBattleFxWork;
    void *work = heap_cache[0];
    void *source;
    s32 *counter;
    s32 next_counter;

    if (FIELD(work, s32, 0x7824) == 1) {
        source = heap_cache[1];
        switch (FIELD(work, u32, 0x7780)) {
        case 0:
            Iwram_CopyWords((void *)0x06004000, source, 0x4000);
            break;
        case 1:
            Iwram_CopyWords((void *)0x06004000, source, 0x4000);
            FillWords(source, 0x4000, FIELD(work, s32, 0x7784));
            break;
        case 2:
            if (FIELD(work, s32, 0x7784) == 50) {
                ColorBuffer_BackupAndHalve(source, (void *)0x06004000, 0x4000);
            } else {
                ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
            }
            break;
        case 3:
            ColorBuffer_BackupAndDarken(source, FIELD(work, s32, 0x7784),
                (void *)0x06004000, 0x4000);
            break;
        case 4:
            ColorBuffer_BackupAndBrighten(source, FIELD(work, s32, 0x7784),
                (void *)0x06004000, 0x4000);
            break;
        }
        FIELD(work, s32, 0x7824) = 0;
        counter = (s32 *)((u8 *)work + 0x7820);
        next_counter = 1;
    } else {
        counter = (s32 *)((u8 *)work + 0x7820);
        next_counter = *counter + 1;
    }
    *counter = next_counter;
}

/* Flush the battle compositor's pending display transfer. */
void BattleFx_FlushPendingGraphicsTransfer(void)
;
