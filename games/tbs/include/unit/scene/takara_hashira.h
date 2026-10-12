#ifndef ALCHEMY_UNIT_SCENE_TAKARA_HASHIRA_H
#define ALCHEMY_UNIT_SCENE_TAKARA_HASHIRA_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 TakaraHashira_SetupArea(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern const s32 *gEffectScripts[];
extern s32 TakaraHashira_ActionTable[];
extern u16 TakaraHashira_ShiftSteps1[];
extern u16 TakaraHashira_ShiftSteps2[];
extern u16 TakaraHashira_ShiftSteps3[];
extern u16 TakaraHashira_ShiftSteps4[];
extern u16 TakaraHashira_ShiftSteps5[];
extern struct SceneEntrance gTakaraHashiraEntrancesOther[];
extern struct SceneEntrance gTakaraHashiraEntrances1[];
extern struct SceneEntrance gTakaraHashiraEntrances2[];
extern struct SceneEntrance gTakaraHashiraEntrances3[];
extern struct SceneEntrance gTakaraHashiraEntrances4[];
extern struct SceneEntrance gTakaraHashiraEntrances5[];
extern u32 gTakaraHashiraExits[];
extern struct ScenePlacement gTakaraHashiraNoPlacements[];
extern struct ScenePlacement gTakaraHashiraPlacements1[];
extern struct ScenePlacement gTakaraHashiraPlacements2[];
extern struct ScenePlacement gTakaraHashiraPlacements3[];
extern struct ScenePlacement gTakaraHashiraPlacementsOther[];
extern struct ScenePlacement gTakaraHashiraPlacements5[];
extern struct SceneEvent gTakaraHashiraEventsOther[];
extern struct SceneEvent gTakaraHashiraEvents1[];
extern struct SceneEvent gTakaraHashiraEvents2[];
extern struct SceneEvent gTakaraHashiraEvents3[];
extern struct SceneEvent gTakaraHashiraEvents4[];
extern struct SceneEvent gTakaraHashiraEvents5[];

#endif
