#ifndef ALCHEMY_UNIT_MAP_TUNNEL_RUINS_H
#define ALCHEMY_UNIT_MAP_TUNNEL_RUINS_H

#include "types.h"
#include "map_tables.h"
#include "field_coordinate.h"
#include "scene_header.h"

struct SceneEntrance;
struct SceneExitRegion;

const struct SceneEntrance *TunnelRuins_GetEntrances(void);
const struct SceneExitRegion *TunnelRuins_GetRegions(void);
const u32 *TunnelRuins_GetExits(void);
const struct ActorTemplate *TunnelRuins_GetActorTemplates(void);
const struct MapEvent *TunnelRuins_GetEvents(void);
s32 TunnelRuins_Initialize(void);
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern u32 gTunnelRuins_BalanceHeights[];
extern const s32 *gFieldEffect_DurationScripts[];
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
extern struct ActorTemplate gTunnelRuinsGreatStatueActorTemplates[];
extern struct ActorTemplate gTunnelRuinsActorTemplatesOther[];
extern struct MapEvent gTunnelRuinsGreatStatueEvents[];
extern struct MapEvent gTunnelRuinsEventsOther[];

#endif
