#ifndef ALCHEMY_UNIT_SCENE_TUNNEL_RUINS_H
#define ALCHEMY_UNIT_SCENE_TUNNEL_RUINS_H

#include "types.h"
#include "field_scene.h"
#include "fieldcoo.h"

struct SceneEntrance;
struct SceneRegion;

typedef struct MoveTarget
{
	union FieldCoordinate x;
	s32					  pad4;
	union FieldCoordinate z;
} MoveTarget;

typedef struct Record
{
	s32 pad0;
	s32 pad4;
	s32 w8;
	s32 w12;
	s32 w16;
} Record;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
u8 *BabiChika_GetExits(void);
u8 *SceneData_SelectAndApplyTableBySceneId(void);
s32 BabiChika_SelectEvents(void);
s32 FieldScene_InitializeActorGroups(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern u32 BabiChika_SettleHeights[];
extern const s32 *gEffectScripts[];
extern s32 BabiChika_ShrinkScript[];
extern s32 BabiChika_StretchScript[];
extern s32 BabiChika_FlickerScript[];
extern u16 BabiChika_CellStepsA[];
extern u16 BabiChika_CellStepsB[];
extern struct SceneEntrance gBabiChikaEntrancesOther[];
extern struct SceneEntrance gBabiChikaEntrances1[];
extern struct SceneEntrance gBabiChikaEntrances2[];
extern s16 gBabiChikaRegions2[];
extern u32 gBabiChikaExits[];
extern struct ScenePlacement gBabiChikaPlacements1[];
extern u32 gBabiChikaPlacementsOther[];
extern struct SceneEvent gBabiChikaEvents1[];
extern struct SceneEvent gBabiChikaEventsOther[];

#endif
