#ifndef ALCHEMY_UNIT_FIELD_GOMA_HASHIRA_HASHIRA_H
#define ALCHEMY_UNIT_FIELD_GOMA_HASHIRA_HASHIRA_H

#include "types.h"
#include "field_scene.h"

struct PillarEffect;

void ConfigureActorThirteenSceneParameters(void);
void SceneEffect_AdvancePositionByAxisMode(struct PillarEffect *effect);
s32 OverlayObject_ApplyValue15(s32 actor);
void GomaHashira_SpawnPillarEffect(s32 x, s32 y, s32 z, s32 a3, s32 a4, s32 flags, s32 layer);
u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
void ConfigureSceneForActorEightColumn(void);
void FieldScene_RunPrimarySequence(void);
extern s32 GomaHashira_Actor13Frames;
void FieldScene_RunPillarSequence(void);
void FieldScene_RunPillarBurst(void);
void FieldScene_RunThreeCallSequence(void);
void FieldScene_RunFourStepSequence(void);
void FieldScene_TryJumpForward(void);
u8 *SceneData_GetExtraTable(void);
void GomaHashira_DriveActor13Idle(void);
void FieldScene_RunActor13Departure(void);
s32 FieldScene_SetupPillarsOnEntry(void);
extern s16 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 *GomaHashira_PillarScripts[];
extern s32 GomaHashira_PillarScript0[];
extern s32 GomaHashira_PillarScript1[];
extern s32 GomaHashira_PillarScript2[];
extern struct SceneEntrance GomaHashira_Scripts[];
extern u32 GomaHashira_Messages[];
extern struct SceneEntrance GomaHashira_Actors[];
extern struct SceneEvent GomaHashira_Extras[];

#endif
