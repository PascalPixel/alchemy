/// @file
/// Vale's Psynergy sparkles and the main image's field Psynergy
/// (field_psynergy.c), each with its own move-target routine.
#include "types.h"
#include "field.h"
#include "fixed_math.h"

/// Sends an actor toward the point magnitude (16.16 pixels) away from it in
/// direction angle: sets its move target.
#define DEFINE_SET_TARGET_POSITION_FROM_MAGNITUDE_ANGLE(linkage,               \
                                                        setMoveTarget)         \
  linkage void Actor_SetTargetPositionFromMagnitudeAngle(                      \
      struct Actor *actor, s32 magnitude, s32 angle) {                         \
    s32 pos[3];                                                                \
                                                                               \
    if (actor != 0) {                                                          \
      SET_VECTOR(pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);         \
      Vector_AddPolarOffset(magnitude, angle, pos);                            \
      setMoveTarget(actor, pos[0], pos[1], pos[2]);                            \
    }                                                                          \
  }
