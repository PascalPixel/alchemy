#ifndef ALCHEMY_UNIT_FIELD_MAKYURI_IRIGUCHI_IRIGUCHI_H
#define ALCHEMY_UNIT_FIELD_MAKYURI_IRIGUCHI_IRIGUCHI_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 type);
s32 MakyuriIriguchi_TrailSparks(struct FieldActor *a0);
s32 SceneAudio_PlayCue118AndReturnZero(void);
s32 SceneActor_FaceLeaderWhileGrounded(struct FieldActor *object);
void MakyuriIriguchi_TickSpawnTimer(void);
void MakyuriIriguchi_ClearCurrentRecordAndReleaseTarget(void);
void FieldScene_RunSingleStep(void);
void MakyuriIriguchi_TriggerPillarRise(void);
void *SceneData_GetSceneTableA(void);
int SceneData_ReturnZero(void);
void *SceneData_GetSceneTableB(void);
void *SceneData_GetSceneTableC(void);
void FieldScene_RunStepWithValue1632(void);
void SceneActor_RunActorZeroHandledMotion(s32 a);
void MakyuriIriguchi_DropLeaderToColumn(s32 a0);
void FieldScene_RunIndexedStep17(void);
void FieldScene_RunIndexedStep18(void);
void FieldScene_RunIndexedStep19(void);
void *SceneData_GetSceneTableD(void);
void FieldScene_RunSupplementalSequenceTwo(void);
void FieldScene_RunIndexedStep63(void);
void MakyuriIriguchi_InstallActorEightPath(void);
void MakyuriIriguchi_SendActor8ByLeaderColumn(void);
void MakyuriIriguchi_OpenEntrance(void);
void MakyuriIriguchi_ArriveWithSparks(void);
s32 MakyuriIriguchi_ApplyEntryState(void);
void MakyuriIriguchi_CrossDoorway(void);
void MakyuriIriguchi_RunDoorScene(void);
void OverlayObject_SpawnKind24AtObject(struct FieldActor *src);
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
