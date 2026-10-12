#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_MARKER_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_MARKER_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"

struct ObjectRuntime;

void ObjectMotion_SnapToTerrain(void *object);
void Battle_PlaceMapMarkers(void);
s32 EffectRuntime_FindSlotByObject(s32 id);
void EffectRuntime_ReservedNoOp(void);
void EffectRuntime_SetMode5AndPlayCue(s32 id);
void EffectRuntime_SetMode7AndLaunch(s32 id);
void EffectRuntime_SetMode4AndPlayCue(s32 id);
void EffectRuntime_SetMode2(s32 id);
struct ObjectRuntime *EffectRuntime_GetCurrentObject(s32 id);
void EffectRuntime_ClearCurrentFlags(s32 id);
void EffectRuntime_SetCurrentPosition(s32 id, s32 x, s32 y);

#endif
