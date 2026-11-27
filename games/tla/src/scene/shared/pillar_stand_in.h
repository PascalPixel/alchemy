// The stand-in actors of a room whose crumbling pillars all share one sprite's picture.
#ifndef ALCHEMY_PILLAR_STAND_IN_H
#define ALCHEMY_PILLAR_STAND_IN_H

// The first three actors of a room whose crumbling pillars all share one sprite's picture, so that
// none can change its own (Dehkan Plateau's second room): stone columns kept out of sight. The
// stand-in is drawn in a pillar's place while it cracks or gives way; the other two hold the
// cracked picture a cracked pillar copies and the whole one every pillar shares.
typedef enum CRUMBLING_PILLAR_STAND_IN
{
	ACTOR_PILLAR_STAND_IN = ACTOR_FIRST_PLACED,
	ACTOR_CRACKED_PILLAR_SPRITE,
	ACTOR_WHOLE_PILLAR_SPRITE
} CRUMBLING_PILLAR_STAND_IN;

#endif
