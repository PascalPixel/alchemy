#ifndef ALCHEMY_ACTOR_UPDATE_ARM_H
#define ALCHEMY_ACTOR_UPDATE_ARM_H

/* @file */
/* The unused ARM copy of the actors' update (actor_update_arm.c), and the */
/* arrival axes both copies watch. */

#include "types.h"

/* The axis a moving actor watches to know it has arrived at its target: the */
/* one along which it has the furthest to go. */
typedef enum ARRIVAL_AXIS {
	ARRIVAL_AXIS_NONE = 0, /* no arrival test; set again once it arrives */
	ARRIVAL_AXIS_X = 16,
	ARRIVAL_AXIS_Y = 17,
	ARRIVAL_AXIS_Z = 18
} ARRIVAL_AXIS;

void fdActorUpdateAll(void);

#endif
