#ifndef ALCHEMY_MODULE_SCENE_KOLIMA_H
#define ALCHEMY_MODULE_SCENE_KOLIMA_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
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
