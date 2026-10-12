#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT8_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT8_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"

struct BattleEffectEntry;

void BattleFx_InterpolateBuffers(s16 *source, s16 *target, s16 *step, s32 frames);
void BattlePalette_UpdateBlend(void);
void BattleFx_BuildBuffer(u32 mode, u16 *src, u16 *dst, s32 half);
void BattleEffect_InitializeBuffers(void);
void Runtime_ScheduleCallbackAndReleaseBlock32B(void);
void BattleFx_ApplyColorToTargetBuffer(s32 mode, s32 half);
void BattleFx_ApplyColorToSourceBuffer(s32 mode, s32 half);
void BattleFx_SetPrimaryBufferValue(u32 col);
void BattleFx_StartBufferInterpolation(s32 frames);
s32 BattleFx_ClampRgb555Channel(s32 value);
s32 BattleFx_ClampRgb555Component(s32 value);
void Object_EffectSpawnCallback(void);
void BattleFx_StartFadeOverlay(void *object);
void Object_EnableEffectSpawnCallback(void);
void Object_DisableEffectSpawnCallback(void);
struct BattleEffectEntry *BattleFx_FindDefinition(u32 id);
s32 BattleFx_GetAnimationValue(s32 id);
s32 BattleFx_GetResourceId(u32 id);
s32 BattleFx_GetFlags(s32 id);

#endif
