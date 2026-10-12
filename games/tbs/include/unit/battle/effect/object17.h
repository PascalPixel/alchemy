#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT17_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT17_H

#include "types.h"
#include "effect_slot.h"

struct EffectSlot;

void EffectSlot_Update(struct EffectSlot *effect);
void BattleFx_DrawScaledObject(struct EffectSlot *object);
void EffectSlot_UpdateMotion(struct EffectSlot *effect);
u32 BattleFx_HasReachedTarget(struct EffectSlot *effect);
void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z);
void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode);
void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback);
void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z);
void BattleFx_ClearOwnedSlot(struct EffectSlot *slot);

#endif
