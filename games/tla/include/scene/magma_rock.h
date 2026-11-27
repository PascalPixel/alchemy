#ifndef ALCHEMY_SCENE_MAGMA_ROCK_H
#define ALCHEMY_SCENE_MAGMA_ROCK_H

/* @file */
/* What Magma Rock's outside (magma_rock.c) and inside (magma_rock_inside.c) */
/* share: where a statue's fire stops, the flag set while its Burst plays, and
 */
/* the outside's tables and events. */

#include "types.h"

struct ACTOR;

/* The collision codes a statue's fire stops at as it flies */
/* (scMagmaRockFireFromStatue, scMagmaRockInsideFireFromStatue): the edge of */
/* its way, and a lava fall, which it floods the room from */
/* (scMagmaRockInsideRaiseLava). */
typedef enum MAGMA_ROCK_FIRE_STOP_CELL {
	CELL_BEHAVIOR_MAGMA_ROCK_FIRE_EDGE = 212,
	CELL_BEHAVIOR_MAGMA_ROCK_LAVA_FALL = 222
} MAGMA_ROCK_FIRE_STOP_CELL;

/* Set while a statue's fire bursts where it lands, so that Burst there does */
/* not target the statue again (scMagmaRockFireFromStatue, */
/* scMagmaRockInsideFireFromStatue). */
#define FLAG_FIRE_BURST_PLAYING FLAG_SCENE_TEMP_F0

/* A crumbling pillar of Magma Rock (scMagmaRockCrumblePillar): its actor and */
/* its flag. */
typedef struct MAGMA_ROCK_CRUMBLING_PILLAR {
	u16 id;
	u16 flg;
} MAGMA_ROCK_CRUMBLING_PILLAR;

s32 fdMapCellGetAttributes(u32 layer, s32 x, s32 z);

extern s16 MagmaRock_SlidePillars[];

extern s16 MagmaRock_SalamanderTailHighWestPillars[];

extern s16 MagmaRock_SalamanderTailHighEastPillars[];
extern s16 MagmaRock_SalamanderTailLowWestPillars[];
extern s16 MagmaRock_SalamanderTailLowEastPillars[];

void scMagmaRockMoveFireball(struct ACTOR *actor);

extern u32 MagmaRock_EmberScript[];

void scMagmaRockBurnStatue(struct ACTOR *actor);

extern s16 MagmaRock_SlideStatues[];
extern s16 MagmaRock_SalamanderTailHighWestStatues[];
extern s16 MagmaRock_SalamanderTailHighEastStatues[];
extern s16 MagmaRock_SalamanderTailLowWestStatues[];
extern s16 MagmaRock_SalamanderTailLowEastStatues[];
extern s16 MagmaRock_StatueStatues[];

void syStatueFlashUnderBurst(struct ACTOR *actor);

void scMagmaRockDropRocks(s32 x, s32 y, s32 z, s32 count);
void scMagmaRockStartFallingRocks(void);

void scMagmaRockBurstCrackedPillar(s32 mode, s32 cue);

void fdActorSetCellShape(s32 level, s32 layer, s32 id);

struct ACTOR *fdActorFindInCell(s32 *position, struct ACTOR *self);

extern s16 MagmaRock_CrumblingPillars[];

void scMagmaRockRestoreCrumblingPillars(const u16 *table);

/* Lays a tightrope between its two posts (fdTightropePlace, through the */
/* overlay's own import row). */
void scMagmaRockPlaceTightrope();

void scDustScatter(s32 spread);
extern u32 Dust_ScatterScript[];

extern u32 Dust_BurstScript[];

void scMagmaRockFireStatue(void);

void scMagmaRockIgniteStatue(void);
void scMagmaRockResetCaveDoor(void);

void scMagmaRockEnterCave(void);

void scMagmaRockResolveBeforeBurst(void);
void scMagmaRockCheckCrumblingPillars(void);

void scMagmaRockReadyHighWestStatue(void);

void scMagmaRockResetHighWestStatue(void);
void scMagmaRockKnockLeaderOffWall(void);
void scMagmaRockPushPillarEast(void);
void scMagmaRockPushPillarWest(void);

extern u32 FieldEffect_ShortScript[];
extern u32 FieldEffect_MediumScript[];
extern u32 FieldEffect_LongScript[];

#endif
