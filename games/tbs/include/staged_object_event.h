#ifndef ALCHEMY_STAGED_ACTOR_H
#define ALCHEMY_STAGED_ACTOR_H

/// @file
/// The pushable actors' movement probes and footprint bounds.

#include "types.h"
#include "object_event.h"
#include "field_runtime.h"
#include "static_assert.h"

/// Where a large thing ahead of the leader would roll to: its footprint, its
/// actor, that position, and a callback during the leader's follow-through.
struct StagedActorProbe {
  /// The large thing's row in gStagedActor_FootprintKinds and
  /// gStagedActor_FootprintBounds, 0 to 5.
  s32 footprintIndex;
  s32 actorSlot; ///< the large thing's actor number, 8 to 65
  /// 16.16 pixels while moving; converted to cells by footprint redraws.
  struct FieldPosition position;
  /// Run as the leader follows the rolling thing, when not null; the probe
  /// clears it.
  void (*callback)(void);
};

/// A footprint's bounds, as gStagedActor_FootprintBounds holds four words a
/// footprint; no TBS code names this struct.
struct StagedActorFootprint {
  s32 x0; ///< left edge, pixels from the thing's position
  s32 z0; ///< top edge, pixels from the thing's position
  s32 x1; ///< right edge, pixels from the thing's position
  s32 z1; ///< bottom edge, pixels from the thing's position
};

STATIC_ASSERT(sizeof(struct StagedActorProbe) == 0x18);
STATIC_ASSERT(offsetof(struct StagedActorProbe, position.x) == 0x08);
STATIC_ASSERT(offsetof(struct StagedActorProbe, position.z) == 0x10);

STATIC_ASSERT(sizeof(struct StagedActorFootprint) == 0x10);

#endif
