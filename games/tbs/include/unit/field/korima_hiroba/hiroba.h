#ifndef ALCHEMY_UNIT_FIELD_KORIMA_HIROBA_HIROBA_H
#define ALCHEMY_UNIT_FIELD_KORIMA_HIROBA_HIROBA_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
void SceneActor_RunPlacementQuery(void);
void KorimaHiroba_AwardHiddenNut(void);
u8 *SceneData_GetExtraTable(void);
s32 FieldScene_SetupEntryActors8To11(void);
extern s16 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern struct SceneEntrance KorimaHiroba_Scripts[];
extern u32 KorimaHiroba_Messages[];
extern struct ScenePlacement KorimaHiroba_Actors[];
extern struct SceneEvent KorimaHiroba_Extras[];

#endif
