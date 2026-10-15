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
u8 *Imil_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Imil_ApplyEntryState(void);
extern s16 Imil_KeyHeadings[];
extern s16 Imil_SwayHeadings[];
extern s32 gImilPlacementData[];
extern s32 Imil_ActorScriptA[];
extern s32 Imil_ActorScriptB[];
extern s32 Imil_ActorScriptC[];
extern s32 Imil_TurnScript[];
extern s32 Imil_SparkScript[];
extern s32 Imil_MiaScriptA[];
extern s32 Imil_MiaScriptB[];
extern s32 Imil_PrimaryScript[];
extern s32 Imil_PrimaryScript2[];
extern struct SceneEntrance gImilEntrancesOther[];
extern struct SceneEntrance gImilEntrances2[];
extern u32 Imil_SceneTable[];
extern struct ScenePlacement gImilPlacements[];
extern struct ScenePlacement gImilPlacementsFlag881[];
extern struct ScenePlacement gImilPlacements2[];
extern struct SceneEvent gImilEventsOther[];
extern struct SceneEvent gImilEvents2[];
extern u16 Imil_ExitCellStep5[];
extern u16 Imil_ExitCellStep4[];
extern u16 Imil_ExitCellStep1[];
extern u16 Imil_ExitCellStep0[];
extern u16 Imil_ExitCellStep2[];
extern u16 Imil_ExitCellStep3[];
extern u16 *Imil_ExitCellSteps[];
extern s16 Imil_ExitCellPoints[][2];
extern u16 Imil_CellStepsA[];
extern u16 Imil_CellStepsB[];

#endif
