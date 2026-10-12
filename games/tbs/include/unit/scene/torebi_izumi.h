#ifndef ALCHEMY_UNIT_SCENE_TOREBI_IZUMI_H
#define ALCHEMY_UNIT_SCENE_TOREBI_IZUMI_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8 *SceneData_GetSceneTableA(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetSceneTableB(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 TorebiIzumi_OpenScene(void);
extern const struct SceneEntrance TorebiIzumi_SceneTableA[];
extern const u32 TorebiIzumi_SceneTableB[];
extern const struct ScenePlacement gTorebiIzumiPlacements2[];
extern const struct ScenePlacement gTorebiIzumiPlacementsOther[];
extern const struct SceneEvent gTorebiIzumiEventsOther[];
extern const struct SceneEvent gTorebiIzumiEvents2[];
extern const u16 TorebiIzumi_AlphaSteps[];
extern const u32 Data_02001fd0[];
extern const u32 Data_0200200c[];
extern const u32 TorebiIzumi_TopicIds[];
extern const u8 TorebiIzumi_SliderFrames[];
extern const u8 TorebiIzumi_CircleFrames[];
extern const u16 TorebiIzumi_ActorTileX[];
extern const u16 TorebiIzumi_ActorTileZ[];
extern const u16 TorebiIzumi_ActorHeadings[];

#endif
