#ifndef ALCHEMY_UNIT_MAP_TOLBI_GAMES_H
#define ALCHEMY_UNIT_MAP_TOLBI_GAMES_H

#include "types.h"
#include "scene_tables.h"

struct Trigger;
struct ActorPlacement;

const struct SceneEntrance *TolbiSpring_GetEntrances(void);
const struct SceneExitRegion *TolbiSpring_GetRegions(void);
const u32 *TolbiSpring_GetExits(void);
const struct ActorPlacement *TolbiSpring_GetActorPlacements(void);
const struct Trigger *TolbiSpring_GetTriggers(void);
s32 TolbiSpring_Initialize(void);
extern const struct SceneEntrance gTolbiSpring_Entrances[];
extern const u32 gTolbiSpring_Exits[];
extern const struct ActorPlacement gTolbiSpringActorPlacements[];
extern const struct ActorPlacement gTolbiSpringActorPlacementsOther[];
extern const struct Trigger gTolbiSpringTriggersOther[];
extern const struct Trigger gTolbiSpringTriggers[];
extern const u16 gTolbiSpring_AlphaSteps[];
extern const u32 gTolbiSpring_PrizeItems[];
extern const u16 gTolbiSpring_CoinPrizes[];
extern const u32 gTolbiSpring_LuckyWheelsPrizeItems[];
extern const u8 gTolbiSpring_SliderFrames[];
extern const u8 gTolbiSpring_CircleFrames[];
extern const u8 gTolbiSpring_ActorPixelX[];
extern const u8 gTolbiSpring_ActorPixelZ[];
extern const u16 gTolbiSpring_ActorHeadings[];

#endif
