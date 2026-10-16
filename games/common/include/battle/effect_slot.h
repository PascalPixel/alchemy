#ifndef ALCHEMY_EFFECT_SLOT_H
#define ALCHEMY_EFFECT_SLOT_H

#include "edition.h"
#include "types.h"
#include "layout_guard.h"

#define EFFECT_NO_TARGET ((s32)0x80000000)

struct EffectSlot;
typedef void (*EffectCallback)(struct EffectSlot *);

struct EffectSlot {
  void *object;
  s32 x;
  s32 z;
  s32 targetX;
  s32 targetZ;
  s32 originX;
  s32 originZ;
  s32 speed;
  s32 maxSpeed;
  s32 acceleration;
  s32 scaleX;
  s32 scaleY;
  u16 heading;
  s16 maxTurnStep;
  EffectCallback callback;
  u16 age;
  s16 callbackDelay;
  s16 orbitRadius;
  s16 orbitAngle;
  s8 state;
  s8 stopAtTarget;
  s8 limitTurnStep; // Clamp steering to maxTurnStep; zero faces the target
                    // directly.
  s8 updateMotion;
  s8 render;
  s8 active;
  u8 randomValue;
  u8 flags;
};

LAYOUT_SIZE_GUARD(EffectSlot_Size, struct EffectSlot, 0x48);
LAYOUT_OFFSET_GUARD(EffectSlot_OrbitRadius, struct EffectSlot, orbitRadius,
                    0x3c);
LAYOUT_OFFSET_GUARD(EffectSlot_State, struct EffectSlot, state, 0x40);

#if GAME_TLA
u32 EffectSlot_HasPendingTarget(struct EffectSlot *);
void EffectSlot_SetPosition(struct EffectSlot *, s32, s32);
void EffectSlot_SetObjectMode(struct EffectSlot *, s32);
void EffectSlot_SetCallback(struct EffectSlot *, EffectCallback);
void EffectSlot_Update(struct EffectSlot *);
#endif

#endif
