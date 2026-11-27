#ifndef ALCHEMY_MERCURY_LIGHTHOUSE_PILLAR_H
#define ALCHEMY_MERCURY_LIGHTHOUSE_PILLAR_H

/* @file */
/* The events of the pool's cells in Mercury Lighthouse and its entrance */
/* (shared/mercury_lighthouse_pillar.c). */

#include "types.h"

/* The pool's heart gives the leader lights, a column of light over him. */
void scMercuryLighthouseChargeLights(void);

/* A plain stepping square takes the leader's lights away. */
void scMercuryLighthouseLoseLights(void);
/* Pushing against the pool's edge hops the leader two cells. */
void scMercuryLighthouseHopAcrossPool(void);
/* Out in the water, ripples spread from the leader. */
void scMercuryLighthouseRippleAroundLeader(void);

#endif
