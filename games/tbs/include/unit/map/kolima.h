#ifndef ALCHEMY_UNIT_MAP_KOLIMA_H
#define ALCHEMY_UNIT_MAP_KOLIMA_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Kolima_GetWarpDests(void);
const struct MapWarpRegion *Kolima_GetRegions(void);
const u32 *Kolima_GetWarps(void);
const struct ObjectEventTemplate *Kolima_GetObjectEventTemplates(void);
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
extern struct MapWarpDest gKolimaWarpDests[];
extern struct MapWarpDest gKolimaBridgeWarpDests[];
extern struct MapWarpDest gKolimaTunnelWarpDests[];
extern struct MapWarpRegion gKolimaTunnelRegions[];
extern u32 gKolimaWarps[];
extern struct ObjectEventTemplate gKolimaTunnelObjectEventTemplates[];
extern struct ObjectEventTemplate gKolimaObjectEventTemplates[];
extern struct ObjectEventTemplate gKolima_EntryScript[];
extern struct ObjectEventTemplate gKolimaBridgeObjectEventTemplates[];
extern struct MapEvent gKolimaEvents[];
extern struct MapEvent gKolimaBridgeEvents[];
extern s32 gKolima_GoldenRingsScript[];
extern s32 gKolima_RegionScript[];
extern s32 gFallingEffectScript[];

#endif
