#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT7_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT7_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"

struct DisplayTransitionState;

void BattleFx_StartWindowHBlankDma(void);
void DisplayTransition_UpdateScanlineTable(void);
void DisplayTransition_InitializeBattleEffectState(s32 mode);
void BattleFx_EnableTwoCallbacks(void);
struct DisplayTransitionState *DisplayTransition_AllocateAndClearState(void);
void DisplayTransition_Start(s32 mode, s32 frames);
void DisplayTransition_Finish(s32 mode, s32 frames);
void DisplayState_ClearFlags(s32 clear_0800, s32 clear_0400, s32 clear_0200);
void DisplayTransition_Update(void);
void DisplayTransition_UpdateFromCentre(void);
void DisplayTransition_UpdateScanline(void);
void DisplayTransition_UpdateFrame(void);
void DisplayTransition_FillTilemapAndSolidTile(s32 color);
void DisplayTransition_InitializeState(s32 value);

#endif
