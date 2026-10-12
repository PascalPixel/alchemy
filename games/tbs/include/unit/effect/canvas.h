#ifndef ALCHEMY_UNIT_EFFECT_CANVAS_H
#define ALCHEMY_UNIT_EFFECT_CANVAS_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "unit/effect/effect.h"

void BattleEffect_WipeCanvas(s32 mode, s32 value);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_FlushPendingGraphicsTransfer(void);
void Graphics_ApplyWindowBlendRegisters(void);
void Display_ApplyBg2Reference(void);
void Palette_StepFadeTransfer(void);
void BattleFx_ClearActorTimers(void);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_BeginCanvasLayer(s32 bg_control);
void BattleFx_OpenCanvasLayer(s32 bg_control);
void BattleFx_BeginTiledCanvas(s32 bg_control);
void BattleFx_EndCanvasLayer(void);
void BattleFx_SetTransitionFlagAndDisplay(void);
void BattleFx_SetupCanvasTileMap(void);
void BattleFx_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);

#endif
