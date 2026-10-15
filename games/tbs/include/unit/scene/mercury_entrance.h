#ifndef ALCHEMY_UNIT_SCENE_MERCURY_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_MERCURY_ENTRANCE_H

#include "types.h"
#include "field_scene.h"

void *SceneData_GetSceneTableA(void);
int SceneData_ReturnZero(void);
void *SceneData_GetSceneTableB(void);
void *SceneData_GetSceneTableC(void);
void *SceneData_GetSceneTableD(void);
s32 MercuryEntrance_ApplyEntryState(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 MercuryEntrance_RiseScript[];
extern s32 MercuryLighthouse_SparkScript[];
extern s32 Mercury_RampScript[];
extern s32 Mercury_ScaleCounterScript[];
extern s32 Mercury_PillarScript[];
extern u16 Mercury_MoveAngles[];
extern const s32 *gEffectScripts[];
extern s32 MercuryEntrance_Actor8Path1[];
extern s32 MercuryEntrance_Actor8Path2[];
extern s32 MercuryEntrance_Actor8Path3[];
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;
extern struct SceneEntrance MercuryEntrance_SceneTableA[];
extern struct SceneEntrance MercuryEntrance_SceneTableB[];
extern struct SceneEntrance MercuryEntrance_SceneTableC[];
extern struct SceneEvent MercuryEntrance_SceneTableD[];
extern u32 MercuryEntrance_SparkBurstScript[];

#endif
