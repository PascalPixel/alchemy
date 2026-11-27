#ifndef ALCHEMY_SCENE_AIRS_ROCK_H
#define ALCHEMY_SCENE_AIRS_ROCK_H

/* What Air's Rock's outside (airs_rock.c) and inside (airs_rock_inside.c)
   share: the whirlwinds the statues breathe and the leader rides, and the
   outside's tables, scripts and scripts. */

#include "types.h"
#include "field/scene_tables.h"

/* Set while the leader rides a statue's whirlwind (scAirsRockRideWhirlwind,
   scAirsRockInsideRideWhirlwind): the statues breathe theirs without a
   flare. */
#define FLAG_RIDING_WHIRLWIND FLAG_SCENE_TEMP_50

/* How near across the ground a statue's whirlwind comes to pick up the leader,
   in pixels; on Air's Rock's outside, also how near it comes to another actor
   to end on it. */
#define WHIRLWIND_REACH 8

/* A statue breathes a whirlwind every this many frames. */
#define WHIRLWIND_BREATH_FRAMES 240

Sint32 fdMapMetatileGetAttributes(Uint32 layer, Sint32 x, Sint32 y);

extern SCENE_REGION AirsRock_StormBrandChestRegions[];
extern SCENE_REGION AirsRock_EntranceRegions[];
extern ACTOR_PLACE	AirsRock_StormBrandChestPlacements[];
extern ACTOR_PLACE	AirsRock_EntrancePlacements[];
extern ACTOR_PLACE	AirsRock_FujinShieldChestPlacements[];
extern CUE			AirsRock_StormBrandChestCues[];
extern CUE			AirsRock_EntranceCues[];
extern CUE			AirsRock_FujinShieldChestCues[];

void scAirsRockDriftFog(void);
void scAirsRockLiftFog(void);

/* The great whirlwind's sway: BG0's line scroll, by HBlank DMA. */
void scAirsRockScrollWave(void);

extern Sint16 AirsRock_EntrancePillars[];
extern Sint16 AirsRock_FrozenPuddle[];

extern Sint8 AirsRock_FogAlpha;
extern Sint8 AirsRock_FogAlphaStep;
extern Sint8 AirsRock_LiftingFogAlpha;
extern Sint8 AirsRock_LiftingFogAlphaStep;

void scAirsRockExamineWindStone(void);

void scAirsRockEnterTheRock(void);
void scAirsRockMoveFirstPillar(void);

void scAirsRockMoveThirdPillar(void);

void scAirsRockDropOffLedge(void);
void scAirsRockStartWhirlwind(void);
void scAirsRockTakeOverWhirlwind(void);

void scAirsRockBlowGust(void);

void scAirsRockWhirlwindClearsWall(void);
void scAirsRockRideWhirlwind(void);
void scAirsRockLiftPillar(void);
void scAirsRockPlacePillar(void);
void scAirsRockFreezePuddle(void);
void scAirsRockWhirlwindOpensTheTop(void);
void scAirsRockWhirlwindClearsFog(void);

extern Uint32 FieldEffect_ShortScript[];
extern Uint32 FieldEffect_MediumScript[];
extern Uint32 FieldEffect_LongScript[];
extern Sint32 AirsRock_WindStoneScript[];

void scAirsRockSetLedgeMetatilesByLevel(void);

struct ACTOR;

#endif
