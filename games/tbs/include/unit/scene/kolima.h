#ifndef ALCHEMY_UNIT_MAP_KOLIMA_H
#define ALCHEMY_UNIT_MAP_KOLIMA_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"

struct SceneEntrance;
struct MapEvent;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Kolima_GetEntrances(void);
const struct SceneExitRegion *Kolima_GetRegions(void);
const u32 *Kolima_GetExits(void);
const struct ActorTemplate *Kolima_GetActorTemplates(void);
const struct MapEvent *Kolima_GetEvents(void);
s32 Kolima_Initialize(void);
extern s32 gKolima_GaretWalksIn[];
extern s32 gKolima_IvanWalksIn[];
extern s32 gKolima_MiaWalksIn[];
extern s32 gKolima_PartySquashActions[];
extern s32 gKolima_PartyRestoreScaleActions[];
extern s32 gKolima_ProtectiveFieldGrowActions[];
extern s32 gKolima_ProtectiveFieldShrinkActions[];
extern s32 gKolima_EntryActions[];
extern s32 gKolima_DebrisScript[];
extern s32 gKolima_FaceLeaderScript[];
extern u16 gKolima_DoorCells[];
extern struct SceneEntrance gKolimaEntrances[];
extern struct SceneEntrance gKolimaBridgeEntrances[];
extern struct SceneEntrance gKolimaTunnelEntrances[];
extern struct SceneExitRegion gKolimaTunnelRegions[];
extern u32 gKolimaExits[];
extern struct ActorTemplate gKolimaTunnelActorTemplates[];
extern struct ActorTemplate gKolimaActorTemplates[];
extern struct ActorTemplate gKolima_EntryScript[];
extern struct ActorTemplate gKolimaBridgeActorTemplates[];
extern struct MapEvent gKolimaEvents[];
extern struct MapEvent gKolimaBridgeEvents[];
extern s32 gKolima_GoldenRingsScript[];
extern s32 gKolima_RegionScript[];
extern s32 gFallingEffectScript[];

#endif
