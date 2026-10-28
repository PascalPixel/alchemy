#ifndef ALCHEMY_UNIT_MAP_KOLIMA_H
#define ALCHEMY_UNIT_MAP_KOLIMA_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *Kolima_GetEntrances(void);
const struct SceneExitRegion *Kolima_GetRegions(void);
const u32 *Kolima_GetExits(void);
const struct ActorPlacement *Kolima_GetActorPlacements(void);
const struct Trigger *Kolima_GetTriggers(void);
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
extern struct ActorPlacement gKolimaTunnelActorPlacements[];
extern struct ActorPlacement gKolimaActorPlacements[];
extern struct ActorPlacement gKolima_EntryScript[];
extern struct ActorPlacement gKolimaBridgeActorPlacements[];
extern struct Trigger gKolimaTriggers[];
extern struct Trigger gKolimaBridgeTriggers[];
extern s32 gKolima_GoldenRingsScript[];
extern s32 gKolima_RegionScript[];
extern s32 gFallingEffectScript[];

#endif
