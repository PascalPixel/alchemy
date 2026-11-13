#ifndef ALCHEMY_MODULE_SCENE_TOLBI_GAMES_H
#define ALCHEMY_MODULE_SCENE_TOLBI_GAMES_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
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
