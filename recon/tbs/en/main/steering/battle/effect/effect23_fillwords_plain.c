/* NONMATCHING: 2026-10-01 brief Wave2 FillWords plain-source attempt.
 * Removing this one source device changes BattlePres_ProcessPendingTileTransfer.
 * Remaining difference: a direct call changes BattlePres_ProcessPendingTileTransfer from mov r0, r4 to lsl r1, r1, #8 (86/86 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * Production retains the measured helper with a body-local FAKEMATCH reason.
 */
#include "TYPES.H"
#include "DMA.H"
#include "IWRAM_CALL.H"
#include "RESOURCE.H"

extern struct GameState gGameState;
extern u8 Data_03001cb4[];

/* runtime/blank_display_load_value_and_run.c */
s32 Audio_PlayCue(s32);
s32 Unnamed_080f7460(void);


extern u8 *gBattleFxWork[2];
void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);

#define ABS(v) ((v) < 0 ? -(v) : (v))

s32 Runtime_BlankDisplayLoadValueAndRun(void)
;

/* graphics/color/Palette_ScaleRgb555.c */
s32 Graphics_ScaleRgb555(
    u16 *source,
    u16 *destination,
    s32 scale,
    s32 count)
;

/* Runs the pending BG tile transfer once the effect requests it: a plain
   copy to 0x06003500 followed by a refill, or the halved or three-quarter
   colour backup; otherwise counts the frames since the last transfer. */
void BattlePres_ProcessPendingTileTransfer(void)
{
    u8 *work;
    u8 *buffer;

    work = gBattleFxWork[0];
    if (*(s32 *)(work + 0x7824) == 1) {
        buffer = gBattleFxWork[1];
        switch (*(s32 *)(work + 0x7780)) {
        case 1:
            Dma_Set(buffer, (void *)0x06003500, 0x84002000, (volatile u32 *)0x040000d4);
            Iwram_FillWords(buffer, 0x8000, *(s32 *)(work + 0x7784));
            break;
        case 2:
            if (*(s32 *)(work + 0x7784) == 50)
                ColorBuffer_BackupAndHalveNonzero(buffer, (u8 *)0x06003500, 0x8000);
            else
                ColorBuffer_BackupAndScaleNonzeroThreeQuarters(buffer, (u8 *)0x06003500, 0x8000);
            break;
        }
        *(s32 *)(work + 0x7824) = 0;
        *(s32 *)(work + 0x7820) = 1;
    } else {
        (*(s32 *)(work + 0x7820))++;
    }
}

/* Darken background entries 160-175 and object entries 1-239 by one step
   per channel, stopping each 5-bit channel at 0. */
void Palette_DarkenSceneStep(void)
;

/* Step background colours 1-63 one unit per channel towards the palette of
   resource_id. */
void Palette_StepTowardResource(s32 resource_id)
;

/* Draw a line into the effect canvas (8bpp tiles, 32 tiles to a row) with
   an 8.8 fraction, keeping the larger of the existing and new colour. The
   major axis is walked from its lower end. */
void BattleFx_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color)
;
