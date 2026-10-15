#ifndef ALCHEMY_UNIT_SCENE_TOLBI_SHIP_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_TOLBI_SHIP_INTERIORS_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;

s32 TolbiShipInteriors_SelectEntrances(void);
s32 SceneData_ReturnZero(void);
s32 TolbiShipInteriors_GetExits(void);
s32 TolbiShipInteriors_GetStepScript(void);
u8 *SceneData_SelectTableBySceneIndexAndFlags(void);
s32 TolbiShipInteriors_ApplyEntryState(void);
extern s32 TolbiShipInteriors_TurnSteps[];
extern s32 TolbiShipInteriors_ActionScriptA[];
extern s32 TolbiShipInteriors_ActionScriptB[];
extern s32 TolbiShipInteriors_Script01[];
extern s32 TolbiShipInteriors_ProgressTableA[];
extern s32 TolbiShipInteriors_ActionScriptC[];
extern s32 TolbiShipInteriors_ActionScriptD[];
extern s32 TolbiShipInteriors_ActionScriptE[];
extern s32 TolbiShipInteriors_EntryActionScript[];
extern s32 TolbiShipInteriors_ProgressTableB[];
extern s32 TolbiShipInteriors_ProgressTableC[];
extern s32 TolbiShipInteriors_ActionScriptF[];
extern s32 TolbiShipInteriors_StepScriptEData[];
extern struct FieldActor *TolbiShipInteriors_AnchorObject;
extern struct SceneEntrance TolbiShipInteriors_SceneTable01[];
extern struct SceneEntrance TolbiShipInteriors_SceneTable02[];
extern u32 TolbiShipInteriors_SceneTable03[];
extern struct ScenePlacement TolbiShipInteriors_StepScriptA[];
extern struct ScenePlacement TolbiShipInteriors_StepScriptB[];
extern struct ScenePlacement TolbiShipInteriors_StepScriptC[];
extern struct ScenePlacement TolbiShipInteriors_StepScriptD[];
extern struct ScenePlacement TolbiShipInteriors_StepScriptE[];
extern struct ScenePlacement TolbiShipInteriors_StepScriptF[];
extern struct ScenePlacement TolbiShipInteriors_StepScriptG[];
extern struct ScenePlacement TolbiShipInteriors_StepScriptH[];
extern struct ScenePlacement TolbiShipInteriors_CellStepsA[];
extern struct ScenePlacement TolbiShipInteriors_CellStepsB[];
extern struct ScenePlacement TolbiShipInteriors_CellStepsC[];
extern struct ScenePlacement TolbiShipInteriors_CellStepsD[];
extern struct SceneEvent TolbiShipInteriors_SceneTable04[];
extern struct SceneEvent TolbiShipInteriors_SceneTable05[];
extern struct SceneEvent TolbiShipInteriors_SceneTable06[];
extern struct SceneEvent TolbiShipInteriors_SceneTable07[];
extern struct SceneEvent TolbiShipInteriors_SceneTable08[];
extern struct SceneEvent TolbiShipInteriors_SceneTable09[];
extern struct SceneEvent TolbiShipInteriors_SceneTable10[];
extern struct SceneEvent TolbiShipInteriors_SceneTable11[];
extern struct SceneEvent TolbiShipInteriors_SceneTable12[];
extern struct SceneEvent TolbiShipInteriors_SceneTable13[];
extern struct SceneEvent TolbiShipInteriors_SceneTable14[];

#endif
