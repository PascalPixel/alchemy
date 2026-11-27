#ifndef ALCHEMY_SCENE_MARS_LIGHTHOUSE_AERIE_H
#define ALCHEMY_SCENE_MARS_LIGHTHOUSE_AERIE_H

/* @file */
/* Mars Lighthouse's aerie: its events and scripts, and the views of a sprite */
/* and the heap through which the beacon is lit. */

#include "fixed_math.h"
#include "types.h"

struct ACTOR;

/* A field sprite (FIELD_SPR) read a byte at a time, as the aerie's */
/* flames take the beacon fire's priority and the Mars Star's sprite its */
/* colours and the star's icon. */
typedef struct MARS_LIGHTHOUSE_AERIE_SPR {
	u8 link[4];
	u8 y; /* attribute 0's y */
	/* Attribute 0's high byte. */
	/* @{ */
	u8 affineModeMosaic : 5;
	u8 fullCol : 1;
	u8 shape : 2;
	/* @} */
	u8 attr1[2];
	u8 tileLow; /* attribute 2's low byte */
	/* Attribute 2's high byte. */
	/* @{ */
	u8 tileHigh : 2; /* tile number bits 9..8 */
	u8 pri : 2;
	u8 pal : 4;
	/* @} */
	u8 attr3[2];
	u8 scale[4];
	u8 vramBlock; /* VRAM cache entry holding its tiles */
	u8 cacheFlg;
	u8 rotation[2];
	u8 size[4]; /* width, height and the x and y offsets */
	u8 lastAnm; /* the animation number last selected */
	u8 dirty;	/* nonzero redraws its pixels */
	u8 flg;		/* the sprite flags (SPR_FLAG) */
	u8 layerCnt;
} MARS_LIGHTHOUSE_AERIE_SPR;

void scMarsLighthouseAerieTalkToKyle(void);

void scMarsLighthouseAerieFindWhoTheDragonWas(void);

void scMarsLighthouseAerieUrgeOnToBeacon(void);

void scMarsLighthouseAerieLightBeacon(void);
void scMarsLighthouseAerieWatchGoldenSunRise(void);
void scMarsLighthouseAerieEndRisingRings(struct ACTOR *actor);

void scOrbUpdateRising(struct ACTOR *self);

extern u32 Orb_RisingScript[];

extern s32 MarsLighthouseAerie_BeaconSparkScript[];

void scOrbSpawnRising(void);

extern s32 MarsLighthouseAerie_BeamWidenScript[];
extern u32 MarsLighthouseAerie_BeamShootUpScript[];
extern u32 MarsLighthouseAerie_BeamRiseScript[];
extern s32 MarsLighthouseAerie_WatchGoldenSunRiseScript[];

void scTeleportArriveWhirling(void);

extern s32 MarsLighthouseAerie_RisingRingTiles[];

void scMarsLighthouseAerieUpdateRisingRings(struct ACTOR *actor);
void scMarsLighthouseAerieHoldBackFromParents(void);
void scMarsLighthouseAerieHoldBackFromParentsAgain(void);
void scMarsLighthouseAerieOfferToCastMarsStar(void);
void scMarsLighthouseAerieClearCastOffer(void);
void scMarsLighthouseAerieTalkToIvan(void);
void scMarsLighthouseAerieTalkToJennasMother(void);
void scMarsLighthouseAerieTalkToJennasFather(void);
void scMarsLighthouseAerieHaltJennasMother(void);
void scMarsLighthouseAerieHaltJennasFather(void);

#endif
