#ifndef ALCHEMY_UNIT_SCENE_FUNE_HEYA_H
#define ALCHEMY_UNIT_SCENE_FUNE_HEYA_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;

s32 FuneHeya_SelectEntrances(void);
s32 SceneData_ReturnZero(void);
s32 FuneHeya_GetExits(void);
s32 FuneHeya_GetStepScript(void);
u8 *SceneData_SelectTableBySceneIndexAndFlags(void);
s32 FuneHeya_ApplyEntryState(void);
extern s32 FuneHeya_TurnSteps[];
extern s32 FuneHeya_ActionScriptA[];
extern s32 FuneHeya_ActionScriptB[];
extern s32 FuneHeya_Script01[];
extern s32 FuneHeya_ProgressTableA[];
extern s32 FuneHeya_ActionScriptC[];
extern s32 FuneHeya_ActionScriptD[];
extern s32 FuneHeya_ActionScriptE[];
extern s32 FuneHeya_EntryActionScript[];
extern s32 FuneHeya_ProgressTableB[];
extern s32 FuneHeya_ProgressTableC[];
extern s32 FuneHeya_ActionScriptF[];
extern s32 FuneHeya_StepScriptEData[];
extern struct FieldActor *FuneHeya_AnchorObject;
extern struct SceneEntrance FuneHeya_SceneTable01[];
extern struct SceneEntrance FuneHeya_SceneTable02[];
extern u32 FuneHeya_SceneTable03[];
extern struct ScenePlacement FuneHeya_StepScriptA[];
extern struct ScenePlacement FuneHeya_StepScriptB[];
extern struct ScenePlacement FuneHeya_StepScriptC[];
extern struct ScenePlacement FuneHeya_StepScriptD[];
extern struct ScenePlacement FuneHeya_StepScriptE[];
extern struct ScenePlacement FuneHeya_StepScriptF[];
extern struct ScenePlacement FuneHeya_StepScriptG[];
extern struct ScenePlacement FuneHeya_StepScriptH[];
extern struct ScenePlacement FuneHeya_CellStepsA[];
extern struct ScenePlacement FuneHeya_CellStepsB[];
extern struct ScenePlacement FuneHeya_CellStepsC[];
extern struct ScenePlacement FuneHeya_CellStepsD[];
extern struct SceneEvent FuneHeya_SceneTable04[];
extern struct SceneEvent FuneHeya_SceneTable05[];
extern struct SceneEvent FuneHeya_SceneTable06[];
extern struct SceneEvent FuneHeya_SceneTable07[];
extern struct SceneEvent FuneHeya_SceneTable08[];
extern struct SceneEvent FuneHeya_SceneTable09[];
extern struct SceneEvent FuneHeya_SceneTable10[];
extern struct SceneEvent FuneHeya_SceneTable11[];
extern struct SceneEvent FuneHeya_SceneTable12[];
extern struct SceneEvent FuneHeya_SceneTable13[];
extern struct SceneEvent FuneHeya_SceneTable14[];

#endif
