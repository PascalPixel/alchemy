#ifndef ALCHEMY_PARTY_NOD_H
#define ALCHEMY_PARTY_NOD_H

/* @file */
/* The whole party nodding at once, as Kolima's and Kolima Forest's events */
/* have Isaac, Garet, Mia and Ivan agree together. */

#include "field/event.h"

/* Isaac, Garet, Mia and Ivan nod together; the event waits on Ivan's nod. */
static inline void scNodTogether(void) {
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
}

#endif
