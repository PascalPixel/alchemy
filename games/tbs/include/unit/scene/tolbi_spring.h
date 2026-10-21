#ifndef ALCHEMY_UNIT_SCENE_TOLBI_SPRING_H
#define ALCHEMY_UNIT_SCENE_TOLBI_SPRING_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *TolbiSpring_GetEntrances(void);
const struct SceneRegion *TolbiSpring_GetRegions(void);
const u32 *TolbiSpring_GetExits(void);
const struct ScenePlacement *TolbiSpring_GetPlacements(void);
const struct SceneEvent *TolbiSpring_GetEvents(void);
s32 TolbiSpring_Initialize(void);
extern const struct SceneEntrance gTolbiSpring_Entrances[];
extern const u32 gTolbiSpring_Exits[];
extern const struct ScenePlacement gTolbiSpringPlacements[];
extern const struct ScenePlacement gTolbiSpringPlacementsOther[];
extern const struct SceneEvent gTolbiSpringEventsOther[];
extern const struct SceneEvent gTolbiSpringEvents[];
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
