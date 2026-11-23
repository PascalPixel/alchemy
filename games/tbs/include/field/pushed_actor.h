#ifndef ALCHEMY_PUSHED_ACTOR_H
#define ALCHEMY_PUSHED_ACTOR_H

/// @file
/// The pushed actors' movement probes and rectangles.

#include "static_assert.h"
#include "types.h"
#include "field/actor.h"
#include "field/scene.h"

/// A pixel step packed as one word: X in the upper halfword, Z in the lower.
/// Packed through u32, so a negative step is its two's-complement bits.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((u32)(dz) & 0xffff))
/// A DIRECTION_STEP's X, in pixels.
#define DIRECTION_STEP_X(step) ((s32)(step) >> 16)
/// A DIRECTION_STEP's Z, in pixels.
#define DIRECTION_STEP_Z(step) ((s16)(step))
/// A DIRECTION_STEP's X as a 16.16 offset: its upper halfword, kept in place.
#define DIRECTION_STEP_X_FIXED(step) ((step) & 0xffff0000)
/// A DIRECTION_STEP's Z as a 16.16 offset: its lower halfword, shifted up.
#define DIRECTION_STEP_Z_FIXED(step) INTtoFIXED(step)

/// Where a pushed actor ahead of the leader would roll to: its rectangle, its
/// actor, that position, and a routine run during the leader's
/// follow-through.
struct PushedActorProbe {
  /// The large thing's row in gPushedActor_RectangleKinds and
  /// gPushedActor_Rectangles, 0 to 5.
  s32 rectangleIndex;
  s32 actorSlot; ///< the large thing's actor id, 8 to 65
  /// 16.16 pixels while moving; converted to cells by rectangle redraws.
  struct FieldPosition position;
  /// Run as the leader follows the rolling thing, when not null; the probe
  /// clears it.
  void (*callback)(void);
};

/// A rectangle's edges, as gPushedActor_Rectangles holds four words
/// a rectangle; no code names this struct.
struct PushedActorRectangle {
  s32 x0; ///< left edge, pixels from the thing's position
  s32 z0; ///< top edge, pixels from the thing's position
  s32 x1; ///< right edge, pixels from the thing's position
  s32 z1; ///< bottom edge, pixels from the thing's position
};

STATIC_ASSERT(sizeof(struct PushedActorProbe) == 24);
STATIC_ASSERT(offsetof(struct PushedActorProbe, position.x) == 8);
STATIC_ASSERT(offsetof(struct PushedActorProbe, position.z) == 16);

STATIC_ASSERT(sizeof(struct PushedActorRectangle) == 16);

#endif
