#ifndef ALCHEMY_OBJECT_EFFECT_H
#define ALCHEMY_OBJECT_EFFECT_H

#include "types.h"
#include "edition.h"

#if GAME_TLA
extern u8 *gActorEffectWork;
#else
#include "animspr.h"

struct FieldActor;

// The fade overlay draws one mark on each side of its tracked actor.
struct FadeOverlayWork {
  struct AnimationSpritePart marks[2];
  struct FieldActor *actor;
};

STATIC_ASSERT(sizeof(struct FadeOverlayWork) == 28);

extern struct FadeOverlayWork *gActorEffectWork;
#endif

#endif
