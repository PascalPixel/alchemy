#ifndef ALCHEMY_UNIT_SCENE_TUNNEL_RUINS_H
#define ALCHEMY_UNIT_SCENE_TUNNEL_RUINS_H

#include "types.h"
#include "field_scene.h"
#include "field_coordinate.h"
#include "field_runtime.h"

struct SceneEntrance;
struct SceneRegion;

const struct SceneEntrance *TunnelRuins_GetEntrances(void);
const struct SceneRegion *TunnelRuins_GetRegions(void);
u32 *TunnelRuins_GetExits(void);
struct ScenePlacement *TunnelRuins_GetPlacements(void);
const struct SceneEvent *TunnelRuins_GetEvents(void);
s32 TunnelRuins_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern u32 gTunnelRuins_BalanceHeights[];
extern const s32 *gEffectScripts[];
extern s32 gTunnelRuins_ShrinkScript[];
extern s32 gTunnelRuins_StretchScript[];
extern s32 gTunnelRuins_DouseSplashScript[];
extern u16 gTunnelRuins_DoorOpenSteps[];
extern u16 gTunnelRuins_DoorCloseSteps[];
extern struct SceneEntrance gTunnelRuinsEntrancesOther[];
extern struct SceneEntrance gTunnelRuinsEntrances1[];
extern struct SceneEntrance gTunnelRuinsEntrances2[];
extern struct SceneRegion gTunnelRuinsRegions2[];
extern u32 gTunnelRuinsExits[];
extern struct ScenePlacement gTunnelRuinsPlacements1[];
extern struct ScenePlacement gTunnelRuinsPlacementsOther[];
extern struct SceneEvent gTunnelRuinsEvents1[];
extern struct SceneEvent gTunnelRuinsEventsOther[];

#endif
