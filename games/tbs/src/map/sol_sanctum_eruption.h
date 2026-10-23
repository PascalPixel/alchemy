#ifndef ALCHEMY_SOL_SANCTUM_ERUPTION_H
#define ALCHEMY_SOL_SANCTUM_ERUPTION_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"

enum CollapseLocalId {
  /// The shadow on the platform under each who leaves with Saturos: a
  /// blank actor, whose sprite draws only its shadow, shrunk with the
  /// departing actor, whose own shadow is turned off
  /// (SolSanctumEruption_RaiseAndShrinkObjectEvent).
  /// ★ Named from what the player sees: the shadow that shrinks on the
  /// platform as each of them rises away (its render),
  /// https://alchemy.pascalpixel.com/places/sol-sanctum-eruption/departing-shadow.gif
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

#include "facing_object.h"

#include "object_effect.h"

extern s32 gEmberState[16];
extern s32 gEmberTimer;
extern s32 gArcEffectTimers[10];
extern struct ObjectEvent *gArcEffects[];
extern s32 gEmberMask;
extern s32 gEmberLevel;
extern s32 gEmberLevelTimer;

#endif
