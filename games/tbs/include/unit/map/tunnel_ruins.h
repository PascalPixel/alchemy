#ifndef ALCHEMY_UNIT_MAP_TUNNEL_RUINS_H
#define ALCHEMY_UNIT_MAP_TUNNEL_RUINS_H

#include "types.h"
#include "map_tables.h"
#include "field_coordinate.h"
#include "field_runtime.h"

struct MapWarpDest;
struct MapWarpRegion;

const struct MapWarpDest *TunnelRuins_GetWarpDests(void);
const struct MapWarpRegion *TunnelRuins_GetRegions(void);
const u32 *TunnelRuins_GetWarps(void);
const struct ObjectEventTemplate *TunnelRuins_GetObjectEventTemplates(void);
const struct MapEvent *TunnelRuins_GetEvents(void);
s32 TunnelRuins_Initialize(void);
extern s32 gStagedObjectEvent_DirectionSteps[];
extern u32 gStagedObjectEvent_FootprintKinds[];
extern s32 gStagedObjectEvent_FootprintBounds[];
extern s32 gShortEffectScript[];
extern s32 gMediumEffectScript[];
extern s32 gLongEffectScript[];
extern u32 gTunnelRuins_BalanceHeights[];
extern const s32 *gEffectScripts[];
extern s32 gTunnelRuins_ShrinkScript[];
extern s32 gTunnelRuins_StretchScript[];
extern s32 gTunnelRuins_DouseSplashScript[];
extern u16 gTunnelRuins_DoorOpenSteps[];
extern u16 gTunnelRuins_DoorCloseSteps[];
extern struct MapWarpDest gTunnelRuinsWarpDestsOther[];
extern struct MapWarpDest gTunnelRuinsGreatStatueWarpDests[];
extern struct MapWarpDest gTunnelRuinsLadyStatuesWarpDests[];
extern struct MapWarpRegion gTunnelRuinsLadyStatuesRegions[];
extern u32 gTunnelRuinsWarps[];
extern struct ObjectEventTemplate gTunnelRuinsGreatStatueObjectEventTemplates[];
extern struct ObjectEventTemplate gTunnelRuinsObjectEventTemplatesOther[];
extern struct MapEvent gTunnelRuinsGreatStatueEvents[];
extern struct MapEvent gTunnelRuinsEventsOther[];

#endif
