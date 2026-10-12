#ifndef ALCHEMY_UNIT_FIELD_GOMA_HASHIRA_HASHIRA_H
#define ALCHEMY_UNIT_FIELD_GOMA_HASHIRA_HASHIRA_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
extern s32 GomaHashira_Actor13Frames;
u8 *SceneData_GetExtraTable(void);
s32 FieldScene_SetupPillarsOnEntry(void);
extern s32 StagedActor_DirectionSteps[];
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
