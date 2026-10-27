#ifndef ALCHEMY_PUSHED_ACTOR_H
#define ALCHEMY_PUSHED_ACTOR_H

/// @file
/// The pushed objects' movement probes and footprint bounds.

#include "types.h"
#include "actor.h"
#include "scene_header.h"
#include "static_assert.h"

/// Where a pushed object ahead of the leader would roll to: its footprint, its
/// actor, that position, and a callback during the leader's
/// follow-through.
struct PushedActorProbe {
  /// The large thing's row in gPushedActor_FootprintKinds and
  /// gPushedActor_FootprintBounds, 0 to 5.
  s32 footprintIndex;
  s32 actorSlot; ///< the large thing's actor id, 8 to 65
  /// 16.16 pixels while moving; converted to cells by footprint redraws.
  struct FieldPosition position;
  /// Run as the leader follows the rolling thing, when not null; the probe
  /// clears it.
  void (*callback)(void);
};

/// A footprint's bounds, as gPushedActor_FootprintBounds holds four words
/// a footprint; no TBS code names this struct.
struct PushedActorFootprint {
  s32 x0; ///< left edge, pixels from the thing's position
  s32 z0; ///< top edge, pixels from the thing's position
  s32 x1; ///< right edge, pixels from the thing's position
  s32 z1; ///< bottom edge, pixels from the thing's position
};

STATIC_ASSERT(sizeof(struct PushedActorProbe) == 0x18);
STATIC_ASSERT(offsetof(struct PushedActorProbe, position.x) == 0x08);
STATIC_ASSERT(offsetof(struct PushedActorProbe, position.z) == 0x10);

STATIC_ASSERT(sizeof(struct PushedActorFootprint) == 0x10);

#endif
