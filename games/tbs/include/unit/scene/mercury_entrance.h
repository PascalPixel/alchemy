#ifndef ALCHEMY_UNIT_SCENE_MERCURY_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_MERCURY_ENTRANCE_H

#include "types.h"
#include "field_scene.h"

void *SceneData_GetSceneTableA(void);
int SceneData_ReturnZero(void);
void *SceneData_GetSceneTableB(void);
void *SceneData_GetSceneTableC(void);
void *SceneData_GetSceneTableD(void);
s32 MakyuriIriguchi_ApplyEntryState(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 MakyuriIriguchi_RiseScript[];
extern s32 MakyuriHeya_SparkScript[];
extern s32 Makyuri_RampScript[];
extern s32 Makyuri_ScaleCounterScript[];
extern s32 Makyuri_PillarScript[];
extern u16 Makyuri_MoveAngles[];
extern const s32 *gEffectScripts[];
extern s32 MakyuriIriguchi_Actor8Path1[];
extern s32 MakyuriIriguchi_Actor8Path2[];
extern s32 MakyuriIriguchi_Actor8Path3[];
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;
extern struct SceneEntrance MakyuriIriguchi_SceneTableA[];
extern struct SceneEntrance MakyuriIriguchi_SceneTableB[];
extern struct SceneEntrance MakyuriIriguchi_SceneTableC[];
extern struct SceneEvent MakyuriIriguchi_SceneTableD[];
extern u32 MakyuriIriguchi_SparkBurstScript[];

#endif
