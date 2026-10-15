#ifndef ALCHEMY_UNIT_SCENE_IMIL_H
#define ALCHEMY_UNIT_SCENE_IMIL_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

typedef struct
{
	u8	filler0[8];
	s32 unk8;
	s32 unkC;
	s32 unk10;
	u8	filler14[80];
	u16 unk64;
	s16 unk66;
} CountdownArcObject;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *ImiruMura_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 ImiruMura_ApplyEntryState(void);
extern s16 ImiruMura_KeyHeadings[];
extern s16 ImiruMura_SwayHeadings[];
extern s32 gImiruMuraPlacementData[];
extern s32 ImiruMura_ActorScriptA[];
extern s32 ImiruMura_ActorScriptB[];
extern s32 ImiruMura_ActorScriptC[];
extern s32 ImiruMura_TurnScript[];
extern s32 ImiruMura_SparkScript[];
extern s32 ImiruMura_MiaScriptA[];
extern s32 ImiruMura_MiaScriptB[];
extern s32 ImiruMura_PrimaryScript[];
extern s32 ImiruMura_PrimaryScript2[];
extern struct SceneEntrance gImiruMuraEntrancesOther[];
extern struct SceneEntrance gImiruMuraEntrances2[];
extern u32 ImiruMura_SceneTable[];
extern struct ScenePlacement gImiruMuraPlacements[];
extern struct ScenePlacement gImiruMuraPlacementsFlag881[];
extern struct ScenePlacement gImiruMuraPlacements2[];
extern struct SceneEvent gImiruMuraEventsOther[];
extern struct SceneEvent gImiruMuraEvents2[];
extern u16 ImiruMura_ExitCellStep5[];
extern u16 ImiruMura_ExitCellStep4[];
extern u16 ImiruMura_ExitCellStep1[];
extern u16 ImiruMura_ExitCellStep0[];
extern u16 ImiruMura_ExitCellStep2[];
extern u16 ImiruMura_ExitCellStep3[];
extern u16 *ImiruMura_ExitCellSteps[];
extern s16 ImiruMura_ExitCellPoints[][2];
extern u16 ImiruMura_CellStepsA[];
extern u16 ImiruMura_CellStepsB[];

#endif
