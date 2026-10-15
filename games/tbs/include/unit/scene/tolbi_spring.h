#ifndef ALCHEMY_UNIT_SCENE_TOLBI_SPRING_H
#define ALCHEMY_UNIT_SCENE_TOLBI_SPRING_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8 *SceneData_GetSceneTableA(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetSceneTableB(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 TolbiSpring_OpenScene(void);
extern const struct SceneEntrance gTolbiSpring_SceneTableA[];
extern const u32 gTolbiSpring_SceneTableB[];
extern const struct ScenePlacement gTolbiSpringPlacements2[];
extern const struct ScenePlacement gTolbiSpringPlacementsOther[];
extern const struct SceneEvent gTolbiSpringEventsOther[];
extern const struct SceneEvent gTolbiSpringEvents2[];
extern const u16 gTolbiSpring_AlphaSteps[];
extern const u32 gTolbiSpring_PrizeItems[];
extern const u32 gTolbiSpring_CoinPrizes[];
extern const u32 gTolbiSpring_TopicIds[];
extern const u8 gTolbiSpring_SliderFrames[];
extern const u8 gTolbiSpring_CircleFrames[];
extern const u16 gTolbiSpring_ActorTileX[];
extern const u16 gTolbiSpring_ActorTileZ[];
extern const u16 gTolbiSpring_ActorHeadings[];

#endif
