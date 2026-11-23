#ifndef ALCHEMY_PARTY_NOD_H
#define ALCHEMY_PARTY_NOD_H

/// @file
/// The whole party nodding at once, as Kolima's and Kolima Forest's events
/// have Isaac, Garet, Mia and Ivan agree together.

#include "field/event.h"

/// Isaac, Garet, Mia and Ivan nod together; the event waits on Ivan's nod.
static inline void Scene_NodTogether(void) {
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
}

#endif
