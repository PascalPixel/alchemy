#ifndef ALCHEMY_MERCURY_LIGHTHOUSE_PILLAR_H
#define ALCHEMY_MERCURY_LIGHTHOUSE_PILLAR_H

// The scripts of the pool's metatiles in Mercury Lighthouse and its entrance
// (shared/mercury_lighthouse_pillar.c).

#include "types.h"

// The pool's heart gives the leader lights, a column of light over him.
void          scMercuryLighthouseChargeLights(void);

// A plain stepping square takes the leader's lights away.
void          scMercuryLighthouseLoseLights(void);
// Pushing against the pool's edge hops the leader two metatiles.
void          scMercuryLighthouseHopAcrossPool(void);
// Out in the water, ripples spread from the leader.
void          scMercuryLighthouseRippleAroundLeader(void);

// The scripts the pool's lights and sparks run, which each scene defines or includes
// (shared/mercury_lighthouse_pillar_tables.c), and the d-pad's headings.
extern Sint32 MercuryLighthouseSparkScript[];
extern Sint32 MercuryLighthouseGrowLightScript[];
extern Sint32 MercuryLighthouseLightOutScript[];
extern Sint32 MercuryLighthousePillarScript[];
extern Uint16 MercuryLighthouseMoveAngles[];

#endif
