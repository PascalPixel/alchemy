#ifndef ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_H
#define ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern u16 gCrossboneIsle_EntranceCells[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct SceneEntrance gCrossboneIsle_IsleEntrances[];
extern struct SceneEntrance gCrossboneIsle_CaveEntranceEntrances[];
extern struct SceneEntrance gCrossboneIsle_FloatingLogsEntrances[];
extern struct SceneEntrance gCrossboneIsle_PierEntrances[];
extern struct SceneEntrance gCrossboneIsle_PirateShipEntrances[];
extern u32 gCrossboneIsleExits[];
extern struct ActorTemplate gCrossboneIsle_NoActorTemplates[];
extern struct ActorTemplate gCrossboneIsle_IsleActorTemplates[];
extern struct ActorTemplate gCrossboneIsle_FloatingLogsActorTemplates[];
extern struct ActorTemplate gCrossboneIsle_CorridorActorTemplates[];
extern struct MapEvent gCrossboneIsle_CorridorEvents[];
extern struct MapEvent gCrossboneIsle_CaveEntranceEvents[];
extern struct MapEvent gCrossboneIsle_IsleEvents[];
extern struct MapEvent gCrossboneIsle_FloatingLogsEvents[];
extern struct MapEvent gCrossboneIsle_PierEvents[];
extern struct MapEvent gCrossboneIsle_PirateShipEvents[];

#endif
