#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_SPARKLE_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_SPARKLE_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/marker.h"

struct EfxObj;
struct FieldActor;
struct ObjectRuntime;

void BattleFx_EmitRandomParticle(void);
void BattleFx_EmitRandomParticleFromEmitter(struct ObjectRuntime *emitter);
struct EfxObj *BattleFx_StartRandomParticleEmitter(s32 actor, s32 item);
void Object_DestroyIfPresent(void *object);
void EffectRuntime_PrepareRisingObject(struct ObjectRuntime *object);
void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags);
void BattleFx_StartEffectObject22(s32 value, s32 flags);
void BattleFx_ClearRandomParticles(void);
void BattleFx_SpawnRandomParticleAtPosition(const struct FieldActor *source);
u32 EffectRuntime_IsActive(void);
void FieldEffect_SpawnNearbyMarkers(void);

#endif
