#ifndef ALCHEMY_UNIT_SCENE_TUNNEL_RUINS_H
#define ALCHEMY_UNIT_SCENE_TUNNEL_RUINS_H

#include "types.h"
#include "field_scene.h"
#include "field_coordinate.h"

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
u8 *TunnelRuins_GetExits(void);
u8 *SceneData_SelectAndApplyTableBySceneId(void);
s32 TunnelRuins_SelectEvents(void);
s32 FieldScene_InitializeActorGroups(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern u32 gTunnelRuins_SettleHeights[];
extern const s32 *gEffectScripts[];
extern s32 gTunnelRuins_ShrinkScript[];
extern s32 gTunnelRuins_StretchScript[];
extern s32 gTunnelRuins_FlickerScript[];
extern u16 gTunnelRuins_CellStepsA[];
extern u16 gTunnelRuins_CellStepsB[];
extern struct SceneEntrance gTunnelRuinsEntrancesOther[];
extern struct SceneEntrance gTunnelRuinsEntrances1[];
extern struct SceneEntrance gTunnelRuinsEntrances2[];
extern s16 gTunnelRuinsRegions2[];
extern u32 gTunnelRuinsExits[];
extern struct ScenePlacement gTunnelRuinsPlacements1[];
extern u32 gTunnelRuinsPlacementsOther[];
extern struct SceneEvent gTunnelRuinsEvents1[];
extern struct SceneEvent gTunnelRuinsEventsOther[];

#endif
