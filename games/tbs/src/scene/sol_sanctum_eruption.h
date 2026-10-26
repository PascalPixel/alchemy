#ifndef ALCHEMY_SOL_SANCTUM_ERUPTION_H
#define ALCHEMY_SOL_SANCTUM_ERUPTION_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"

enum CollapseActorId {
  /// The shadow on the platform under each who leaves with Saturos: a
  /// blank actor, whose sprite draws only its shadow, shrunk with
  /// the departing actor, whose own shadow is turned off
  /// (SolSanctumEruption_RaiseAndShrinkActor).
  /// ★ Named from what the player sees: the shadow that shrinks on the
  /// platform as each of them rises away (its render)
  ACTOR_PLATFORM_SHADOW = ACTOR_FIRST_PLACED,
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

#include "object_effect.h"

extern s32 gEmberState[16];
extern s32 gEmberTimer;
extern s32 gArcEffectTimers[10];
extern struct Actor *gArcEffects[];
extern s32 gEmberMask;
extern s32 gEmberLevel;
extern s32 gEmberLevelTimer;

#endif
