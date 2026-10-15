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
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gMercuryEntrance_RiseScript[];
extern s32 gMercuryLighthouse_SparkScript[];
extern s32 gMercury_RampScript[];
extern s32 gMercury_ScaleCounterScript[];
extern s32 gMercury_PillarScript[];
extern u16 gMercury_MoveAngles[];
extern const s32 *gEffectScripts[];
extern s32 gMercuryEntrance_Actor8Path1[];
extern s32 gMercuryEntrance_Actor8Path2[];
extern s32 gMercuryEntrance_Actor8Path3[];
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;
extern struct SceneEntrance gMercuryEntrance_SceneTableA[];
extern struct SceneEntrance gMercuryEntrance_SceneTableB[];
extern struct SceneEntrance gMercuryEntrance_SceneTableC[];
extern struct SceneEvent gMercuryEntrance_SceneTableD[];
extern u32 gMercuryEntrance_SparkBurstScript[];

#endif
