#ifndef ALCHEMY_SOL_SANCTUM_ERUPTION_H
#define ALCHEMY_SOL_SANCTUM_ERUPTION_H

#include "types.h"
#include "event.h"
#include "scene_tables.h"

enum CollapseActorId {
  /// The shadow on the platform under each who leaves with Saturos: a
  /// blank actor, whose sprite draws only its shadow, shrunk with
  /// the departing actor, whose own shadow is turned off
  /// (SolSanctumEruption_RaiseAndShrinkActor).
  ACTOR_PLATFORM_SHADOW = ACTOR_FIRST_PLACED,
  ACTOR_KRADEN,
  ACTOR_SATUROS,
  ACTOR_MENARDI,
  /// Felix in his mask, before the star room unmasks him.
  ACTOR_FELIX_MASKED,
  ACTOR_FELIX,
  ACTOR_ALEX
};

#include "ground_mask.h"

extern s32 gEmberState[16];
extern s32 gEmberTimer;
extern s32 gArcEffectTimers[10];
extern struct Actor *gArcEffects[];
extern s32 gEmberMask;
extern s32 gEmberLevel;
extern s32 gEmberLevelTimer;

#endif
