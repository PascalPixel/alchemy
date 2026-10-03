#ifndef ALCHEMY_PALBUF_H
#define ALCHEMY_PALBUF_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* TitlePalette_InitializeBuffers creates the two-bank title palette fade. */
struct TitlePaletteWork {
    u16 palette[512];
    u16 current[0x600];
    u16 target[0x600];
    u16 delta[0x600];
    u16 packed[2][512];
    u8 page;
    s8 duration;
    u8 step;
    u8 padding_3003;
};

LAYOUT_SIZE_GUARD(TitlePaletteWork_Size, struct TitlePaletteWork, 0x3004);
LAYOUT_OFFSET_GUARD(TitlePaletteWork_Current, struct TitlePaletteWork, current, 0x400);
LAYOUT_OFFSET_GUARD(TitlePaletteWork_Target, struct TitlePaletteWork, target, 0x1000);
LAYOUT_OFFSET_GUARD(TitlePaletteWork_Delta, struct TitlePaletteWork, delta, 0x1c00);
LAYOUT_OFFSET_GUARD(TitlePaletteWork_Packed, struct TitlePaletteWork, packed, 0x2800);
LAYOUT_OFFSET_GUARD(TitlePaletteWork_Page, struct TitlePaletteWork, page, 0x3000);

void TitlePalette_InitializeBuffers(void);
void TitlePalette_UpdateFade(void);
void Graphics_TransformLargePalette(s32 index, s32 transform);
void Graphics_SetPaletteTransformValue(s32 value);
void Graphics_UpdatePaletteInterpolation(s32 frames);
void Graphics_InterpolatePaletteBuffers(
    s16 *source, s16 *target, s16 *step, s32 frames);
void Runtime_ScheduleCallbackAndReleaseBlock32A(void);

#endif
