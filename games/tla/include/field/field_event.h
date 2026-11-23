#ifndef ALCHEMY_FIELD_EVENT_H
#define ALCHEMY_FIELD_EVENT_H

/// @file
/// Creating actors and starting their scripts and animations from a scene.

#include "fixed_math.h"
#include "types.h"
#include "field/field_sprite.h"
#include "field/field_sequence_event.h"

#include "field/actor.h"

/// struct Actor as the two scene routines that keep a flag number at byte 100
/// read it: as Actor.timer, the s16, their store of that number compiles
/// differently (Tundaria Tower's and the Mars Lighthouse entrance's
/// placement tables).
struct FieldActor {
  u8 unused[85]; ///< no code names it
  u8 motionFlags;
  u8 unusedAfterMotionFlags[3]; ///< no code names it
  u8 collisionFlags;
  u8 unusedAfterCollisionFlags[10]; ///< no code names it
  u16 timer;
};

struct Actor *Actor_Create(s32 type, FIXED x, FIXED y, FIXED z);
void Sound_PlayFar(s32 cue);

void Import_EventSetExitRequest(s32 countdown);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
/// Starts an animation on an actor's sprite (Actor_SelectAnimation, through the
/// overlay's own import row).
void Import_ActorSelectAnimation(struct Actor *actor, s32 animation);
/// Gives an actor a script and starts it from the top (Actor_StartScript,
/// through the overlay's own import row).
void Import_ActorStartScript(struct Actor *actor, const void *script);

#endif
