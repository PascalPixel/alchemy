#ifndef ALCHEMY_SOL_SANCTUM_ERUPTION_H
#define ALCHEMY_SOL_SANCTUM_ERUPTION_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum CollapseActor {
  /// The glow that grows or shrinks round an actor rising through the floor.
  /// ★ Named from what the player sees: a glow round Alex as he rises and
  /// shrinks away (SolSanctumEruption_RaiseAndShrinkActor)
  ACTOR_RISING_GLOW = ACTOR_FIRST_PLACED,
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
