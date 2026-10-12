#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT13_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT13_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"

void DisplayScroll_ArmHBlankDma(void);
void DisplayScroll_BuildAndSwapHBlankPage(void);
void DisplayScroll_DisarmHBlankDma(void);
void FieldFx_UpdateGroundParticles(void);
void BattleFx_UpdateStormFlash(void);
void FieldFx_StartGroundParticles(void);
void FieldMotes_Update(void);
void FieldMotes_Start(void);
void FieldEffect_UpdateSparkles(void);
void FieldEffect_InitSparkles(void);
void BattleFx_StartTwelveFrameBlend(void);
void BattleFx_SetBlock30ValuesMaxZero(void);
void BattleFx_SetBlock30Values12Zero(void);
void BattleFx_SetBlock30Values128One(void);
void BattleFx_StartBufferBlend(s32 from, s32 to);

#endif
