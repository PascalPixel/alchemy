#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_CLEARING_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_CLEARING_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
u8 *SceneData_GetExtraTable(void);
s32 FieldScene_SetupEntryActors8To11(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern struct SceneEntrance KolimaForestClearing_Scripts[];
extern u32 KolimaForestClearing_Messages[];
extern struct ScenePlacement KolimaForestClearing_Actors[];
extern struct SceneEvent KolimaForestClearing_Extras[];

#endif
