#ifndef ALCHEMY_UNIT_SCENE_TOLBI_SHIP_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_TOLBI_SHIP_INTERIORS_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;

s32 TolbiShipInteriors_SelectEntrances(void);
s32 TolbiShipInteriors_GetRegions(void);
s32 TolbiShipInteriors_GetExits(void);
s32 TolbiShipInteriors_SelectPlacements(void);
u8 *TolbiShipInteriors_SelectEvents(void);
s32 TolbiShipInteriors_ApplyEntryState(void);
extern s32 gTolbiShipInteriors_TurnSteps[];
extern s32 gTolbiShipInteriors_ActionScriptA[];
extern s32 gTolbiShipInteriors_ActionScriptB[];
extern s32 gTolbiShipInteriors_Script01[];
extern s32 gTolbiShipInteriors_ProgressTableA[];
extern s32 gTolbiShipInteriors_ActionScriptC[];
extern s32 gTolbiShipInteriors_ActionScriptD[];
extern s32 gTolbiShipInteriors_ActionScriptE[];
extern s32 gTolbiShipInteriors_EntryActionScript[];
extern s32 gTolbiShipInteriors_ProgressTableB[];
extern s32 gTolbiShipInteriors_ProgressTableC[];
extern s32 gTolbiShipInteriors_ActionScriptF[];
extern s32 gTolbiShipInteriors_StepScriptEData[];
extern struct FieldActor *gTolbiShipInteriors_AnchorObject;
extern struct SceneEntrance gTolbiShipInteriors_SceneTable01[];
extern struct SceneEntrance gTolbiShipInteriors_SceneTable02[];
extern u32 gTolbiShipInteriors_SceneTable03[];
extern struct ScenePlacement gTolbiShipInteriors_StepScriptA[];
extern struct ScenePlacement gTolbiShipInteriors_StepScriptB[];
extern struct ScenePlacement gTolbiShipInteriors_StepScriptC[];
extern struct ScenePlacement gTolbiShipInteriors_StepScriptD[];
extern struct ScenePlacement gTolbiShipInteriors_StepScriptE[];
extern struct ScenePlacement gTolbiShipInteriors_StepScriptF[];
extern struct ScenePlacement gTolbiShipInteriors_StepScriptG[];
extern struct ScenePlacement gTolbiShipInteriors_StepScriptH[];
extern struct ScenePlacement gTolbiShipInteriors_CellStepsA[];
extern struct ScenePlacement gTolbiShipInteriors_CellStepsB[];
extern struct ScenePlacement gTolbiShipInteriors_CellStepsC[];
extern struct ScenePlacement gTolbiShipInteriors_CellStepsD[];
extern struct SceneEvent gTolbiShipInteriors_SceneTable04[];
extern struct SceneEvent gTolbiShipInteriors_SceneTable05[];
extern struct SceneEvent gTolbiShipInteriors_SceneTable06[];
extern struct SceneEvent gTolbiShipInteriors_SceneTable07[];
extern struct SceneEvent gTolbiShipInteriors_SceneTable08[];
extern struct SceneEvent gTolbiShipInteriors_SceneTable09[];
extern struct SceneEvent gTolbiShipInteriors_SceneTable10[];
extern struct SceneEvent gTolbiShipInteriors_SceneTable11[];
extern struct SceneEvent gTolbiShipInteriors_SceneTable12[];
extern struct SceneEvent gTolbiShipInteriors_SceneTable13[];
extern struct SceneEvent gTolbiShipInteriors_SceneTable14[];

#endif
