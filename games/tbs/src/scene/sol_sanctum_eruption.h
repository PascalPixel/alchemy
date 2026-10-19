#ifndef ALCHEMY_SOL_SANCTUM_ERUPTION_H
#define ALCHEMY_SOL_SANCTUM_ERUPTION_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum CollapseActor {
  ACTOR_TELEPORT_GLOW = ACTOR_FIRST_PLACED,
  ACTOR_KRADEN,
  ACTOR_SATUROS,
  ACTOR_MENARDI,
  ACTOR_FELIX_MASKED,
  ACTOR_FELIX,
  ACTOR_ALEX
};

#include "facing_object.h"

#include "object_effect.h"

extern s32 gEmberState[16];
extern s32 gEmberTimer;
extern s32 gArcEffectTimers[10];
extern struct FieldActor *gArcEffects[];
extern s32 gEmberMask;
extern s32 gEmberLevel;
extern s32 gEmberLevelTimer;

#endif
