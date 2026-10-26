#ifndef ALCHEMY_SOL_SANCTUM_ERUPTION_H
#define ALCHEMY_SOL_SANCTUM_ERUPTION_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"

enum CollapseLocalId {
  /// The shadow on the platform under each who leaves with Saturos: a
  /// blank object event, whose sprite draws only its shadow, shrunk with
  /// the departing object event, whose own shadow is turned off
  /// (SolSanctumEruption_RaiseAndShrinkObjectEvent).
  /// ★ Named from what the player sees: the shadow that shrinks on the
  /// platform as each of them rises away (its render)
  LOCALID_PLATFORM_SHADOW = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "What's wrong, Kraden?" (MsgSolSanctumWrongKraden)
  LOCALID_KRADEN,
  /// ★ Named from the game: "But, Saturos, there's still a chance they might
  /// survive." (MsgSolSanctumKradenCouldThatBeThe)
  LOCALID_SATUROS,
  /// ★ Named from the game: "Menardi!" (MsgSolSanctumKradenCouldThatBeThe)
  LOCALID_MENARDI,
  /// Felix in his mask, before the star room unmasks him.
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  LOCALID_FELIX_MASKED,
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  LOCALID_FELIX,
  /// ★ Named from the game: "Alex ... Are you breaking our promise?"
  /// (MsgSolSanctumKradenCouldThatBeThe)
  LOCALID_ALEX
};

#include "object_effect.h"

extern s32 gEmberState[16];
extern s32 gEmberTimer;
extern s32 gArcEffectTimers[10];
extern struct ObjectEvent *gArcEffects[];
extern s32 gEmberMask;
extern s32 gEmberLevel;
extern s32 gEmberLevelTimer;

#endif
