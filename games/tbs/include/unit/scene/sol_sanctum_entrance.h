#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_ENTRANCE_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *SolSanctumEntrance_GetEntrances(void);
const struct SceneExitRegion *SolSanctumEntrance_GetRegions(void);
const u32 *SolSanctumEntrance_GetExits(void);
const struct ActorTemplate *SolSanctumEntrance_GetActorTemplates(void);
const struct MapEvent *SolSanctumEntrance_GetEvents(void);
s32 SolSanctumEntrance_Initialize(void);
extern struct SceneEntrance gSolSanctumEntrance_FallbackEntrances[];
extern struct SceneEntrance gSolEntrance_Entrances[];
extern struct SceneEntrance gSolSanctumReliefs_Entrances[];
extern u32 gSolSanctumEntrance_Exits[];
extern struct ActorTemplate gSolSanctumEntrance_FallbackActorTemplates[];
extern struct ActorTemplate gSolSanctumReliefs_ActorTemplates[];
extern struct ActorTemplate gSolSanctumReliefs_FirstReliefRoomActorTemplates[];
extern struct ActorTemplate gSolSanctumReliefs_SecondReliefRoomActorTemplates[];
extern struct ActorTemplate gSolEntrance_ActorTemplates[];
extern struct MapEvent gSolSanctumEntrance_FallbackEvents[];
extern struct MapEvent gSolEntrance_Events[];
extern struct MapEvent gSolSanctumReliefs_Events[];
extern struct MapEvent gSolSanctumReliefs_FirstReliefRoomEvents[];
extern struct MapEvent gSolSanctumReliefs_SecondReliefRoomEvents[];
extern s32 gSolSanctumPushSteps[];

#endif
