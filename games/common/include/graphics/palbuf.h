#ifndef ALCHEMY_PALBUF_H
#define ALCHEMY_PALBUF_H

#include "edition.h"
#include "types.h"
#include "layout_guard.h"

struct TitlePaletteWork
{
    u16 palette[512];
    u16 current[0x600];
    u16 target[0x600];
    u16 delta[0x600];
    u16 packed[2][512];
    u8  page;
    s8  duration;
    u8  step;
    u8  padding3003;
};

LAYOUT_SIZE_GUARD(TitlePaletteWork_Size, struct TitlePaletteWork, 0x3004);
LAYOUT_OFFSET_GUARD(TitlePaletteWork_Current, struct TitlePaletteWork, current, 0x400);
LAYOUT_OFFSET_GUARD(TitlePaletteWork_Target, struct TitlePaletteWork, target, 0x1000);
LAYOUT_OFFSET_GUARD(TitlePaletteWork_Delta, struct TitlePaletteWork, delta, 0x1c00);
LAYOUT_OFFSET_GUARD(TitlePaletteWork_Packed, struct TitlePaletteWork, packed, 0x2800);
LAYOUT_OFFSET_GUARD(TitlePaletteWork_Page, struct TitlePaletteWork, page, 0x3000);

struct BattleEffectBuffers
{
    u16 palette[0x1c0];
    u16 current[0x540];
    u16 target[0x540];
    u16 delta[0x540];
    u16 packed[2][0x1c0];
    u8  page;
    s8  duration;
    s8  step;
    u8  padding2a03;
};

LAYOUT_SIZE_GUARD(BattleEffectBuffers_Size, struct BattleEffectBuffers, 0x2a04);
LAYOUT_OFFSET_GUARD(BattleEffectBuffers_Current, struct BattleEffectBuffers, current, 0x380);
LAYOUT_OFFSET_GUARD(BattleEffectBuffers_Target, struct BattleEffectBuffers, target, 0xe00);
LAYOUT_OFFSET_GUARD(BattleEffectBuffers_Delta, struct BattleEffectBuffers, delta, 0x1880);
LAYOUT_OFFSET_GUARD(BattleEffectBuffers_Packed, struct BattleEffectBuffers, packed, 0x2300);
LAYOUT_OFFSET_GUARD(BattleEffectBuffers_Page, struct BattleEffectBuffers, page, 0x2a00);

extern u16 gPalette_ChannelCurve[32];
extern u16 gPalette_ChannelCurveLight[32];
extern u16 gPalette_ChannelCurveContrast[32];

enum PaletteTransform
{
    PALETTE_TRANSFORM_FROM_SOURCE = 0x8000,
    PALETTE_TRANSFORM_PLAIN = 0x10000,
    PALETTE_TRANSFORM_GREY = 0x10001,
    PALETTE_TRANSFORM_SEPIA = 0x10002,
    PALETTE_TRANSFORM_WARM = 0x10003,
    PALETTE_TRANSFORM_MUTED = 0x10004,
    PALETTE_TRANSFORM_PALE = 0x10005,
    PALETTE_TRANSFORM_RED = 0x10006,
    PALETTE_TRANSFORM_WARM2 = 0x10007,
    PALETTE_TRANSFORM_FLAGS = 0x100000,
    PALETTE_TRANSFORM_TINT = 0x200000,
    PALETTE_TRANSFORM_BLEND = 0x400000,
    PALETTE_TRANSFORM_SPLIT = 0x800000
};

#if GAME_TLA
void Graphics_TransformPaletteBuffer(u32 mode, u16 *source, u16 *destination, s32 half);

void TitlePalette_InitializeBuffers(void);
void TitlePalette_UpdateFade(void);
void Graphics_TransformLargePalette(s32 index, s32 transform);
void Graphics_SetPaletteTransformValue(s32 value);
void Graphics_UpdatePaletteInterpolation(s32 frames);
void Graphics_InterpolatePaletteBuffers(s16 *source, s16 *target, s16 *step, s32 frames);
void Runtime_ScheduleCallbackAndReleaseBlock32A(void);
#endif

#endif
