#ifndef ALCHEMY_WALK_INTO_LEADER_H
#define ALCHEMY_WALK_INTO_LEADER_H

/* @file */
/* A companion and the party leader: placed on the leader, walked toward the */
/* leader, or walked into the leader and taken off the map, as the scenes' */
/* events gather the party back into one walking sprite. */

#include "field/event.h"

/* Puts an actor where the leader stands, if there is a leader. */
static inline void scPlaceAtLeader(s32 actorId) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	if(leader != NULL) {
		fdActorSetPosFX(actorId, leader->x.fixed, leader->z.fixed);
	}
}

/* Sends an actor walking to where the leader stands, if there is a leader. */
static inline void scWalkTowardLeader(s32 actorId) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	if(leader != NULL) {
		fdActorSetDestination(actorId, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	}
}

/* An actor walks into the leader and is taken off the map, as the party */
/* closes up into one walking sprite at the end of an event. */
static inline void scWalkIntoLeader(s32 actorId) {
	ACTOR *leader;

	fdActorSetAnimation(actorId, ANIM_WALK);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetDestination(actorId, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	}
	fdActorWaitForMoveById(actorId);
	fdActorSetPos(actorId, 0, 0);
}

#endif
