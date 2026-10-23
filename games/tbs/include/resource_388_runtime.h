#ifndef ALCHEMY_RESOURCE_388_RUNTIME_H
#define ALCHEMY_RESOURCE_388_RUNTIME_H

/// @file
/// The views a map overlay takes of its runtime work and slots; most fields
/// are still unnamed.

#include "heap_slots.h"
#include "static_assert.h"

/// A view of a work block that nothing in TBS reads; Goma Cave's pond map
/// includes the header. XXX: likely struct FieldWork, because 0x1c0 and 0x1c8
/// are its transition and transitionFrames.
struct Resource388Runtime {
  u8 beforeTransition[0x1c0]; ///< FieldWork's fields before transition
  s32 setupRequest1c0;        ///< at FieldWork.transition's offset
  /// FieldWork's spare halfword and transitionActive.
  u8 beforeTransitionFrames[4];
  s32 setupValue1c8; ///< at FieldWork.transitionFrames's offset
};

/// A view of an object that nothing in TBS reads. XXX: likely struct
/// Object, because 0x23 and 0x59 are its priority and collision flags.
struct Resource388SlotView {
  u8 beforePriority[0x23]; ///< Object's fields before priorityFlags
  s8 priorityFlags;        ///< at Object.priorityFlags's offset
  /// Object's fields from velocityX to snapToTarget.
  u8 beforeCollision[0x35];
  s8 collisionFlags; ///< at Object.collisionFlags's offset
};

STATIC_ASSERT(offsetof(struct Resource388Runtime, setupRequest1c0) == 0x1c0);
STATIC_ASSERT(offsetof(struct Resource388Runtime, setupValue1c8) == 0x1c8);
STATIC_ASSERT(offsetof(struct Resource388SlotView, priorityFlags) == 0x23);
STATIC_ASSERT(offsetof(struct Resource388SlotView, collisionFlags) == 0x59);

#endif
