#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_H

#include "types.h"
#include "map_tables.h"
#include "field_runtime.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Kolima_GetEntrances(void);
const struct SceneRegion *Kolima_GetRegions(void);
const u32 *Kolima_GetExits(void);
const struct ScenePlacement *Kolima_GetPlacements(void);
const struct SceneEvent *Kolima_GetEvents(void);
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
extern struct SceneRegion gKolimaTunnelRegions[];
extern u32 gKolimaExits[];
extern struct ScenePlacement gKolimaTunnelPlacements[];
extern struct ScenePlacement gKolimaPlacements[];
extern struct ScenePlacement gKolima_EntryScript[];
extern struct ScenePlacement gKolimaBridgePlacements[];
extern struct SceneEvent gKolimaEvents[];
extern struct SceneEvent gKolimaBridgeEvents[];
extern s32 gKolima_Object26Script[];
extern s32 gKolima_RegionScript[];
extern s32 gFallingEffectScript[];

#endif
