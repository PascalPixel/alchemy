#ifndef ALCHEMY_SCENE_MAGMA_ROCK_H
#define ALCHEMY_SCENE_MAGMA_ROCK_H

// What Magma Rock's outside (magma_rock.c) and inside (magma_rock_inside.c) share: where a statue's
// fire stops, the flag set while its Burst plays, and the outside's tables and scripts.

#include "types.h"

struct ACTOR;

// The collision codes a statue's fire stops at as it flies (scMagmaRockFireFromStatue,
// scMagmaRockInsideFireFromStatue): the edge of its way, and a lava fall, which it floods the room
// from (scMagmaRockInsideRaiseLava).
typedef enum MAGMA_ROCK_FIRE_STOP_METATILE
{
	METATILE_BEHAVIOR_MAGMA_ROCK_FIRE_EDGE = 212,
	METATILE_BEHAVIOR_MAGMA_ROCK_LAVA_FALL = 222
} MAGMA_ROCK_FIRE_STOP_METATILE;

// Set while a statue's fire bursts where it lands, so that Burst there does not target the statue
// again (scMagmaRockFireFromStatue, scMagmaRockInsideFireFromStatue).
#define FLAG_FIRE_BURST_PLAYING FLAG_SCENE_TEMP_F0

// A crumbling pillar of Magma Rock (scMagmaRockCrumblePillar): its actor and its flag.
typedef struct MAGMA_ROCK_CRUMBLING_PILLAR
{
	Uint16 id;
	Uint16 flags;
} MAGMA_ROCK_CRUMBLING_PILLAR;

Sint32        fdMapMetatileGetAttributes(Uint32 layer, Sint32 x, Sint32 z);

extern Sint16 MagmaRockSlidePillars[];

extern Sint16 MagmaRockSalamanderTailHighWestPillars[];

extern Sint16 MagmaRockSalamanderTailHighEastPillars[];
extern Sint16 MagmaRockSalamanderTailLowWestPillars[];
extern Sint16 MagmaRockSalamanderTailLowEastPillars[];

void          scMagmaRockMoveFireball(struct ACTOR *actor);

extern Uint32 MagmaRockEmberScript[];

void          scMagmaRockBurnStatue(struct ACTOR *actor);

extern Sint16 MagmaRockSlideStatues[];
extern Sint16 MagmaRockSalamanderTailHighWestStatues[];
extern Sint16 MagmaRockSalamanderTailHighEastStatues[];
extern Sint16 MagmaRockSalamanderTailLowWestStatues[];
extern Sint16 MagmaRockSalamanderTailLowEastStatues[];
extern Sint16 MagmaRockStatueStatues[];

void          syStatueFlashUnderBurst(struct ACTOR *actor);

void          scMagmaRockDropRocks(Sint32 x, Sint32 y, Sint32 z, Sint32 count);
void          scMagmaRockStartFallingRocks(void);

void          scMagmaRockBurstCrackedPillar(Sint32 mode, Sint32 sound);

void          fdActorSetMetatileShape(Sint32 level, Sint32 layer, Sint32 id);

struct ACTOR *fdActorFindInMetatile(Sint32 *pos, struct ACTOR *self);

extern Sint16 MagmaRockCrumblingPillars[];

void          scMagmaRockRestoreCrumblingPillars(const Uint16 *table);

// Lays a tightrope between its two posts (fdTightropePlace, through the overlay's own import row).
void          scMagmaRockPlaceTightrope();

void          scDustScatter(Sint32 spread);
extern Uint32 DustScatterScript[];

extern Uint32 DustBurstScript[];

void          scMagmaRockFireStatue(void);

void          scMagmaRockIgniteStatue(void);
void          scMagmaRockResetCaveDoor(void);

void          scMagmaRockEnterCave(void);

void          scMagmaRockResolveBeforeBurst(void);
void          scMagmaRockCheckCrumblingPillars(void);

void          scMagmaRockReadyHighWestStatue(void);

void          scMagmaRockResetHighWestStatue(void);
void          scMagmaRockKnockLeaderOffWall(void);
void          scMagmaRockPushPillarEast(void);
void          scMagmaRockPushPillarWest(void);

extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];
extern Uint32 FieldEffectLongScript[];

#endif
