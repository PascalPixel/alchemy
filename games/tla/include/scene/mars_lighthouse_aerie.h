#ifndef ALCHEMY_SCENE_MARS_LIGHTHOUSE_AERIE_H
#define ALCHEMY_SCENE_MARS_LIGHTHOUSE_AERIE_H

// Mars Lighthouse's aerie: its scripts and scripts, and the views of a sprite and the heap through
// which the beacon is lit.

#include "fixed_math.h"
#include "types.h"

struct ACTOR;

// A field sprite (FIELD_SPR) read a byte at a time, as the aerie's flames take the beacon fire's
// priority and the Mars Star's sprite its colours and the star's icon.
typedef struct MARS_LIGHTHOUSE_AERIE_SPR
{
	Uint8 link[4];
	Uint8 y; // attribute 0's y
	// Attribute 0's high byte.
	Uint8 affineModeMosaic : 5;
	Uint8 fullColor        : 1;
	Uint8 shape            : 2;
	Uint8 attr1[2];
	Uint8 tileLow; // attribute 2's low byte
	// Attribute 2's high byte.
	Uint8 tileHigh : 2; // tile number bits 9..8
	Uint8 priority : 2;
	Uint8 palette  : 4;
	Uint8 attr3[2];
	Uint8 scale[4];
	Uint8 vramBlock;     // VRAM cache entry holding its tiles
	Uint8 cacheFlags;
	Uint8 rotation[2];
	Uint8 size[4];       // width, height and the x and y offsets
	Uint8 lastAnimation; // the animation number last selected
	Uint8 dirty;         // nonzero redraws its pixels
	Uint8 flags;         // the sprite flags (SPR_FLAG)
	Uint8 layerCount;
} MARS_LIGHTHOUSE_AERIE_SPR;

void          scMarsLighthouseAerieTalkToKyle(void);

void          scMarsLighthouseAerieFindWhoTheDragonWas(void);

void          scMarsLighthouseAerieUrgeOnToBeacon(void);

void          scMarsLighthouseAerieLightBeacon(void);
void          scMarsLighthouseAerieWatchGoldenSunRise(void);
void          scMarsLighthouseAerieEndRisingRings(struct ACTOR *actor);

void          scOrbUpdateRising(struct ACTOR *self);

extern Uint32 OrbRisingScript[];

extern Sint32 MarsLighthouseAerieBeaconSparkScript[];

void          scOrbSpawnRising(void);

extern Sint32 MarsLighthouseAerieBeamWidenScript[];
extern Uint32 MarsLighthouseAerieBeamShootUpScript[];
extern Uint32 MarsLighthouseAerieBeamRiseScript[];
extern Sint32 MarsLighthouseAerieWatchGoldenSunRiseScript[];

void          scTeleportArriveWhirling(void);

extern Sint32 MarsLighthouseAerieRisingRingTiles[];

void          scMarsLighthouseAerieUpdateRisingRings(struct ACTOR *actor);
void          scMarsLighthouseAerieHoldBackFromParents(void);
void          scMarsLighthouseAerieHoldBackFromParentsAgain(void);
void          scMarsLighthouseAerieOfferToCastMarsStar(void);
void          scMarsLighthouseAerieClearCastOffer(void);
void          scMarsLighthouseAerieTalkToIvan(void);
void          scMarsLighthouseAerieTalkToJennasMother(void);
void          scMarsLighthouseAerieTalkToJennasFather(void);
void          scMarsLighthouseAerieHaltJennasMother(void);
void          scMarsLighthouseAerieHaltJennasFather(void);

#endif
