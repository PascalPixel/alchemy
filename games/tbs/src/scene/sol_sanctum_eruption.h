#ifndef ALCHEMY_SOL_SANCTUM_ERUPTION_H
#define ALCHEMY_SOL_SANCTUM_ERUPTION_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum CollapseActor {
  /// A blank actor whose scale SolSanctumEruption_RaiseAndShrinkActor shrinks
  /// with Alex's as he rises away (an open question in sol_sanctum_eruption.c).
  ACTOR_RISING_EFFECT = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "What's wrong, Kraden?" (MsgSolSanctumWrongKraden)
  ACTOR_KRADEN,
  /// ★ Named from the game: "But, Saturos, there's still a chance they might
  /// survive." (MsgSolSanctumKradenCouldThatBeThe)
  ACTOR_SATUROS,
  /// ★ Named from the game: "Menardi!" (MsgSolSanctumKradenCouldThatBeThe)
  ACTOR_MENARDI,
  /// Felix in his mask, before the star room unmasks him.
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  ACTOR_FELIX_MASKED,
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  ACTOR_FELIX,
  /// ★ Named from the game: "Alex ... Are you breaking our promise?"
  /// (MsgSolSanctumKradenCouldThatBeThe)
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
