#ifndef ALCHEMY_UNIT_BATTLE_SCREEN_H
#define ALCHEMY_UNIT_BATTLE_SCREEN_H

#include "types.h"

struct BattleTransitionEntry;

s32 BattlePres_RunMosaicFadeOut(void);
void Graphics_BuildSequentialTileTable(s32 *dst);
void BattlePresentation_BuildTilemap(s32 *destination);
void BattlePres_UpdateHBlankScroll(void);
void BattlePresentation_UploadTileVariant(void);
void BattlePres_AdvanceTransitionTimer(void);
void BattlePres_DrawTransitionRows(void);
void Graphics_ClearBg0Vofs(void);
void BattlePres_RunEntryTransition(s32 enemy_count, s32 kind);
void BattlePresentation_SetPaletteLevel(s32 unused, s32 level);
void BattlePresentation_ConfigurePaletteFade(s32 mode, s32 value, s32 fade);
void BattlePresentation_InitializeWorkAndResetState(void);
void Runtime_ReleaseHeapBlock10(void);
void BattleBackground_Load(s32 mode, s32 resource, s32 level);
void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale);
void BattlePres_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y);
void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 zoom);
void BattlePres_SetupTransitionAtPairMidpoint(s32 a, s32 b, s32 zoom);
void Graphics_FadeAlphaOut(void);
void Graphics_FadeAlphaIn(void);
void Graphics_SetBlendControl(void);
void BattlePresentation_InitializeTransitionEntries(struct BattleTransitionEntry *entries);
s32 BattlePres_DivideBy16(s32 value);
s32 Runtime_AdjustDebugValueWithButtons(s32 ret);
void BattlePres_SetActorRecordMode(s32 actor_id, s32 mode);
void BattlePres_SetOtherActorRecordModes(s32 except);
void BattlePres_ClearAllActorRecordModes(void);
void Graphics_AdvancePaletteCycle(void);
void BattlePres_SetActorModes(u16 *actors, s32 mode);

#endif
