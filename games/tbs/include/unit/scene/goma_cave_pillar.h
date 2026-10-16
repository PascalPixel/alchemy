#ifndef ALCHEMY_UNIT_SCENE_GOMA_CAVE_PILLAR_H
#define ALCHEMY_UNIT_SCENE_GOMA_CAVE_PILLAR_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
extern s32 gGomaCavePillar_Actor13Frames;
u8 *SceneData_GetExtraTable(void);
s32 FieldScene_SetupPillarsOnEntry(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 *gGomaCavePillar_PillarScripts[];
extern s32 gGomaCavePillar_PillarScript0[];
extern s32 gGomaCavePillar_PillarScript1[];
extern s32 gGomaCavePillar_PillarScript2[];
extern struct SceneEntrance gGomaCavePillar_Scripts[];
extern u32 gGomaCavePillar_Messages[];
extern struct SceneEntrance gGomaCavePillar_Actors[];
extern struct SceneEvent gGomaCavePillar_Extras[];

#endif
