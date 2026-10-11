#ifndef ALCHEMY_UNIT_FIELD_GOMA_HASHIRA_HASHIRA_H
#define ALCHEMY_UNIT_FIELD_GOMA_HASHIRA_HASHIRA_H

#include "types.h"

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
void FieldScene_RunPillarSequence(void);
void FieldScene_RunPillarBurst(void);
void FieldScene_RunThreeCallSequence(void);
void FieldScene_RunFourStepSequence(void);
void FieldScene_TryJumpForward(void);
u8 *SceneData_GetExtraTable(void);
void GomaHashira_DriveActor13Idle(void);
void FieldScene_RunActor13Departure(void);
s32 FieldScene_SetupPillarsOnEntry(void);

#endif
