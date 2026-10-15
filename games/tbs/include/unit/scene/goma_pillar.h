#ifndef ALCHEMY_UNIT_SCENE_GOMA_PILLAR_H
#define ALCHEMY_UNIT_SCENE_GOMA_PILLAR_H

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
extern s32 *GomaPillar_PillarScripts[];
extern s32 GomaPillar_PillarScript0[];
extern s32 GomaPillar_PillarScript1[];
extern s32 GomaPillar_PillarScript2[];
extern struct SceneEntrance GomaPillar_Scripts[];
extern u32 GomaPillar_Messages[];
extern struct SceneEntrance GomaPillar_Actors[];
extern struct SceneEvent GomaPillar_Extras[];

#endif
