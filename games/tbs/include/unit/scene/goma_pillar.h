#ifndef ALCHEMY_UNIT_SCENE_GOMA_PILLAR_H
#define ALCHEMY_UNIT_SCENE_GOMA_PILLAR_H

#include "types.h"
#include "field_scene.h"

u8                         *SceneData_GetScriptTable(void);
s32                         SceneData_ReturnZero(void);
u8                         *SceneData_GetMessageTable(void);
u8                         *SceneData_GetActorTable(void);
extern s32                  gGomaPillar_Actor13Frames;
u8                         *SceneData_GetExtraTable(void);
s32                         FieldScene_SetupPillarsOnEntry(void);
extern s32                  gStagedActor_DirectionSteps[];
extern u32                  gStagedActor_FootprintKinds[];
extern s32                  gStagedActor_FootprintBounds[];
extern s32                 *gGomaPillar_PillarScripts[];
extern s32                  gGomaPillar_PillarScript0[];
extern s32                  gGomaPillar_PillarScript1[];
extern s32                  gGomaPillar_PillarScript2[];
extern struct SceneEntrance gGomaPillar_Scripts[];
extern u32                  gGomaPillar_Messages[];
extern struct SceneEntrance gGomaPillar_Actors[];
extern struct SceneEvent    gGomaPillar_Extras[];

#endif
