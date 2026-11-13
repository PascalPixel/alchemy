#ifndef ALCHEMY_MODULE_SCENE_TUNNEL_RUINS_H
#define ALCHEMY_MODULE_SCENE_TUNNEL_RUINS_H

#include "types.h"
#include "scene_tables.h"
#include "field_coordinate.h"
#include "scene.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern u32 gTunnelRuins_BalanceHeights[];
extern s32 gTunnelRuins_ShrinkScript[];
extern s32 gTunnelRuins_StretchScript[];
extern s32 gTunnelRuins_DouseSplashScript[];
extern u16 gTunnelRuins_DoorOpenSteps[];
extern u16 gTunnelRuins_DoorCloseSteps[];
extern struct SceneEntrance gTunnelRuinsEntrancesOther[];
extern struct SceneEntrance gTunnelRuinsGreatStatueEntrances[];
extern struct SceneEntrance gTunnelRuinsLadyStatuesEntrances[];
extern struct SceneExitRegion gTunnelRuinsLadyStatuesRegions[];
extern u32 gTunnelRuinsExits[];
extern struct ActorPlacement gTunnelRuinsGreatStatueActorPlacements[];
extern struct ActorPlacement gTunnelRuinsActorPlacementsOther[];
extern struct Trigger gTunnelRuinsGreatStatueTriggers[];
extern struct Trigger gTunnelRuinsTriggersOther[];

#endif
