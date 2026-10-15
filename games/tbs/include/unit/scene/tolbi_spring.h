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
extern const struct SceneEntrance TolbiSpring_SceneTableA[];
extern const u32 TolbiSpring_SceneTableB[];
extern const struct ScenePlacement gTolbiSpringPlacements2[];
extern const struct ScenePlacement gTolbiSpringPlacementsOther[];
extern const struct SceneEvent gTolbiSpringEventsOther[];
extern const struct SceneEvent gTolbiSpringEvents2[];
extern const u16 TolbiSpring_AlphaSteps[];
extern const u32 TolbiSpring_PrizeItems[];
extern const u32 TolbiSpring_CoinPrizes[];
extern const u32 TolbiSpring_TopicIds[];
extern const u8 TolbiSpring_SliderFrames[];
extern const u8 TolbiSpring_CircleFrames[];
extern const u16 TolbiSpring_ActorTileX[];
extern const u16 TolbiSpring_ActorTileZ[];
extern const u16 TolbiSpring_ActorHeadings[];

#endif
