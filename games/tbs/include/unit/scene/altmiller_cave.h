#ifndef ALCHEMY_UNIT_SCENE_ALTMILLER_CAVE_H
#define ALCHEMY_UNIT_SCENE_ALTMILLER_CAVE_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8 *ArutamiraDou_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *ArutamiraDou_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 ArutamiraDou_ApplyEntryState(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern u16 *ArutamiraDou_ClearTarget;
extern struct SceneEntrance ArutamiraDou_SceneTableA[];
extern u32 ArutamiraDou_SceneTableB[];
extern struct ScenePlacement gArutamiraDouPlacementsOther[];
extern struct ScenePlacement gArutamiraDouPlacements2[];
extern struct ScenePlacement gArutamiraDouPlacements4[];
extern struct ScenePlacement gArutamiraDouPlacements6[];
extern u32 ArutamiraDou_PulseScales[];
extern s32 gAltmillerActionA[];
extern s32 gAltmillerActionB[];
extern s32 gAltmillerActionC[];
extern s32 gAltmillerActionD[];
extern struct SceneEvent gArutamiraDouEventsOther[];
extern struct SceneEvent gArutamiraDouEvents2[];
extern struct SceneEvent gArutamiraDouEvents3[];
extern struct SceneEvent gArutamiraDouEvents4[];
extern struct SceneEvent gArutamiraDouEvents5[];
extern struct SceneEvent gArutamiraDouEvents6[];

#endif
